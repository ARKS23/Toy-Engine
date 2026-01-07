#include "hzpch.h"
#include "Renderer2D.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/Renderer/UniformBuffer.h"
#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Renderer/Camera/EditorCamra.h"

namespace Hazel {
	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TillingFactor;

		int EntityID;
	};

	struct CircleVertex {
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		int EntityID;
	};

	struct LineVertex {
		glm::vec3 Position;
		glm::vec4 Color;

		int EntityID;
	};

	// TODO: 文本渲染
	struct TextVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;

		int EntityID;
	};

	struct Renderer2DData {
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVetexBuffer;
		Ref<Shader> QuadShader;
		Ref<Texture2D> WhiteTexutre;

		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;

		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;
		
		Ref<VertexArray> TextVertexArray;
		Ref<VertexBuffer> TextVertexBuffer;
		Ref<Shader> TextShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;	// Base指向数组首地址
		QuadVertex* QuadVertexBufferPtr = nullptr;	// Ptr指向当前写到的位置

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		uint32_t TextIndexCount = 0;
		TextVertex* TextVertexBufferBase = nullptr;
		TextVertex* TextVertexBufferPtr = nullptr;
		
		float LineWidth = 2.0f;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;	// 纹理数组
		uint32_t TextureSlotIndex = 1;  // white texutre == 0

		Ref<Texture2D> FontAtlasTexture;

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;

		struct CameraData {
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> cameraUniformBuffer;
	};
	static Renderer2DData s_2DData;


	void Renderer2D::Init() {
		/* Quad的VAO, VBO, EBO构建 */
		s_2DData.QuadVertexArray = VertexArray::Create();
		s_2DData.QuadVetexBuffer = VertexBuffer::Create(s_2DData.MaxVertices * sizeof(QuadVertex));
		s_2DData.QuadVetexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float, "a_TexIndex"},
			{ShaderDataType::Float, "a_TilingFactor"},
			{ShaderDataType::Int, "a_EntityID"}
		});
		s_2DData.QuadVertexArray->AddVertexBuffer(s_2DData.QuadVetexBuffer); // VBO绑定到VAO

		s_2DData.QuadVertexBufferBase = new QuadVertex[s_2DData.MaxVertices];	// CPU侧顶点数组基地址

		uint32_t* quadIndices = new uint32_t[s_2DData.MaxIndices]; // 临时数组用于构造索引数组EBO
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_2DData.MaxIndices; i += 6) {
			// 第一个三角形[0, 1, 2], 第二个三角形[2, 3, 0], 顺时针构造（索引顺序构造影响面剔除）
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		Ref<IndexBuffer> QuadEBO = IndexBuffer::Create(quadIndices, s_2DData.MaxIndices);
		s_2DData.QuadVertexArray->SetIndexBuffer(QuadEBO); // EBO绑定到VAO
		delete[]quadIndices;

		/* Circle的VAO, VBO, EBO构建 */
		s_2DData.CircleVertexArray = VertexArray::Create();
		s_2DData.CircleVertexBuffer = VertexBuffer::Create(s_2DData.MaxVertices * sizeof(CircleVertex));
		s_2DData.CircleVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_WorldPosition"},
			{ShaderDataType::Float3, "a_LocalPosition"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float, "a_Thickness"},
			{ShaderDataType::Float, "a_Fade"},
			{ShaderDataType::Int, "a_EntityID"}
		});
		s_2DData.CircleVertexArray->AddVertexBuffer(s_2DData.CircleVertexBuffer);
		s_2DData.CircleVertexArray->SetIndexBuffer(QuadEBO); // 复用QuadEBO
		s_2DData.CircleVertexBufferBase = new CircleVertex[s_2DData.MaxVertices];

		/* Lines */
		s_2DData.LineVertexArray = VertexArray::Create();

		s_2DData.LineVertexBuffer = VertexBuffer::Create(s_2DData.MaxVertices * sizeof(LineVertex));
		s_2DData.LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"    },
			{ ShaderDataType::Int,    "a_EntityID" }
			});
		s_2DData.LineVertexArray->AddVertexBuffer(s_2DData.LineVertexBuffer);
		s_2DData.LineVertexBufferBase = new LineVertex[s_2DData.MaxVertices];

		/* Text
		s_2DData.TextVertexArray = VertexArray::Create();

		s_2DData.TextVertexBuffer = VertexBuffer::Create(s_2DData.MaxVertices * sizeof(TextVertex));
		s_2DData.TextVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Int,    "a_EntityID"     }
			});
		s_2DData.TextVertexArray->AddVertexBuffer(s_2DData.TextVertexBuffer);
		s_2DData.TextVertexArray->SetIndexBuffer(QuadEBO);
		s_2DData.TextVertexBufferBase = new TextVertex[s_2DData.MaxVertices];
		*/

		s_2DData.WhiteTexutre = Texture2D::Create(TextureSpecification());
		uint32_t WhiteTexutreData = 0xffffffff;
		s_2DData.WhiteTexutre->SetData(&WhiteTexutreData, sizeof(uint32_t));

		uint32_t samplers[s_2DData.MaxTextureSlots];
		for (uint32_t i = 0; i < s_2DData.MaxTextureSlots; ++i) samplers[i] = i;

		/* 着色器: 待改良 */
		s_2DData.QuadShader = Shader::Create("assets/shaders/quadShader.glsl");
		s_2DData.CircleShader = Shader::Create("assets/shaders/Renderer2D_Circle.glsl");
		s_2DData.LineShader = Shader::Create("assets/shaders/quadShader.glsl");
		//s_2DData.TextShader = Shader::Create("");

		s_2DData.TextureSlots[0] = s_2DData.WhiteTexutre; // 0 == white

		s_2DData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_2DData.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_2DData.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_2DData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_2DData.cameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::Shutdown() {
		delete[] s_2DData.QuadVertexBufferBase;
		delete[] s_2DData.CircleVertexBufferBase;
		delete[] s_2DData.LineVertexBufferBase;
		//delete[] s_2DData.TextVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		s_2DData.CameraBuffer.ViewProjection = camera.GetViewProjetionMatrix();
		s_2DData.cameraUniformBuffer->SetData(&s_2DData.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform) {
		s_2DData.CameraBuffer.ViewProjection = camera.GetProjectionMatrix() * glm::inverse(transform);
		s_2DData.cameraUniformBuffer->SetData(&s_2DData.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera) {
		HZ_PROFILE_FUNCTION();

		s_2DData.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_2DData.cameraUniformBuffer->SetData(&s_2DData.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::EndScene() {
		Flush();
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_2DData.Stats, 0, sizeof(Statistics));
	}


	void Renderer2D::StartBatch() {
		s_2DData.QuadIndexCount = 0;
		s_2DData.QuadVertexBufferPtr = s_2DData.QuadVertexBufferBase;

		s_2DData.CircleIndexCount = 0;
		s_2DData.CircleVertexBufferPtr = s_2DData.CircleVertexBufferBase;

		s_2DData.LineVertexCount = 0;
		s_2DData.LineVertexBufferPtr = s_2DData.LineVertexBufferBase;

		s_2DData.TextIndexCount = 0;
		s_2DData.TextVertexBufferPtr = s_2DData.TextVertexBufferBase;

		s_2DData.TextureSlotIndex = 1;
	}

	void Renderer2D::Flush() {
		if (s_2DData.QuadIndexCount) {
			uint32_t dataSize = static_cast<uint32_t>((uint8_t*)s_2DData.QuadVertexBufferPtr - (uint8_t*)s_2DData.QuadVertexBufferBase); // 算出这一帧的数据量
			s_2DData.QuadVetexBuffer->SetData(s_2DData.QuadVertexBufferBase, dataSize);
			
			// 纹理绑定
			for (uint32_t i = 0; i < s_2DData.TextureSlotIndex; ++i)
				s_2DData.TextureSlots[i]->Bind(i);

			s_2DData.QuadShader->Bind();
			RenderCommand::DrawIndexed(s_2DData.QuadVertexArray, s_2DData.QuadIndexCount);
			s_2DData.Stats.DrawCalls++;
		}

		if (s_2DData.CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_2DData.CircleVertexBufferPtr - (uint8_t*)s_2DData.CircleVertexBufferBase);
			s_2DData.CircleVertexBuffer->SetData(s_2DData.CircleVertexBufferBase, dataSize);

			s_2DData.CircleShader->Bind();
			RenderCommand::DrawIndexed(s_2DData.CircleVertexArray, s_2DData.CircleIndexCount);
			s_2DData.Stats.DrawCalls++;
		}

		if (s_2DData.LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_2DData.LineVertexBufferPtr - (uint8_t*)s_2DData.LineVertexBufferBase);
			s_2DData.LineVertexBuffer->SetData(s_2DData.LineVertexBufferBase, dataSize);

			s_2DData.LineShader->Bind();
			RenderCommand::SetLineWidth(s_2DData.LineWidth);
			RenderCommand::DrawLines(s_2DData.LineVertexArray, s_2DData.LineVertexCount);
			s_2DData.Stats.DrawCalls++;
		}

		/*
		if (s_2DData.TextIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_2DData.TextVertexBufferPtr - (uint8_t*)s_2DData.TextVertexBufferBase);
			s_2DData.TextVertexBuffer->SetData(s_2DData.TextVertexBufferBase, dataSize);

			auto buf = s_2DData.TextVertexBufferBase;
			s_2DData.FontAtlasTexture->Bind(0);

			s_2DData.TextShader->Bind();
			RenderCommand::DrawIndexed(s_2DData.TextVertexArray, s_2DData.TextIndexCount);
			s_2DData.Stats.DrawCalls++;
		}
		*/
	}

	void Renderer2D::NextBatch() {
		Flush();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad(glm::vec3(position, 1.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		HZ_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tillingFactor /* = 1.0f */, const glm::vec4& tintColor /* = glm::vec4(1.0f) */) {
		DrawQuad(glm::vec3(position, 0.0f), size, texture, tillingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tillingFactor /* = 1.0f */, const glm::vec4& tintColor /* = glm::vec4(1.0f) */) {
		HZ_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tillingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID /* = -1 */) {
		HZ_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (s_2DData.QuadIndexCount >= Renderer2DData::MaxIndices) {	// 积攒的索引数量达到了上限
			NextBatch();
		}

		for (size_t i = 0; i < quadVertexCount; ++i) {	// 写入顶点信息
			s_2DData.QuadVertexBufferPtr->Position = transform * s_2DData.QuadVertexPositions[i];
			s_2DData.QuadVertexBufferPtr->Color = color;
			s_2DData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_2DData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_2DData.QuadVertexBufferPtr->TillingFactor = tilingFactor;
			s_2DData.QuadVertexBufferPtr->EntityID = entityID;
			s_2DData.QuadVertexBufferPtr++;
		}

		s_2DData.QuadIndexCount += 6;	// 画一个四边形需要6个索引
		s_2DData.Stats.QuadCount++;		// 数据统计
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor /* = 1.0f */, const glm::vec4& tintColor /* = glm::vec4(1.0f) */, int entityID /* = -1 */) {
		HZ_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_2DData.QuadIndexCount >= Renderer2DData::MaxIndices) {	// 积攒的索引数量达到了上限
			NextBatch();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_2DData.TextureSlotIndex; ++i) {
			if (*s_2DData.TextureSlots[i] == *texture) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) { // 这张纹理是这批次第一次出现
			if (s_2DData.TextureSlotIndex >= Renderer2DData::MaxTextureSlots) { // 纹理槽满了，使用新的批次
				NextBatch();
			}
			textureIndex = (float)s_2DData.TextureSlotIndex;
			s_2DData.TextureSlots[s_2DData.TextureSlotIndex] = texture;
			s_2DData.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_2DData.QuadVertexBufferPtr->Position = transform * s_2DData.QuadVertexPositions[i];
			s_2DData.QuadVertexBufferPtr->Color = tintColor;
			s_2DData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_2DData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_2DData.QuadVertexBufferPtr->TillingFactor = tilingFactor;
			s_2DData.QuadVertexBufferPtr->EntityID = entityID;
			s_2DData.QuadVertexBufferPtr++;
		}

		s_2DData.QuadIndexCount += 6;
		s_2DData.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		HZ_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness /*= 1.0f*/, float fade /*= 0.005f*/, int entityID /*= -1*/)
	{
		//HZ_PROFILE_FUNCTION();

		// TODO: implement for circles
		// if (s_2DData.QuadIndexCount >= Renderer2DData::MaxIndices)
		// 	NextBatch();

		for (size_t i = 0; i < 4; i++)
		{
			s_2DData.CircleVertexBufferPtr->WorldPosition = transform * s_2DData.QuadVertexPositions[i];
			s_2DData.CircleVertexBufferPtr->LocalPosition = s_2DData.QuadVertexPositions[i] * 2.0f;
			s_2DData.CircleVertexBufferPtr->Color = color;
			s_2DData.CircleVertexBufferPtr->Thickness = thickness;
			s_2DData.CircleVertexBufferPtr->Fade = fade;
			s_2DData.CircleVertexBufferPtr->EntityID = entityID;
			s_2DData.CircleVertexBufferPtr++;
		}

		s_2DData.CircleIndexCount += 6;

		s_2DData.Stats.QuadCount++;
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		s_2DData.LineVertexBufferPtr->Position = p0;
		s_2DData.LineVertexBufferPtr->Color = color;
		s_2DData.LineVertexBufferPtr->EntityID = entityID;
		s_2DData.LineVertexBufferPtr++;

		s_2DData.LineVertexBufferPtr->Position = p1;
		s_2DData.LineVertexBufferPtr->Color = color;
		s_2DData.LineVertexBufferPtr->EntityID = entityID;
		s_2DData.LineVertexBufferPtr++;

		s_2DData.LineVertexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color, entityID);
		DrawLine(p1, p2, color, entityID);
		DrawLine(p2, p3, color, entityID);
		DrawLine(p3, p0, color, entityID);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform * s_2DData.QuadVertexPositions[i];

		DrawLine(lineVertices[0], lineVertices[1], color, entityID);
		DrawLine(lineVertices[1], lineVertices[2], color, entityID);
		DrawLine(lineVertices[2], lineVertices[3], color, entityID);
		DrawLine(lineVertices[3], lineVertices[0], color, entityID);
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_2DData.Stats;
	}
}