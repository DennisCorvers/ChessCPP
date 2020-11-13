#pragma once
#include "SFML/Network/Packet.hpp"

enum struct PacketType : unsigned char {
	Connect = 0,
	Disconnect = 1,
	NewMove = 10,
	Snapshot = 11,
	ResetBoard = 21,
	SwapColour = 22
};

sf::Packet& operator <<(sf::Packet& packet, const PacketType& packetType) {
	return packet << sf::Uint8(packetType);
}

sf::Packet& operator >>(sf::Packet& packet, PacketType& packetType) {
	sf::Uint8 pakid(0);
	packet >> pakid;
	packetType = static_cast<PacketType>(pakid);
	return packet;
}

