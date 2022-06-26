#pragma once

#include "AssetDatabase.h"

#include "Utils/Memory/Memory.h"

#include <unordered_map>
#include <vector>

namespace DuskEngine
{
	class AssetHandler;

	class Material;
	class Texture;
	class Mesh;
	class Shader;

	template<typename T>
	class _AssetPool
	{
	public:
		_AssetPool() = default;
		~_AssetPool() = default;

		uint32_t AddToPool(std::unordered_map<uuids::uuid, uint32_t>& handleMap, const uuids::uuid& uuid)
		{
			if (handleMap.find(uuid) == handleMap.end())
			{
				if constexpr (std::is_same<T, Shader>::value)
					m_Pool.push_back(AssetDatabase::LoadShader(uuid));
				else if constexpr (std::is_same<T, Texture>::value)
					m_Pool.push_back(AssetDatabase::LoadTexture(uuid));
				else if constexpr (std::is_same<T, Mesh>::value)
					m_Pool.push_back(AssetDatabase::LoadModel(uuid));
				else if constexpr (std::is_same<T, Material>::value)
					m_Pool.push_back(AssetDatabase::LoadMaterial(uuid, m_AssetHandler));

				handleMap[uuid] = m_Pool.size() - 1;
				return m_Pool.size() - 1;
			}

			return handleMap[uuid];
		};

		UniqueRef<T>& operator()(uint32_t handle)
		{
			return m_Pool[handle];
		}

	private:
		std::vector<UniqueRef<T>> m_Pool;

		AssetHandler* m_AssetHandler = nullptr;
		friend class AssetHandler;
	};
}
