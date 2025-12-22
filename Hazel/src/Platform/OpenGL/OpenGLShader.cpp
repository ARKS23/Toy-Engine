#include "hzpch.h"
#include "OpenGLShader.h"

namespace Hazel {
	OpenGLShader::OpenGLShader(const std::string& filepath) : m_FilePath(filepath) {
		// TODO: 单文件shader
		HZ_CORE_WARN("TODO: single file constructor");
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

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		// 待实现
		return std::string();
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		// 待实现
		return std::unordered_map<GLenum, std::string>();
	}

	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		// 待实现
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		// 待实现
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