#include "pch.h"
#include "SoundManager.hpp"
#include "AudioManager.hpp"

SoundManager::SoundManager(AudioManager & audioManager) :
	m_lastID(0),
	m_audioManager(&audioManager),
	m_elapsed(0.f),
	m_numSounds(0)
{}

SoundManager::~SoundManager() {
	freeAllResources();
}

void SoundManager::freeAllResources()
{
	for (auto& state : m_audio) {
		for (auto& soundInfo : state.second) {
			m_audioManager->releaseResource(soundInfo.second.m_audioName);
		}
	}
	m_audio.clear();

	for (auto& recycledSound : m_recycledSounds) {
		m_audioManager->releaseResource(recycledSound.second.m_audioName);
	}
	m_recycledSounds.clear();

	m_music.clear();

	m_numSounds = 0;
	m_lastID = 0;
}

void SoundManager::switchState(States state)
{
	pauseAll(m_currentState);
	unpauseAll(state);
	m_currentState = state;
}

void SoundManager::removeState(States state)
{
	auto states = m_audio.find(state);
	if (states == m_audio.end())
		return;

	for (auto& it : states->second)
		recycleSound(it.first, it.second);

	m_audio.erase(state);
	auto it = m_music.find(state);
	if (it == m_music.end())
		return;

	m_numSounds--;
	m_music.erase(it);
}

void SoundManager::update(float deltaTime)
{
	m_elapsed += deltaTime;
	if (m_elapsed < 1.f)
		return;

	m_elapsed = 0;
	auto& soundC = m_audio[m_currentState];

	for (auto it = soundC.begin(); it != soundC.end();) {
		if (it->second.m_audio->getStatus() == sf::Sound::Status::Stopped) {
			recycleSound(it->first, it->second);
			it = soundC.erase(it);
		}
		else {
			it++;
		}
	}

	auto music = m_music.find(m_currentState);
	if (music == m_music.end())
		return;
	if (music->second.m_audio->getStatus() == sf::Sound::Status::Stopped)
		return;

	m_music.erase(m_currentState);
	--m_numSounds;
}

int SoundManager::playSound(AssetNames soundName, const sf::Vector3f & position, bool loop, bool relative)
{
	int soundID;
	auto sound = soundFactory(soundID, soundName);

	if (soundID < 0)
		return -1;

	setUpSound(*sound, SoundProperties(soundName), loop, relative);

	sound->setPosition(position);
	sound->play();
	m_audio[m_currentState].emplace(soundID, SoundInfo(soundName, sound));
	return soundID;
}

int SoundManager::playSound(const SoundProperties & soundproperties, bool loop, bool relative) {
	return playSound(soundproperties.m_soundName, sf::Vector3f(), loop, relative);
}

int SoundManager::playSound(AssetNames soundName, bool loop, bool relative) {
	return playSound(soundName, sf::Vector3f(), loop, relative);
}

bool SoundManager::playSound(int id)
{
	auto& container = m_audio[m_currentState];
	auto sound = container.find(id);
	if (sound == container.end())
		return false;

	sound->second.m_audio->play();
	sound->second.m_isManualPaused = false;
	return true;
}

bool SoundManager::stopSound(int id)
{
	auto& container = m_audio[m_currentState];
	auto sound = container.find(id);
	if (sound == container.end())
		return false;

	sound->second.m_audio->stop();
	sound->second.m_isManualPaused = true;
	return true;
}

bool SoundManager::pauseSound(int id)
{
	auto& container = m_audio[m_currentState];
	auto sound = container.find(id);
	if (sound == container.end())
		return false;

	sound->second.m_audio->pause();
	sound->second.m_isManualPaused = true;
	return true;
}

bool SoundManager::startMusic(AssetNames musicName, float volume, bool loop)
{
	auto it = m_music.find(m_currentState);
	if (it != m_music.end()) {
		if (it->second.m_audioName == musicName) {
			playMusic();
			return true;
		}
		else {
			stopMusic();
			m_music.erase(it);
		}
	}

	std::unique_ptr<sf::Music> music = std::make_unique<sf::Music>();
	const auto& path = m_audioManager->getPath(musicName);
	if (!music->openFromFile(path)) {
		throw std::runtime_error("SoundManager::loadResource - Failed to load font: " + path);
		return false;
	}
	music->setLoop(loop);
	music->setVolume(volume);
	music->setRelativeToListener(true);
	music->play();

	MusicInfo musicInfo = MusicInfo(musicName, music);

	m_music.emplace(m_currentState, std::move(musicInfo));
	m_numSounds++;
	return true;
}

