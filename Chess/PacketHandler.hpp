#pragma once
#include <functional>
#include <unordered_map>
#include "PacketType.h"

class PacketHandler {
public:
	using PacketDelegate = std::function<void(sf::Packet& packet)>;

private:
	std::unordered_map<PacketType, PacketDelegate> m_packetHandlers;

public:
	template<typename T>
	void registerHandle(PacketType type, void(T::*func)(sf::Packet&), T* instance) {
		m_packetHandlers.emplace(type, std::bind(func, instance, std::placeholders::_1));
	}
	
	void clearHandles() {
		m_packetHandlers.clear();
	}

	void handlePacket(PacketType type, sf::Packet& packet) {
		auto itr = m_packetHandlers.find(type);
		if (itr == m_packetHandlers.end())
			return;

		itr->second(packet);
	}
};