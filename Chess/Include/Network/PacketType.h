#pragma once
#include "SFML/Network/Packet.hpp"

namespace sf { class Packet; }
enum struct PacketType : unsigned char {
	Connect = 0,
	Disconnect = 1,
	RoleAssign = 2,
	NewMove = 10,
	Snapshot = 11,
	ResetBoard = 21
};

sf::Packet& operator <<(sf::Packet& packet, const PacketType& packetType);

sf::Packet& operator >>(sf::Packet& packet, PacketType& packetType);

