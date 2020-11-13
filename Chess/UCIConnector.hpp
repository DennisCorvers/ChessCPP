#pragma once
#include <windows.h>
#include "Poller.hpp"
#pragma warning( disable : 4267 ) 

namespace UCI {

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

	class UCIConnector {
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
		EngineInformation m_info;
		Poller m_poller;

	public:
		void setSkillLevel(const unsigned int skillLevel);
		unsigned int getSkillLevel() const;

		UCIConnector(const std::string& path, EngineInformation& engineInfo);

		virtual ~UCIConnector();

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