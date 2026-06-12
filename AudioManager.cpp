#include "AudioManager.hpp"

#include <windows.h>
#include <mmsystem.h>
#include <iostream>

AudioManager::AudioManager()
{
    bgmPlaying = false;
}

void AudioManager::playBackgroundMusic(const std::string& filePath)
{
    if (bgmPlaying)
        return;

    BOOL success = PlaySound(
        filePath.c_str(),
        NULL,
        SND_FILENAME | SND_ASYNC | SND_LOOP
    );

    if (!success)
    {
        std::cout << "Failed to play background music: "
                  << filePath << std::endl;
        return;
    }

    bgmPlaying = true;
}

void AudioManager::stopBackgroundMusic()
{
    PlaySound(NULL, NULL, 0);
    bgmPlaying = false;
}
