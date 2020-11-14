#include "pch.h"
#include "PacketType.h"
#include "SFML/Network/Packet.hpp"

sf::Packet & operator<<(sf::Packet & packet, const PacketType & packetType)
{
	return packet << sf::Uint8(packetType);
}

sf::Packet & operator>>(sf::Packet & packet, PacketType & packetType)
{
	sf::Uint8 pakid(0);
	packet >> pakid;
	packetType = static_cast<PacketType>(pakid);
	return packet;
}
