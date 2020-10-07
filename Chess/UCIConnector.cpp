#include "pch.h"
#include "UCIConnector.hpp"

namespace UCI {
	static const std::string BESTMOVE = "bestmove";
	static const std::string UCIOK = "uciok";
	static const std::string READYOK = "readyok";

	UCIConnector::UCIConnector(const std::string & path, EngineInformation & engineInfo) :
		hWritePipeIn(NULL), hReadPipeIn(NULL), hWritePipeOut(NULL), hReadPipeOut(NULL),
		m_info(engineInfo), m_isWorking(false), m_workTime(0)
	{
		char* writable = new char[path.size() + 1];
		std::copy(path.begin(), path.end(), writable);
		writable[path.size()] = '\0';

		connectToEngine(writable);

		delete[] writable;
	}

	UCIConnector::~UCIConnector() {
		stopEngine();
		closeConnection();
	}

	void UCIConnector::resetGame() {
		queueCommand("ucinewgame\n");
	}

	void UCIConnector::setSkillLevel(const unsigned int skillLevel) {
		m_skillLevel = skillLevel;
		queueCommand("setoption name Skill Level value " + std::to_string(skillLevel) + "\n");
	}

	unsigned int UCIConnector::getSkillLevel() const {
		return m_skillLevel;
	}

	EngineMessage UCI::UCIConnector::getMessage()
	{
		ASSERT(m_fromEngine.size() > 0, "No messages available!");
		EngineMessage info = m_fromEngine.top();
		m_fromEngine.pop();
		return info;
	}

	bool UCIConnector::pollEngine() {
		return m_fromEngine.size() > 0;
	}

	void UCIConnector::runEngine(float deltaTime)
	{
		m_lastUpdate += deltaTime;

		if (m_isWorking) {
			m_workTime += deltaTime;
			if (m_workTime > m_info.maxEngineTime) {
				sendCommand("stop\n");
				m_isWorking = false;
				m_workTime = 0;
			}
		}

		if (m_lastUpdate < m_info.pollInterval)
			return;

		m_lastUpdate = 0;

		//Process ToEngine messages
		//while (m_toEngine.size() > 0) {
		//	sendCommand(m_toEngine.top());
		//	m_toEngine.pop();
		//}

		while (pollEngineRead())
			readEngine();

		return;
	}

	void UCIConnector::requestMove(const std::string& FENString)
	{
		queueCommand("position fen " + FENString + '\n');
		//queueCommand("go\n");
		queueCommand("go movetime " + std::to_string(m_info.maxEngineTime * 1000) + '\n');
		m_isWorking = true;

	}

	void UCIConnector::stopEngine() {
		sendCommand("stop\n");
	}

	void UCIConnector::connectToEngine(char* path)
	{
		ASSERT(lpProcessInformation.hProcess == NULL, "Engine already running!");
		lpPipeAttributes.nLength = sizeof(lpPipeAttributes);
		lpPipeAttributes.bInheritHandle = TRUE;
		lpPipeAttributes.lpSecurityDescriptor = NULL;

		CreatePipe(&hReadPipeOut, &hWritePipeOut, &lpPipeAttributes, 0);
		CreatePipe(&hReadPipeIn, &hWritePipeIn, &lpPipeAttributes, 0);

		lpStartupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		lpStartupInfo.wShowWindow = SW_HIDE;
		lpStartupInfo.hStdInput = hReadPipeIn;
		lpStartupInfo.hStdOutput = hWritePipeOut;
		lpStartupInfo.hStdError = hWritePipeOut;

		CreateProcess(NULL, path, NULL, NULL, TRUE, 0, NULL, NULL, &lpStartupInfo, &lpProcessInformation);
		//queueCommand("uci\n");
	}

	void UCIConnector::closeConnection()
	{
		sendCommand("quit\n");
		if (hWritePipeIn != NULL) CloseHandle(hWritePipeIn);
		if (hReadPipeIn != NULL) CloseHandle(hReadPipeIn);
		if (hWritePipeOut != NULL) CloseHandle(hWritePipeOut);
		if (hReadPipeOut != NULL) CloseHandle(hReadPipeOut);
		if (lpProcessInformation.hProcess != NULL) CloseHandle(lpProcessInformation.hProcess);
		if (lpProcessInformation.hThread != NULL) CloseHandle(lpProcessInformation.hThread);
	}

	void UCI::UCIConnector::sendCommand(const std::string & command) {
		WriteFile(hWritePipeIn, command.c_str(), static_cast<DWORD>(command.length()), &lpNumberOfBytesWritten, NULL);
	}

	void UCI::UCIConnector::queueCommand(const std::string & command) {
		//m_toEngine.push(command);
		sendCommand(command);
	}

	bool UCIConnector::pollEngineRead()
	{
		if (!PeekNamedPipe(hReadPipeOut, NULL, NULL, &lpBytesRead, &lpTotalBytesAvail, NULL))
			return false;

		return lpTotalBytesAvail > 0;
	}

	void UCIConnector::readEngine()
	{
		do
		{
			int start = 0;
			if (!ReadFile(hReadPipeOut, buffer, sizeof(buffer), &lpBytesRead, NULL) || !lpBytesRead) break;
			for (unsigned int i = 1; i < lpBytesRead; i++) {
				if (buffer[i] == '\n') {
					processEngineMessage(std::string((char*)buffer + start, i - start - 1));
					start = i + 1;
				}
			}
		} while (lpBytesRead >= sizeof(buffer));
	}

	void UCIConnector::processEngineMessage(const std::string& message)
	{
		if (message.compare(0, BESTMOVE.size(), BESTMOVE) == 0) {
			int pos = static_cast<int>(message.find("none", 10, 4));
			if (pos > 0) //Engine found no more moves. Likely end game?
				return;
			m_fromEngine.emplace(EngineMessageType::BestMove, message.substr(BESTMOVE.size() + 1, 4));
			m_isWorking = false;
			return;
		}
	}
}