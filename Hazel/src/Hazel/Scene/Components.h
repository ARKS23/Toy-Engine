#pragma once

#include "Hazel/Scene/SceneCamera.h"
#include "Hazel/Core/UUID.h"
#include "Hazel/Renderer/Texture.h"

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Hazel {
	class ScriptableEntity;

	/* Entity 全局标识符 */
	struct IDComponent {
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	/* Entity 名称 */
	struct TagComponent {
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string &tag) : Tag(tag) {}
	};

	/* 空间信息结构体 */
	struct TransformComponent {
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.f ,1.f ,1.f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation) : Translation(translation) {}

		glm::mat4 GetTransform() const {
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation)); // 转成四元数

			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;		// 2D渲染控制贴图重复次数

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CircleRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float Thickness = 1.0f;		// 厚度
		float Fade = 0.005f;		// 边缘柔化参数

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

	struct CameraComponent {
		SceneCamera camera;
		bool Primary = true;			// 是否为主摄像机
		bool FixedAspectRatio = false;	// 锁定长宽比

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	/* 之后接入脚本使用 */
	struct ScriptComponent {
		std::string ClassName;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
	};

	/* 这种写法值得学习，OOP享受ECS的性能 */
	struct HAZEL_API NativeScriptComponent {
		ScriptableEntity* Instance = nullptr;

		/* 函数指针: 返回值 | 变量名 | 参数列表 */
		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind() {
			InstantiateScript = []() {return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	/* TODO: 物理部分 */
	struct Rigidbody2DComponet {
		int a = 0;
	};

	struct BoxCollider2DComponent {
		int a = 0;
	};

	struct CircleCollider2Dcomponent {
		int a = 0;
	};

	/* TODO: 字体部分 */
	//struct TextComponent
	//{
	//	std::string TextString;
	//	Ref<Font> FontAsset = Font::GetDefault();
	//	glm::vec4 Color{ 1.0f };
	//	float Kerning = 0.0f;
	//	float LineSpacing = 0.0f;
	//};

	struct TextComponent {
		int a = 0;
	};


	template<typename... Component>
	struct ComponentGroup {};

	/* 类型列表 */
	using AllComponents = ComponentGroup<TransformComponent, SpriteRendererComponent,
	CircleRendererComponent, CameraComponent, ScriptComponent, NativeScriptComponent,
	Rigidbody2DComponet, BoxCollider2DComponent, CircleCollider2Dcomponent>;
}