bool SoundManager::playMusic()
{
	auto it = m_music.find(m_currentState);
	if (it == m_music.end())
		return false;

	it->second.m_audio->play();
	it->second.m_isManualPaused = false;
	return true;
}

bool SoundManager::stopMusic()
{
	auto it = m_music.find(m_currentState);
	if (it == m_music.end())
		return false;

	it->second.m_audio->stop();
	m_music.erase(m_currentState);

	--m_numSounds;
	return true;

}

bool SoundManager::pauseMusic()
{
	auto it = m_music.find(m_currentState);
	if (it == m_music.end())
		return false;

	it->second.m_audio->pause();
	it->second.m_isManualPaused = true;

	return true;
}

bool SoundManager::setPosition(int id, const sf::Vector3f & pos)
{
	auto& container = m_audio[m_currentState];
	auto sound = container.find(id);
	if (sound == container.end())
		return false;

	sound->second.m_audio->setPosition(pos);
	return true;
}

bool SoundManager::isPlaying(int id)
{
	auto& container = m_audio[m_currentState];
	auto sound = container.find(id);

	if (sound == container.end())
		return false;

	return sound->second.m_audio->getStatus() == sf::Sound::Status::Playing;
}

void SoundManager::pauseAll(States state)
{
	auto& container = m_audio[state];
	for (auto it = container.begin(); it != container.end();) {
		if (it->second.m_audio->getStatus() == sf::Sound::Status::Stopped) {
			recycleSound(it->first, it->second);
			it = container.erase(it);
		}
		else {
			it->second.m_audio->pause();
			++it;
		}
	}
	auto music = m_music.find(state);
	if (music == m_music.end())
		return;

	music->second.m_audio->pause();
}

void SoundManager::unpauseAll(States state)
{
	auto& container = m_audio[state];
	for (auto &it : container) {
		if (it.second.m_isManualPaused)
			continue;
		it.second.m_audio->play();
	}

	auto music = m_music.find(state);
	if (music == m_music.end())
		return;

	if (!music->second.m_isManualPaused)
		music->second.m_audio->play();
}

std::unique_ptr<sf::Sound> SoundManager::soundFactory(int& id, AssetNames soundName)
{
	std::unique_ptr<sf::Sound> sound;

	auto it = m_recycledSounds.begin();
	while (it != m_recycledSounds.end()) {
		if (it->second.m_audioName == soundName)
			break;
		else
			++it;
	}
	if (it == m_recycledSounds.end()) {
		if (m_numSounds >= MAX_SOUNDS || m_recycledSounds.size() >= SOUND_CACHE) {
			auto element = m_recycledSounds.begin();
			id = element->first;
			m_audioManager->releaseResource(element->second.m_audioName);
			m_audioManager->requireResource(soundName);

			sound = std::move(element->second.m_audio);
			sound->setBuffer(*m_audioManager->getResource(soundName));
			m_recycledSounds.erase(element);

			return sound;
		}
		else {
			if (m_audioManager->requireResource(soundName)) {
				sound = std::make_unique<sf::Sound>();
				id = m_lastID;
				++m_lastID;
				++m_numSounds;
				sound->setBuffer(*m_audioManager->getResource(soundName));

				return sound;
			}
		}
	}
	else {
		id = it->first;
		sound = std::move(it->second.m_audio);
		m_recycledSounds.erase(it);

		return sound;
	}

	std::cerr << "SoundManager::soundFactory Failed to create sound." << std::endl;
	return nullptr;
}

void SoundManager::setUpSound(sf::Sound& sound, const SoundProperties& properties, bool loop, bool relative)
{
	sound.setVolume(properties.m_volume);
	sound.setPitch(properties.m_pitch);
	sound.setMinDistance(properties.m_distance);
	sound.setAttenuation(properties.m_attenuation);
	sound.setLoop(loop);
	sound.setRelativeToListener(relative);

}

void SoundManager::recycleSound(int id, SoundInfo& soundInfo)
{
	//std::cout << "Recycling sound: " << (int)soundInfo.m_audioName << " With ID: " << id << std::endl;
	m_recycledSounds.emplace_back(id, std::move(soundInfo));
}




