#pragma once

struct ClientInfo {
private:
	friend class NetServer;
	int m_clientID;

public:
	ClientInfo(int clientID) :
		m_clientID(clientID)
	{ }

	int getClientID() const {
		return m_clientID;
	}
};
