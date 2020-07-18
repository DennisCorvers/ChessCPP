#pragma once
#include <windows.h>

#pragma warning( disable : 4267 ) 

//https://github.com/official-stockfish/Stockfish
//https://gist.github.com/aliostad/f4470274f39d29b788c1b09519e67372
//https://www.chessprogramming.org/Forsyth-Edwards_Notation#:~:text=Forsyth%2DEdwards%20Notation%20(FEN),of%20the%20Portable%20Game%20Notation%20.

namespace URI {

	enum struct EngineMessageType {
		UCIOK,
		ReadyOK,
		BestMove
	};

	struct EngineMessage {
		EngineMessage() = delete;
		EngineMessage(EngineMessageType messageType, const std::string& message) :
			messageType(messageType),
			message(message) {}
		EngineMessageType messageType;
		std::string message;
	};

	struct EngineInformation {
		float pollIntervalSec = 0.1f;
		unsigned int maxEngineTimeMs = 500;
	};

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

		bool m_engineReady;
		std::stack<std::string> m_toEngine;
		std::stack <EngineMessage> m_fromEngine;

		unsigned int m_skillLevel;
		float m_lastUpdate;
		EngineInformation m_info;

	public:
		void setSkillLevel(const unsigned int skillLevel);
		unsigned int getSkillLevel() const;

		URIConnector(const std::string& path, EngineInformation& engineInfo);

		virtual ~URIConnector();

		void resetGame();

		void requestMove(const std::string& FENString);

		EngineMessage getMessage();
		bool pollEngine();
		void runEngine(float deltaTime);
		void stopEngine();

	private:

		void connectToEngine(char* path);

		void sendCommand(const std::string& command);
		void queueCommand(const std::string& command);

		bool pollEngineRead();
		void readEngine();
		void processEngineMessage(const std::string& message);

		void closeConnection();
	};
}

#pragma warning( default : 4267 )