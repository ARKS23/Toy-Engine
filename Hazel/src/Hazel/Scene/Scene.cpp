#include "hzpch.h"
#include "Scene.h"
#include "Entity.h"

#include "ScriptableEntity.h"
#include "Hazel/Renderer/Renderer2D.h"
#include "Components.h"

// Box2D
//#include "box2d/b2_world.h"
//#include "box2d/b2_body.h"
//#include "box2d/b2_fixture.h"
//#include "box2d/b2_polygon_shape.h"
//#include "box2d/b2_circle_shape.h"

namespace Hazel {
	Scene::Scene() {}

	Scene::~Scene() {
		//delete m_PhysicsWorld;
	}

	// 辅助函数负责把srcRegistry的某一组件拷贝到destRegistry对应的实体上
	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap) {
		// C++17:折叠表达式 (Fold Expression)：对模板参数包里的每一个 Component 类型，执行一次 Lambda
		(
			[&]() {
				auto view = src.view<Component>();	// 获取场景里所有拥有该组件的实体
				for (auto srcEntity : view) {
					entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);	// 两个场景的id可能不一样但是UUID一样
					Component& srcComponent = src.get<Component>(srcEntity);
					dst.emplace_or_replace<Component>(dstEntity, srcComponent);
				}
			}(),
				...);
	}

	// 包装函数，把CopyComponent的调用转发给上面的辅助函数
	// ComponentGroup是Components.h里的AllComponents，传给辅助函数后遍历所有型别,遵循开闭原则
	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap) {
		CopyComponent<Component...>(dst, src, enttMap);
	}

	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src) {
		// 折叠表达式：对模板列表里的每一个 Component 类型，执行一次 Lambda
		([&]()
			{
				// 如果源实体身上有这个组件，就给目标实体也装一个一模一样的
				if (src.HasComponent<Component>())
					dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src) {
		CopyComponentIfExists<Component...>(dst, src);
	}

	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		entt::registry& srcSceneRegistry = other->m_Registry;
		entt::registry& dstSceneRegistry = newScene->m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		// 新场景中把所有的Entity创建出来,只带ID和Name
		auto& idView = srcSceneRegistry.view<IDComponent>();
		for (auto e : idView) {
			UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;

			Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity; // 记录 UUID -> 新实体ID 的映射，方便后面拷组件用
		}

		// 拷贝所有组件，用到了之前的AllComponents
		CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
	}

	Entity Scene::CreateEntity(const std::string& name) {
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name) {
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);

		m_EntityMap[uuid] = entity;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity) {
		m_EntityMap.erase(entity.GetUUID());
		m_Registry.destroy(entity);
	}

	void Scene::OnRuntimeStart() {
		m_IsRunning = true;

		//OnPhysics2DStart();

		{
			//ScriptEngine::OnRuntimeStart(this);

			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view) {
				Entity entity = { e, this };
				//ScriptEngine::OnCreateEntity(entity);
			}
		}
	}

	void Scene::OnRuntimeStop() {
		m_IsRunning = false;

		// OnPhysics2DStop();

		// ScriptEngine::OnRuntimeStop();
	}

	void Scene::OnSimulationStart(){
		//OnPhysics2DStart();
	}

	void Scene::OnSimulationStop(){
		//OnPhysics2DStop();
	}

	void Scene::OnUpdateRuntime(Timestep ts) {
		if (!m_IsPaused || m_StepFrames-- > 0) {
			/* 脚本 */
			// C# (TODO)
			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view) {
				Entity entity = { e, this };
				//ScriptEngine::OnUpdateEntity(entity, ts);
			}

			// 遍历所有挂载了原生脚本组件的实体
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc) {
				if (!nsc.Instance) {
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}
				nsc.Instance->OnUpdate(ts);
			});

			/* 物理(TODO) */
		}


		/* 渲染 */
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransfrom;
		{
			// 找主摄像机
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			view.each([&](auto entity, TransformComponent& transform, CameraComponent& camera) {
				if (mainCamera) return;
				if (camera.Primary) {
					mainCamera = &camera.camera;
					cameraTransfrom = transform.GetTransform();
				}
			});
		}

		if (mainCamera) {
			Renderer2D::BeginScene(*mainCamera, cameraTransfrom);
			// Draw Sprites 跳过
			// 圆形绘制
			{
				auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
				view.each([](auto entity, TransformComponent& transform, CircleRendererComponent& circle) {
					Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
				});
			}

			// Draw text 跳过
			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdateSimulation(Timestep ts, EditorCamera& camera) {
		if (!m_IsPaused || m_StepFrames-- > 0)
		{
			// 物理模拟(TODO)
		}

		// Render
		RenderScene(camera);
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera) {
		RenderScene(camera);	// 编辑器模式只渲染
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height) {
		if (m_ViewportWidth == width && m_ViewportHeight == height)
			return;

		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		view.each([=](auto entity, CameraComponent& cameraComponent) {
			if (!cameraComponent.FixedAspectRatio) {	// 不锁宽高比的才进行设置
				cameraComponent.camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
			}
		});
	}

	Entity Scene::DuplicateEntity(Entity entity) {
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);
		CopyComponentIfExists(AllComponents{}, newEntity, entity);
		return newEntity;
	}

	Entity Scene::FindEntityByName(std::string_view name) {
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
				return Entity{ entity, this };
		}
		return {};
	}

	Entity Scene::GetEntityByUUID(UUID uuid) {
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return { m_EntityMap.at(uuid), this };
		return {};
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	void Scene::Step(int frames) {
		m_StepFrames = frames;
	}

	void Scene::RenderScene(EditorCamera& camera) {
		Renderer2D::BeginScene(camera);

		// 纹理
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);	// group会把内存中的数据紧凑
		for (auto entity : group) {
			auto [transf0rm, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			//Renderer2D::DrawSprite(transf0rm.GetTransform(), sprite, (int)entity);
		}

		// 圆形
		auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
		view.each([&](auto entity, auto& transform, auto& circle)
		{
			Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
		});

		// 文字
		Renderer2D::EndScene();
	}
}