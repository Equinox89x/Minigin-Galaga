#pragma once

#include "resource.h"
#include <Scene.h>
#include <GameObject.h>

void CreateEndScreen(dae::Scene* scene);
void CreateScore(dae::Scene* scene);
void MakeGalaga(dae::Scene* scene, std::string textureName, int id);
void MakeMainGalaga(dae::Scene* scene);
void MakeSecondGalaga(dae::Scene* scene);
void MakeStageOfNr(dae::Scene* scene, Stages stageName, float delayTimer);
void MakeStage(dae::Scene* scene);
void MakeVersusStage(dae::Scene* scene);
void MakeMainMenu(dae::Scene* scene);
