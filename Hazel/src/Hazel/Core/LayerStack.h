#pragma once
#include "Hazel/Core/Core.h"
#include "Layer.h"

#include <vector>

namespace Hazel {
	class HAZEL_API LayerStack {
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer); // 插入layer
		void PushOverlay(Layer* overlay); // 插入overlay（优先级更高）
		void PopLayer(Layer* layer);	// layer移出容器
		void PopOverlay(Layer* overlay); // overlay移出容器

		// 迭代器接口
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;  // 底层存储容器
		unsigned int m_LayerInsertIndex = 0; // 插入位置指针，记录layer和Overlay的分界线
	};
}

/*
	区分layer和overlay，调度优先级不同
	layer插入在容器中部
	overlay插入在容器末尾优先处理

	[ GameLayer ]
	[ PhysicsLayer ]
	[ AudioLayer ]
	---------------- m_LayerInsertIndex
	[ DebugOverlay ]
	[ ImGuiOverlay ]

*/

