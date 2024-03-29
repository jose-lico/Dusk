#pragma once

#include "AssetDatabase.h"
#include "Core/Application/Application.h"
#include "Utils/Memory/Memory.h"
#include "AssetHandler.h"

#include <unordered_map>
#include <vector>

namespace DuskEngine
{
	class Material;
	struct Texture;
	class Mesh;
	struct ShaderNew;

	template<typename T>
	class AssetPool
	{
	public:
		AssetPool() = default;
		~AssetPool() = default;

		uint32_t AddToPool(std::unordered_map<uuids::uuid, uint32_t>& handleMap, const uuids::uuid& uuid)
		{
			if (handleMap.find(uuid) == handleMap.end())
			{
				if constexpr (std::is_same<T, Shader>::value)
					m_Pool.push_back(Application::Get().GetAssetDatabase().LoadShader(uuid));
				else if constexpr (std::is_same<T, Texture>::value)
					m_Pool.push_back(Application::Get().GetAssetDatabase().LoadTexture(uuid, m_AssetHandler->m_ProjectPath));
				else if constexpr (std::is_same<T, Mesh>::value)
					m_Pool.push_back(Application::Get().GetAssetDatabase().LoadModel(uuid));
				else if constexpr (std::is_same<T, Material>::value)
					m_Pool.push_back(Application::Get().GetAssetDatabase().LoadMaterial(uuid, m_AssetHandler));
				// add assert

				handleMap[uuid] = (uint32_t)m_Pool.size() - 1;
				return (uint32_t)m_Pool.size() - 1;
			}

			return handleMap[uuid];
		};

		T& operator()(uint32_t handle)
		{
			return m_Pool[handle];
		}

	private:
		std::vector<T> m_Pool;

		AssetHandler* m_AssetHandler = nullptr;
		friend class AssetHandler;
	};
}
