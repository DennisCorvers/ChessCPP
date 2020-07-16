#pragma once
#include <windows.h>

#pragma warning( disable : 4267 ) 

//https://github.com/official-stockfish/Stockfish
//https://gist.github.com/aliostad/f4470274f39d29b788c1b09519e67372

class URIConnector {
private:

	STARTUPINFO lpStartupInfo = { 0 };
	SECURITY_ATTRIBUTES lpPipeAttributes = { 0 };
	PROCESS_INFORMATION lpProcessInformation = { 0 };
	HANDLE hWritePipeIn;
	HANDLE hReadPipeIn;
	HANDLE hWritePipeOut;
	HANDLE hReadPipeOut;
	BYTE buffer[2048];
	DWORD lpNumberOfBytesWritten;
	DWORD lpBytesRead;
	DWORD lpTotalBytesAvail;

	unsigned int m_skillLevel;
	float m_lastUpdate;

public:
	URIConnector(const std::string& path);

	virtual ~URIConnector();

	void resetGame();

	void setSkillLevel(const unsigned int skillLevel);
	unsigned int getSkillLevel() const;

	void update(float deltaTime);

	void requestMove(const std::string& position);

	void stopEngine();

private:

	void connectToEngine(char* path);

	void closeConnection();

	inline void sendCommand(const std::string& command) {
		WriteFile(hWritePipeIn, command.c_str(), command.length(), &lpNumberOfBytesWritten, NULL);
	}

	bool pollEngine();

	std::string readEngine();
};

#pragma warning( default : 4267 )