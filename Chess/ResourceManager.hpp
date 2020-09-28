#pragma once
#include "States.h"
#include "AssetNames.h"

template<typename Derived, typename T>
class ResourceManager : NonCopyable {

private:
	template<typename K, typename V>
	using map = std::unordered_map<K, V>;

	/*Keeps track of all loaded resources*/
	map<AssetNames, std::pair<std::shared_ptr<T>, unsigned int>> m_resources;
	/*Keeps track of all resources owned by a state*/
	map<States, std::set<AssetNames>> m_stateResources;

	/*Mapping from full path to resource ID*/
	map<AssetNames, std::string> m_pathReferences;

public:
	ResourceManager()
	{	}

	virtual ~ResourceManager() {
		freeAllResources();
	}

	std::shared_ptr<T> getResource(const AssetNames asset)
	{
		auto res = findResource(asset);
		return res ? res->first : nullptr;
	}

	bool requireResource(const AssetNames asset)
	{
		auto res = findResource(asset);
		if (res) {
			++res->second;
			return true;
		}

		auto path = m_pathReferences.find(asset);
		if (path == m_pathReferences.end())
			return false;

		std::shared_ptr<T> resource = loadResource(path->second);
		if (!resource)
			return false;

		m_resources.emplace(asset, std::make_pair(resource, 1));
		return true;
	}
	bool requireResource(const States state, const AssetNames asset)
	{
		auto it = m_stateResources.find(state);
		if (it == m_stateResources.end())
			m_stateResources.emplace(state, std::set<AssetNames>());

		if (requireResource(asset)) {
			m_stateResources[state].emplace(asset);
			return true;
		}

		return false;
	}

	std::shared_ptr<T> requireAndGet(const AssetNames asset) {
		if (requireResource(asset))
			return getResource(asset);

		return nullptr;
	}
	std::shared_ptr<T> requireAndGet(const States state, const AssetNames asset) {
		if (requireResource(state, asset))
			return getResource(asset);

		return nullptr;
	}

	bool releaseResource(const AssetNames asset) {
		auto res = findResource(asset);
		if (!res)
			return false;

		--res->second;
		if (!res->second)
			freeResource(asset);
		return true;
	}
	bool releaseResource(const States state) {
		auto it = m_stateResources.find(state);
		if (it == m_stateResources.end())
			return false;

		for (auto items : it->second)
			releaseResource(items);

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

	std::string getPath(const AssetNames asset) {
		return "";
	}

	void registerPath(const AssetNames asset, const std::string& path) {
		m_pathReferences.emplace(asset, path);
	}

protected:
	virtual std::shared_ptr<T> loadResource(const std::string& path) const = 0;

private:
	std::pair<std::shared_ptr<T>, unsigned int>* findResource(const AssetNames asset) {
		auto itr = m_resources.find(asset);
		return (itr != m_resources.end() ? &itr->second : nullptr);
	}

	bool freeResource(const AssetNames asset) {
		auto itr = m_resources.find(asset);
		if (itr == m_resources.end())
			return false;

		m_resources.erase(itr);
		return true;
	}
};
