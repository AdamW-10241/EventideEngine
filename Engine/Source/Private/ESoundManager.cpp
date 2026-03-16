#include "ESoundManager.h"
#include "EngineTypes.h"

ESoundManager::ESoundManager() {
	// Initialise sound manager
	Mix_AllocateChannels(32);
	m_sounds.assign(EE_SOUND_NUM, nullptr);
	m_music.assign(EE_MUSIC_NUM, nullptr);

	// Setup Sounds
	LoadMusic("Sounds/MUSIC_PLAY.wav", EE_MUSIC_MUSIC);
	LoadSound("Sounds/SFX_HIT.wav", EE_SOUND_HIT);
	LoadSound("Sounds/SFX_COIN.wav", EE_SOUND_COIN);
	LoadSound("Sounds/DEATH_SFX_PLAYER.wav", EE_SOUND_HIT_PLAYER);

	PlayMusic(EE_MUSIC_MUSIC, 50);
}

void ESoundManager::Shutdown()
{
	// Cleanup audio
	Mix_HaltChannel(-1);

	for (auto sound : m_sounds) {
		if (sound) Mix_FreeChunk(sound);
	}

	for (auto music : m_music) {
		if (music) Mix_FreeMusic(music);
	}

	Mix_CloseAudio();
}

void ESoundManager::PlaySound(EESoundType soundType, int volume)
{
	if (soundType == EE_SOUND_NONE) return;

	// Play sound
	if (auto sound = m_sounds.at(soundType)) {
		if (Mix_PlayChannel(-1, sound, 0) == -1) {
			EDebug::Log("Sound could not be played: " + toEString(soundType));
		}
		else Mix_VolumeChunk(sound, volume);
	}
	else EDebug::Log("Sound is not defined - EESoundType::" + toEString(soundType));
}

void ESoundManager::PlayMusic(EEMusicType musicType, int volume)
{
	if (musicType == EE_MUSIC_NONE) return;

	// Play music
	if (auto music = m_music.at(musicType)) {
		if (Mix_PlayMusic(music, -1) == -1) {
			EDebug::Log("Music could not be played: " + toEString(musicType));
		}
		else Mix_VolumeMusic(volume);
	}
	else EDebug::Log("Music is not defined - EEMusicType::" + toEString(musicType));
}

Mix_Chunk* ESoundManager::LoadSound(EString path, EESoundType soundType)
{
	Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());

	if (chunk) {
		m_sounds.at(soundType) = chunk;
	}
	else {
		EDebug::Log("Failed to load sound: " + path);
		EDebug::Log("Error: " + EString(Mix_GetError()), LT_ERROR);
	}

	return chunk;
}

Mix_Music* ESoundManager::LoadMusic(EString path, EEMusicType musicType)
{
	Mix_Music* music = Mix_LoadMUS(path.c_str());
	
	if (music) {
		m_music.at(musicType) = music;
	}
	else {
		EDebug::Log("Failed to load music: " + path);
		EDebug::Log("Error: " + EString(Mix_GetError()), LT_ERROR);
	}

	return music;
}

void ESoundManager::PrimeSound(EESoundType soundType, int volume)
{
	if (auto sound = m_sounds.at(soundType)) {
		Mix_VolumeChunk(sound, 0);
		int channel = Mix_PlayChannel(-1, sound, 0);
		if (channel != -1) Mix_HaltChannel(channel);
		Mix_VolumeChunk(sound, volume);
	}
	else EDebug::Log("Could not prime sound: EESoundType::" + toEString(soundType), LT_ERROR);
}
