#include "hzpch.h"
#include "OpenGLShader.h"

namespace Hazel {
	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		HZ_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) : m_FilePath(filepath) {
		// 单文件shader，vertex shader 和 fragment shader在同一文件，用#type fragment分割
		
		std::string source = ReadFile(filepath);
		PreProcess(source);
		CreateProgram();

		// 设置shader name
		auto lastSlash = filepath.find_last_of("/");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : m_Name(name){
		// TODO：待改进
		m_OpenGLSourceCode[GL_VERTEX_SHADER] = vertexSrc;
		m_OpenGLSourceCode[GL_FRAGMENT_SHADER] = fragmentSrc;
		CreateProgram();
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const {
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const {
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		UploadUniformFloat2(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath) {
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary); 

		if (in) {
			in.seekg(0, std::ios::end);
			size_t size = in.tellg(); // 获取文件大小

			if (size != -1) {
				result.resize(size);
				in.seekg(0, std::ios::beg); // 回到开头
				in.read(&result[0], size);  // 读取到result
				in.close();
			}
			else {
				HZ_CORE_ERROR("Could not read from file {0}", filepath);
			}
		}
		else {
			HZ_CORE_ERROR("Could not read from file {0}", filepath);
		}

		return result;
	}

	void OpenGLShader::PreProcess(const std::string& source) {
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); // 查找第一个#type

		if (pos == std::string::npos) {
			HZ_CORE_ERROR("Shader PreProcess failed: No '#type' token found in shader file! Check spelling?");
			return;
		}

		while (pos != std::string::npos) {
			// 找到目前行的末尾
			size_t eol = source.find_first_of("\r\n", pos);
			HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");

			// 提取类型名称
			size_t begin = pos + typeTokenLength + 1; // 跳过#type
			std::string type = source.substr(begin, eol - begin);
			HZ_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specification");

			// 查找下一个#type的位置或文件末尾
			size_t nextLinePos = source.find_first_not_of("\r\n", eol); // 跳过换行符
			HZ_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); // 找下一个 #type

			// 获取源码片段
			m_OpenGLSourceCode[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}
	}

	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		// TODO:待实现
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		// TODO:待实现
	}

	void OpenGLShader::CreateProgram() {
		unsigned int vertexShader, fragmentShader;

		const char* vertexShaderCode = m_OpenGLSourceCode[GL_VERTEX_SHADER].c_str();
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
		glCompileShader(vertexShader);
		checkShaderCompile(vertexShader, "vertex shader:");

		const char* fragmentShaderCode = m_OpenGLSourceCode[GL_FRAGMENT_SHADER].c_str();
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
		glCompileShader(fragmentShader);
		checkShaderCompile(fragmentShader, "fragment shader:");

		// 进行着色器链接操作
		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);
		glLinkProgram(m_RendererID);
		int linkSucess;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &linkSucess);
		if (!linkSucess) {
			char linkLog[512];
			glGetProgramInfoLog(m_RendererID, 512, nullptr, linkLog);
			HZ_CORE_ERROR("{0}: {1}", "ERROR::SHADER::PROGRAM::LINKING_FAILED", linkLog);
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData)
	{
		// 待实现
	}

	void OpenGLShader::checkShaderCompile(unsigned int id, const std::string& msg) {
		int success;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		char logMsg[512];
		if (!success) {
			glGetShaderInfoLog(id, 512, nullptr, logMsg);
			HZ_CORE_ERROR("{0}: {1}", msg, logMsg);
		}
	}
}