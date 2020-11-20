#pragma once
#include "SFML/Network/IpAddress.hpp"

struct NetSettings {
	static const unsigned short DEFAULT_PORT = 54820;

private:
	unsigned short Port;

public:
	std::string IpAddress;

	NetSettings() : Port(0) { }

	std::string toString() const {
		if (IpAddress.length() < 1)
			return "";

		if (Port != 0)
			return IpAddress + ":" + std::to_string(Port);
		else
			return IpAddress;
	}

	unsigned short getPort()  const {
		if (Port == 0)
			return DEFAULT_PORT;
		else
			return Port;
	}

	void setPort(unsigned short port) {
		Port = port;
	}
};