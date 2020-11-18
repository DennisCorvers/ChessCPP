#pragma once
#include "SFML/Network/IpAddress.hpp"

struct NetSettings {
	static const unsigned short DEFAULT_PORT = 1001;

	sf::IpAddress IpAddress;
	unsigned short Port;

	NetSettings() : IpAddress(0, 0, 0, 0), Port(DEFAULT_PORT) { }

	std::string toString() const {
		if (Port != DEFAULT_PORT)
			return IpAddress.toString() + ":" + std::to_string(Port);
		else
			return IpAddress.toString();
	}
};