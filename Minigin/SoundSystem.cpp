#include "SoundSystem.h"

#include <thread>

SoundSystem& ServiceLocator::GetSoundSystem()
{
	return *ssInstance;
}
void ServiceLocator::RegisterSoundSystem(std::shared_ptr<SoundSystem> ss)
{
	ssInstance = ss == nullptr ? defaultSS : ss;
	ss->StartSoundThread();
}
std::shared_ptr<NullSoundSystem> ServiceLocator::defaultSS;
std::shared_ptr<SoundSystem> ServiceLocator::ssInstance = defaultSS;

int SDLSoundSystem::AddAudioClip(std::string path, float volume, int loops)
{
	m_AudioClips.push_back(AudioClip(path, volume, loops));
	return static_cast<int>(m_AudioClips.size()) - 1;
}

SDLSoundSystem::~SDLSoundSystem()
{
	m_ShouldUpdate = false;
	m_SoundThread.join();
	Mix_CloseAudio();
}
void SDLSoundSystem::Update()
{
	while (m_ShouldUpdate)
	{
		for (int i = 0; i < m_NrPending; i++)
		{
			m_AudioClips[m_Pending[i]].Play();
		}
		m_NrPending = 0;
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

void SDLSoundSystem::HandleDeath()
{
}

void SDLSoundSystem::Play(const soundId id, const float volume)
{
	if (id >= m_AudioClips.size()) return;
	m_AudioClips[id].SetVolume(volume);

	m_Pending[m_NrPending] = id;
	++m_NrPending;
}