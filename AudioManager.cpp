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
    if (bgmPlaying) return;

    // Close old BGM alias first, just in case
    mciSendString("close bgm", NULL, 0, NULL);

    std::string openCommand = "open \"" + filePath + "\" alias bgm";
    MCIERROR openResult = mciSendString(openCommand.c_str(), NULL, 0, NULL);

    if (openResult != 0)
    {
        char errorText[256];
        mciGetErrorString(openResult, errorText, sizeof(errorText));

        std::cout << "Failed to open background music: " << filePath << std::endl;
        std::cout << "MCI Error: " << errorText << std::endl;
        return;
    }

    MCIERROR playResult = mciSendString("play bgm repeat", NULL, 0, NULL);

    if (playResult != 0)
    {
        char errorText[256];
        mciGetErrorString(playResult, errorText, sizeof(errorText));

        std::cout << "Failed to play background music: " << filePath << std::endl;
        std::cout << "MCI Error: " << errorText << std::endl;
        return;
    }

    bgmPlaying = true;
}

void AudioManager::stopBackgroundMusic()
{
    mciSendString("stop bgm", NULL, 0, NULL);
    mciSendString("close bgm", NULL, 0, NULL);
    bgmPlaying = false;
}

void AudioManager::playSoundEffect(const std::string& filePath)
{
    BOOL success = PlaySound(
        filePath.c_str(),
        NULL,
        SND_FILENAME | SND_ASYNC
    );

    if (!success)
    {
        std::cout << "Failed to play sound effect: " << filePath << std::endl;
    }
}
