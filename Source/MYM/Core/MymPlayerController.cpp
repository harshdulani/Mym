// Fill out your copyright notice in the Description page of Project Settings.
#include "MymPlayerController.h"
#include "MymHUD.h"
#include "MymCharacter.h"
#include "MYM/Interaction/InteractionTrackerComponent.h"

void AMymPlayerController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);
	
	MymHUD = Cast<AMymHUD>(GetHUD());
	MymCharacter = Cast<AMymCharacter>(P);
	if (MymCharacter.IsValid())
	{
		MymCharacter->MymPC = this;
		UInteractionTrackerComponent* InteractionTracker = MymCharacter->GetInteractionTracker();

		InteractionTracker->SetHUDReference(MymHUD);
		InteractionTracker->SetCharacterReference(MymCharacter.Get());
		InteractionTracker->SetActive(true);
	}
}
