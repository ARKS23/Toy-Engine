#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Log.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Core/UUID.h"
#include "Hazel/Renderer/Camera/EditorCamra.h"

#include "entt.hpp"

#include "Hazel/Scene/Components.h"

//class b2World;

namespace Hazel {
	/* 封装的Entity类，让ECS像OOP一样操作，该类根据Entity模型进行设计 */
	/* 理解这个Entity模型，一个Entity对应每一种Component类型最多只能拥有一个示例 */

	class Scene;
	class HAZEL_API Entity {
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene) : m_EntityHandle(handle), m_Scene(scene) {}
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			HZ_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");

			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);	// 这里进行了调用，Scene需要提供特化版本

			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args) {
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);

			return component;
		}

		template<typename T>
		T& GetComponent() {
			HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not hace component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent() {
			//return m_Scene->m_Registry.has<T>(m_EntityHandle);
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent() {
			HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not hace component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }	// 类型转换，场景Copy时map会用到
		operator uint32_t() const { return static_cast<uint32_t>(m_EntityHandle); }

		/* IDComponent和TagComponent也是原生entity的Component，但这两个Component用于身份认证 */
		UUID GetUUID() { return GetComponent<IDComponent>().ID; }
		const std::string& GetName() { return GetComponent<TagComponent>().Tag; }

		bool operator==(const Entity& other) const {
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const {
			return !(*this == other);
		}

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;	// 所有的组件操作转发给Scene的registry
	};
}


namespace Hazel {
	//class Entity;	// 前置声明避免循环引用，在Entity中引用了Scene

	class HAZEL_API Scene {
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);	// 深拷贝场景，点击运行时copy一份作为运行时场景

		/* 场景实体管理 */
		Entity CreateEntity(const std::string& name = std::string());	// 创建实体的接口
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());	// 带UUID的创建(Copy中会调用到)
		void DestroyEntity(Entity entity);	// 销毁实体

		/* 运行状态管理 */
		void OnRuntimeStart();
		void OnRuntimeStop();

		/* 物理模拟 */
		void OnSimulationStart();
		void OnSimulationStop();

		/* 更新循环 */
		void OnUpdateRuntime(Timestep ts);							// 游戏运行时调用: 脚本 + 物理 + 渲染
		void OnUpdateSimulation(Timestep ts, EditorCamera& camera);	// 物理模拟时调用: 物理 + 渲染
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);		// 编辑器状态下调用: 渲染

		void OnViewportResize(uint32_t width, uint32_t height);

		/* 工具函数 */
		Entity DuplicateEntity(Entity entity);
		Entity FindEntityByName(std::string_view name);
		Entity GetEntityByUUID(UUID uuid);
		Entity GetPrimaryCameraEntity();

		/* 暂停与逐帧调试 */
		bool IsRunning() const { return m_IsRunning; }
		bool IsPaused() const { return m_IsPaused; }
		void SetPaused(bool paused) { m_IsPaused = paused; }
		void Step(int frames = 1);	// 按帧步进调试

		template<typename... Components>
		auto GetAllEntitesWith() { return m_Registry.view<Components ...>(); }	// 模板工具，返回所拥有特定组件的实体视图

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);	// 组件被添加时触发的回调: 进行一些初始化逻辑

		// 物理引擎的开关逻辑
		//void OnPhysics2DStart();
		//void OnPhysics2DStop();

		// 统一渲染入口
		void RenderScene(EditorCamera& camera);

	private:
		entt::registry m_Registry; // EnTT数据库
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		bool m_IsRunning = false;
		bool m_IsPaused = false;
		int m_StepFrames = 0;

		// Box2D的物理世界指针
		//b2World* m_PhysicsWorld = nullptr;

		std::unordered_map<UUID, entt::entity> m_EntityMap;

		friend class Entity;
		friend class SceneSerializer;		// 序列化器需要读写所有数据
		friend class SceneHierarchyPanel;	// 面板需要展示所有数据
	};

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component) {
		static_assert(sizeof(T) == 0);
	}

	/* -------------------------- 模板特化函数: 这个函数在Entity中AddComponent进行了调用 -------------------------- */
	/* 调用链：CopyComponentIfExists ->  Entity::AddOrReplaceComponent -> Scene::OnComponentAdded , 因此要补全特化类型*/
	template<>
	inline void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component) {}

	template<>
	inline void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {}

	template<>
	inline void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	inline void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component) {}

	template<>
	inline void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {}

	template<>
	inline void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component) {}

	template<>
	inline void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {}

	template<>
	inline void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {}

	template<>
	inline void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component) {}

	template<>
	inline void Scene::OnComponentAdded<Rigidbody2DComponet>(Entity entity, Rigidbody2DComponet& component) {}

	template<>
	inline void Scene::OnComponentAdded<CircleCollider2Dcomponent>(Entity entity, CircleCollider2Dcomponent& component) {}

	template<>
	inline void Scene::OnComponentAdded<TextComponent>(Entity entity, TextComponent& component) {}
}

