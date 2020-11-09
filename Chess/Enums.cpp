#include "pch.h"

sf::Packet& operator<<(sf::Packet& packet, const ActionType& actionType)
{
	return packet << sf::Uint16(actionType);
}

sf::Packet& operator>>(sf::Packet& packet, ActionType& actionType)
{
	sf::Uint16 result = 0;
	packet >> result;
	actionType = static_cast<ActionType>(result);
	return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const PacketType& packetType)
{
	return packet << sf::Uint8(packetType);
}

sf::Packet& operator>>(sf::Packet& packet, PacketType& packetType)
{
	sf::Uint8 result = 0;
	packet >> result;
	packetType = static_cast<PacketType>(result);
	return packet;
}
