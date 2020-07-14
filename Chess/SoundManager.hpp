#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include "States.h"
#include "assetNames.h"

struct SoundProperties {
	SoundProperties(AssetNames soundName) :
		m_soundName(soundName),
		m_volume(100.f),
		m_pitch(1.f),
		m_distance(100.f),
		m_attenuation(0.f)
	{}

	AssetNames m_soundName;
	float m_volume;
	float m_pitch;
	float m_distance;
	float m_attenuation;
};

class AudioManager;
class SoundManager {
private:
	template<typename T>
	struct AudioInfo {
		AudioInfo() {}
		AudioInfo(AssetNames audioName, std::unique_ptr<T>& audio) :
			m_audioName(audioName),
			m_audio(std::move(audio)),
			m_isManualPaused(false)
		{}
		AudioInfo(const T& other) = delete;
		AudioInfo(T&& other) :
			m_audioName(other.m_audioName),
			m_isManualPaused(other.m_isManualPaused),
			m_audio(std::move(other.m_audio))
		{}

		std::unique_ptr<T> m_audio;
		AssetNames m_audioName;
		bool m_isManualPaused;
	};

public:
	static const int MAX_SOUNDS = 50;
	static const int SOUND_CACHE = 25;

private:
	template<typename K, typename V>
	using map = std::unordered_map<K, V>;
	using SoundInfo = AudioInfo<sf::Sound>;
	using MusicInfo = AudioInfo<sf::Music>;


	map<States, map<int, SoundInfo>> m_audio;
	map<States, MusicInfo> m_music;
	std::vector<std::pair<int, SoundInfo>> m_recycledSounds;
	States m_currentState;

	int m_lastID;
	int m_numSounds;
	float m_elapsed;

	AudioManager* m_audioManager;

public:
	SoundManager(AudioManager& audioManager);
	~SoundManager();

	void switchState(States state);
	void removeState(States state);

	void update(float deltaTime);

	int playSound(AssetNames soundName, const sf::Vector3f& position, bool loop = false, bool relative = false);
	int playSound(const SoundProperties & soundproperties, bool loop = false, bool relative = false);
	int playSound(AssetNames soundName, bool loop = false, bool relative = false);
	bool playSound(int id);
	bool stopSound(int id);
	bool pauseSound(int id);

	bool startMusic(AssetNames musicName, float volume = 100.f, bool loop = false);
	bool playMusic();
	bool stopMusic();
	bool pauseMusic();

	bool setPosition(int id, const sf::Vector3f& pos);
	bool isPlaying(int id);

private:
	void pauseAll(States state);
	void unpauseAll(States state);

	std::unique_ptr<sf::Sound> soundFactory(int& id, AssetNames soundName);
	void setUpSound(sf::Sound& sound, const SoundProperties& properties,
		bool loop = false, bool relative = false);
	void recycleSound(int id, SoundInfo& soundInfo);
	void freeAllResources();
};
