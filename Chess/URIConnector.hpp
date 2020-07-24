#pragma once
#include <windows.h>

#pragma warning( disable : 4267 ) 

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
		float pollInterval = .1f;
		unsigned int maxEngineTime = 1.0f;
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

		bool m_isWorking;
		float m_workTime;
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