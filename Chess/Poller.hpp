#pragma once
struct Poller final {
private:
	float m_pollInterval;
	float m_lastPoll;

public:
	Poller(float interval) : m_pollInterval(interval) { }

	bool poll(float time) {
		if (m_lastPoll >= m_pollInterval) { m_lastPoll = 0; return true; }
		else { m_lastPoll += time; return false; }
	}
};