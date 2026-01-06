#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Core/UUID.h"
#include "Hazel/Renderer/Camera/EditorCamra.h"

#include "entt.hpp"

//class b2World;

namespace Hazel {
	class Entity;	// 前置声明避免循环引用，在Entity中引用了Scene

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
}

