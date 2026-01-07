#pragma once

#include "Hazel/Core/Core.h"
//#include "Hazel/Scene/Entity.h"
#include "Hazel/Scene/Scene.h"

namespace Hazel {
	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& context);	// 点击play切换场景时需要用到

		void OnImGuiRender();	// 渲染入口: 同时负责渲染场景层级面板和属性面板

		Entity GetSelectEntity() const { return m_SelectionContext; }
		void SelectedEntity(Entity entity);

	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string entryName);	// 添加组件菜单的辅助模板

		void DrawEntityNode(Entity entity);	// 绘制物体名字和处理鼠标时间
		void DrawComponents(Entity entity);	// 绘制组件详情

	private:
		Ref<Scene> m_Context;		// 当前显示的场景
		Entity m_SelectionContext;	// 当前选中的Entity
	};
}

