#pragma once
#include "States.h"
#include "AssetFlags.h"

template<typename Derived, typename T>
class ResourceManager {

private:
	template<typename K, typename V>
	using map = std::unordered_map<K, V>;
	using sPtr = std::shared_ptr<T>;

	/*Keeps track of all loaded resources*/
	map<AssetFlags, std::pair<sPtr, unsigned int>> m_resources;
	/*Keeps track of all resources owned by a state*/
	map<States, std::set<AssetFlags>> m_stateResources;

	/*Mapping from full path to resource ID*/
	map<AssetFlags, std::string> m_pathReferences;

public:
	ResourceManager()
	{	}

	virtual ~ResourceManager() {
		freeAllResources();
	}

	sPtr getResource(const AssetFlags assetFlag)
	{
		auto res = findResource(assetFlag);
		return res ? res->first : nullptr;
	}

	bool aquireResource(const AssetFlags assetFlag, const std::string& path)
	{
		auto res = findResource(assetFlag);
		if (res) {
			++res->second;
			return true;
		}

		sPtr resource = loadResource(path);
		if (!resource)
			return false;

		m_pathReferences.emplace(assetFlag, path);
		m_resources.emplace(assetFlag, std::make_pair(resource, 1));
		return true;
	}

	bool aquireResource(const States state, const AssetFlags assetFlag, const std::string& path)
	{
		auto it = m_stateResources.find(state);
		if (it == m_stateResources.end())
			m_stateResources.emplace(state, std::set<AssetFlags>());

		if (aquireResource(assetFlag, path)) {
			m_stateResources[state].emplace(assetFlag);
			return true;
		}

		return false;
	}

	bool releaseResources(const AssetFlags assetFlag) {
		auto res = findResource(assetFlag);
		if (!res)
			return false;

		--res->second;
		if (!res->second)
			freeResource(assetFlag);
		return true;
	}

	bool releaseResources(const States state) {
		auto it = m_stateResources.find(state);
		if (it == m_stateResources.end())
			return false;

		for (auto items : it->second)
			releaseResources(items);

		m_stateResources.erase(it);
		return true;
	}

	void freeAllResources() {

		while (m_resources.begin() != m_resources.end()) {
			m_resources.erase(m_resources.begin());
		}

		m_stateResources.empty();
		m_pathReferences.empty();
	}

protected:
	virtual sPtr loadResource(const std::string& path) const = 0;

private:
	std::pair<sPtr, unsigned int>* findResource(const AssetFlags assetFlag) {
		auto itr = m_resources.find(assetFlag);
		return (itr != m_resources.end() ? &itr->second : nullptr);
	}

	bool freeResource(const AssetFlags assetFlag) {
		auto itr = m_resources.find(assetFlag);
		if (itr == m_resources.end())
			return false;

		m_resources.erase(itr);
		return true;
	}
};
