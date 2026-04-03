#include "ESoundManager.h"
#include "EngineTypes.h"

ESoundManager::ESoundManager() {
	// Initialise sound manager
	Mix_AllocateChannels(32);
	m_sounds.assign(EE_SOUND_NUM, nullptr);
	m_music.assign(EE_MUSIC_NUM, nullptr);

	// Setup Sounds
	LoadMusic(EE_MUSIC_MUSIC,		"Sounds/MUSIC_PLAY.wav");
	LoadSound(EE_SOUND_HIT,			"Sounds/SFX_HIT.wav");
	LoadSound(EE_SOUND_COIN,		"Sounds/SFX_COIN.wav");
	LoadSound(EE_SOUND_HIT_PLAYER,	"Sounds/DEATH_SFX_PLAYER.wav");
	LoadSound(EE_SOUND_SHOOT,		"Sounds/SFX_SHOOT.wav");

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
		Mix_VolumeChunk(sound, volume);
		if (Mix_PlayChannel(-1, sound, 0) == -1) {
			EDebug::Log(LT_WARNING, "Sound could not be played: " + toEString(soundType));
		}
	}
	else EDebug::Log(LT_WARNING, "Sound is not defined - EESoundType::" + toEString(soundType));
}

void ESoundManager::PlayMusic(EEMusicType musicType, int volume)
{
	if (musicType == EE_MUSIC_NONE) return;

	// Play music
	if (auto music = m_music.at(musicType)) {
		Mix_VolumeMusic(volume);
		if (Mix_PlayMusic(music, -1) == -1) {
			EDebug::Log(LT_WARNING, "Music could not be played: " + toEString(musicType));
		}
	}
	else EDebug::Log(LT_WARNING, "Music is not defined - EEMusicType::" + toEString(musicType));
}

Mix_Chunk* ESoundManager::LoadSound(EESoundType soundType, EString path)
{
	Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());

	if (chunk) {
		m_sounds.at(soundType) = chunk;
	}
	else EDebug::Log(LT_WARNING, "Failed to load sound: " + path + " - Error: " + EString(Mix_GetError()));

	return chunk;
}

Mix_Music* ESoundManager::LoadMusic(EEMusicType musicType, EString path)
{
	Mix_Music* music = Mix_LoadMUS(path.c_str());
	
	if (music) {
		m_music.at(musicType) = music;
	}
	else EDebug::Log(LT_WARNING, "Failed to load music: " + path + " - Error: " + EString(Mix_GetError()));

	return music;
}