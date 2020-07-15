#include "pch.h"
#include "URIConnector.hpp"

URIConnector::URIConnector(const std::string & path) :
	hWritePipeIn(NULL), hReadPipeIn(NULL), hWritePipeOut(NULL), hReadPipeOut(NULL)
{
	char* writable = new char[path.size() + 1];
	std::copy(path.begin(), path.end(), writable);
	writable[path.size()] = '\0';

	connectToEngine(writable);

	delete[] writable;
}

URIConnector::~URIConnector() {
	closeConnection();
}

void URIConnector::resetGame() {
	sendCommand("ucinewgame\n");
}

void URIConnector::setSkillLevel(const unsigned int skillLevel) {
	m_skillLevel = skillLevel;
	sendCommand("setoption name Skill Level value " + std::to_string(skillLevel) + "\n");
}

unsigned int URIConnector::getSkillLevel() const {
	return m_skillLevel;
}

void URIConnector::update(float deltaTime)
{
	m_lastUpdate += deltaTime;
	//Only run once every 100ms.
	if (m_lastUpdate < 0.1f)
		return;

	m_lastUpdate = 0;
	if (!pollEngine())
		return;


}

void URIConnector::stopEngine() {
	sendCommand("stop\n");
}

void URIConnector::connectToEngine(char* path)
{
	lpPipeAttributes.nLength = sizeof(lpPipeAttributes);
	lpPipeAttributes.bInheritHandle = TRUE;
	lpPipeAttributes.lpSecurityDescriptor = NULL;

	CreatePipe(&hReadPipeOut, &hWritePipeOut, &lpPipeAttributes, 0);
	CreatePipe(&hReadPipeIn, &hWritePipeOut, &lpPipeAttributes, 0);

	lpStartupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	lpStartupInfo.wShowWindow = SW_HIDE;
	lpStartupInfo.hStdInput = hReadPipeIn;
	lpStartupInfo.hStdOutput = hWritePipeOut;
	lpStartupInfo.hStdError = hWritePipeOut;

	CreateProcess(NULL, path, NULL, NULL, TRUE, 0, NULL, NULL, &lpStartupInfo, &lpProcessInformation);
	sendCommand("uci\n");
}

void URIConnector::closeConnection()
{
	sendCommand("quit\n");
	if (hWritePipeOut != NULL) CloseHandle(hWritePipeOut);
	if (hReadPipeIn != NULL) CloseHandle(hReadPipeIn);
	if (hWritePipeOut != NULL) CloseHandle(hWritePipeOut);
	if (hReadPipeOut != NULL) CloseHandle(hReadPipeOut);
	if (lpProcessInformation.hProcess != NULL) CloseHandle(lpProcessInformation.hProcess);
	if (lpProcessInformation.hThread != NULL) CloseHandle(lpProcessInformation.hThread);
}

