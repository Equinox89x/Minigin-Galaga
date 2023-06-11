#include "AudioComponent.h"

void dae::AudioComponent::PostInitialize()
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

void dae::AudioComponent::Update()
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

void dae::AudioComponent::PlayPlayerDeathSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(PlayerDeathSoundId);
}

void dae::AudioComponent::PlayDeathSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(DeathSoundId);
}

void dae::AudioComponent::PlayShootSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(ShootSoundId);
}

void dae::AudioComponent::PlayDiveSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(DiveSoundId);
}

void dae::AudioComponent::PlayTractorBeamSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(BeamSoundId);
}

void dae::AudioComponent::PlayCapturedTractorBeamSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(BeamCapturedSoundId);
}

void dae::AudioComponent::PlayMenuSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->PlaySound(MenuSoundId);
	m_pAudioService->SetEffectVolume(60);
}

void dae::AudioComponent::StopSound()
{
	m_pAudioService->StopSound();
}
