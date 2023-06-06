#include "AudioClip.h"
#include <filesystem>
#include <iostream>

AudioClip::AudioClip(std::string path, float volume, int loops)
	: m_Path{ path }
	, m_Volume{ volume }
	, m_Loops{ loops }
{
}

void AudioClip::Play()
{
	if (!m_IsLoaded)
	{
		auto temp{ Mix_LoadWAV(m_Path.c_str()) };
		if (!temp) return;
		std::unique_lock<std::mutex> lock(m_Mutex);

		m_File = temp;

		lock.unlock();
		if (!m_File)
		{
			std::cout << "Could not load: " << std::filesystem::current_path() << " /" << m_Path << std::endl;
			return;
		}

		//std::cout << "Loaded: " << m_Path << std::endl;

		lock.lock();
		m_IsLoaded = true;
		lock.unlock();
	}

	Mix_Volume(-1, static_cast<int>(m_Volume));
	Mix_PlayChannel(-1, m_File, m_Loops);
	//std::cout << "Playing: " << m_Path << std::endl;
}

void AudioClip::SetVolume(const float vol)
{
	m_Volume = vol;
}