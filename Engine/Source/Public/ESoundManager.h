#pragma once
#include "EngineTypes.h"
#include "SDL/SDL_mixer.h"

enum EESoundType {
    EE_SOUND_NONE = 0,
    EE_SOUND_HIT,
    EE_SOUND_HIT_PLAYER,
    EE_SOUND_COIN,
    EE_SOUND_SHOOT,
    // Sound Type Number
    EE_SOUND_NUM
};

enum EEMusicType {
    EE_MUSIC_NONE = 0,
    EE_MUSIC_MUSIC,
    // Music Type Number
    EE_MUSIC_NUM
};

class ESoundManager {
public:
    ESoundManager();

    void PlaySound(EESoundType soundType, int volume = 50);

    void PlayMusic(EEMusicType musicType, int volume = 50);

    void Shutdown();

private:
    Mix_Chunk* LoadSound(EESoundType soundType, EString path);

    Mix_Music* LoadMusic(EEMusicType musicType, EString path);

private:
    // Sounds
    TArray<Mix_Chunk*> m_sounds;

    TArray<Mix_Music*> m_music;
};