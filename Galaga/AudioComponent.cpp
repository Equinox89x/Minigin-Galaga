#include "AudioComponent.h"

void AudioComponent::PostInitialize()
{
	m_pAudioService = Locator::getAudio();
	if (m_pAudioService == nullptr) return;

	BeamSoundId = m_pAudioService->LoadSound("../Data/Sound/TractorBeam.wav");
	ShootSoundId = m_pAudioService->LoadSound("../Data/Sound/ShootSound.wav");
	BeamCapturedSoundId = m_pAudioService->LoadSound("../Data/Sound/TractorBeamCaptured.wav");
	DeathSoundId = m_pAudioService->LoadSound("../Data/Sound/EnemyDeathSound.wav");
	DiveSoundId = m_pAudioService->LoadSound("../Data/Sound/DiveSound.wav");
	PlayerDeathSoundId = m_pAudioService->LoadSound("../Data/Sound/PlayerDeathSound.wav");
	MenuSoundId = m_pAudioService->LoadSound("../Data/Sound/MenuMusic.wav");
}

void AudioComponent::Update()
{
	if (m_pAudioService == nullptr) {
		m_pAudioService = Locator::getAudio();
		if (m_pAudioService == nullptr) return;

		BeamSoundId = m_pAudioService->LoadSound("../Data/Sound/TractorBeam.wav");
		ShootSoundId = m_pAudioService->LoadSound("../Data/Sound/ShootSound.wav");
		BeamCapturedSoundId = m_pAudioService->LoadSound("../Data/Sound/TractorBeamCaptured.wav");
		DeathSoundId = m_pAudioService->LoadSound("../Data/Sound/EnemyDeathSound.wav");
		DiveSoundId = m_pAudioService->LoadSound("../Data/Sound/DiveSound.wav");
		PlayerDeathSoundId = m_pAudioService->LoadSound("../Data/Sound/PlayerDeathSound.wav");
		MenuSoundId = m_pAudioService->LoadSound("../Data/Sound/MenuMusic.wav");
		PlayMenuSound();
	}
}

void AudioComponent::PlayPlayerDeathSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(PlayerDeathSoundId);
}

void AudioComponent::PlayDeathSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(DeathSoundId);
}

void AudioComponent::PlayShootSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(ShootSoundId);
}

void AudioComponent::PlayDiveSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(DiveSoundId);
}

void AudioComponent::PlayTractorBeamSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(BeamSoundId);
}

void AudioComponent::PlayCapturedTractorBeamSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(BeamCapturedSoundId);
}

void AudioComponent::PlayMenuSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->PlaySound(MenuSoundId);
	m_pAudioService->SetEffectVolume(60);
}

void AudioComponent::StopSound()
{
	m_pAudioService->StopSound();
}
