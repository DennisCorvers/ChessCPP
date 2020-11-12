#pragma once
#include "SFML/Network/IpAddress.hpp"

struct NetSettings {
	sf::IpAddress IpAddress;
	unsigned short Port;

	NetSettings() : IpAddress(0, 0, 0, 0), Port(0) { }
};