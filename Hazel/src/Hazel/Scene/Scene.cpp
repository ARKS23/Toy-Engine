#include "hzpch.h"
#include "Scene.h"
#include "Entity.h"

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
					auto& srcComponent = src.get<Component>(srcEntity);
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


}