#pragma once

#include <vector>

namespace DuskEngine
{
	class Layer;

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		operator uint32_t() { return (uint32_t)m_Layers.size(); }
		Layer* operator[](uint32_t i) { return m_Layers[i]; }
	private:
		std::vector<Layer*> m_Layers;
		uint32_t m_LayerInsertIndex = 0;
	};
}