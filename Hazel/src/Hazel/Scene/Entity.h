#pragma once
#include "Hazel/Core/Log.h"
#include "Hazel/Core/UUID.h"
#include "Components.h"
//#include "Scene.h"

#include "entt.hpp"


#if 0
namespace Hazel {
	// Entity迁移到Scene.h中实现
	/* 封装的Entity类，让ECS像OOP一样操作，该类根据Entity模型进行设计 */
	/* 理解这个Entity模型，一个Entity对应每一种Component类型最多只能拥有一个示例 */
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
#endif

