#pragma once

struct ClientInfo {
private:
	friend class NetServer;
	int m_playerID;

public:
	ClientInfo(int clientID) :
		m_playerID(clientID)
	{ }

	int getClientID() const {
		return m_playerID;
	}
};
