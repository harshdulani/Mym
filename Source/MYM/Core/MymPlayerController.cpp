// Fill out your copyright notice in the Description page of Project Settings.


#include "MymPlayerController.h"

#include "MymHUD.h"
#include "MymCharacter.h"
#include "MYM/Interaction/InteractionTrackerComponent.h"

void AMymPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	MymHUD = Cast<AMymHUD>(GetHUD());
	MymCharacter = Cast<AMymCharacter>(InPawn);
	if (MymCharacter.IsValid())
	{
		MymCharacter->MymPC = this;
		MymCharacter->GetInteractionTracker()->SetHUDReference(MymHUD);
		MymCharacter->GetInteractionTracker()->SetCharacterReference(MymCharacter.Get());
	}
}
