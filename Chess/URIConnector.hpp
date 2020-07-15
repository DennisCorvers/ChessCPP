#pragma once
#include <windows.h>

#pragma warning( disable : 4267 ) 

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

	void stopEngine();

	bool pollEngine() {
		if (!PeekNamedPipe(hReadPipeOut, NULL, NULL, &lpBytesRead, &lpTotalBytesAvail, NULL))
			return false;

		return lpBytesRead > 0;
	}

	void readEngine() {
		std::string output;
		do
		{
			ZeroMemory(buffer, sizeof(buffer));
			if (!ReadFile(hReadPipeOut, buffer, sizeof(buffer), &lpBytesRead, NULL) || !lpBytesRead) break;
			buffer[lpBytesRead] = 0;
			output += (char*)buffer;
		} while (lpBytesRead >= sizeof(buffer));

		std::cout << output << std::endl;
	}

private:

	void connectToEngine(char* path);

	inline void sendCommand(const std::string& command) {
		WriteFile(hWritePipeOut, command.c_str(), command.length(), &lpNumberOfBytesWritten, NULL);
	}

	void closeConnection();

	std::string getNextMove(std::string position)
	{
		std::string str;
		position = "position startpos moves " + position + "\ngo\n";

		WriteFile(hWritePipeOut, position.c_str(), position.length(), &lpNumberOfBytesWritten, NULL);
		Sleep(500);

		PeekNamedPipe(hReadPipeOut, buffer, sizeof(buffer), &lpBytesRead, &lpTotalBytesAvail, NULL);
		do
		{
			ZeroMemory(buffer, sizeof(buffer));
			if (!ReadFile(hReadPipeOut, buffer, sizeof(buffer), &lpBytesRead, NULL) || !lpBytesRead) break;
			buffer[lpBytesRead] = 0;
			str += (char*)buffer;
		} while (lpBytesRead >= sizeof(buffer));

		int n = str.find("bestmove");
		if (n != -1) return str.substr(n + 9, 4);

		return "error";
	}
};

#pragma warning( default : 4267 )