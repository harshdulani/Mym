// Fill out your copyright notice in the Description page of Project Settings.
#include "MymPlayerController.h"
#include "MymHUD.h"
#include "MymCharacter.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MYM/Interaction/InteractionTrackerComponent.h"
#include "MYM/Order/WidgetHostingActor.h"

void AMymPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// servers have the refs ready by OnPossess, but dont call AcknowledgePossession
	if (InPawn && GetNetMode() == NM_ListenServer)
	{
		InitializeRefs(Cast<AMymCharacter>(InPawn));
	}
}

void AMymPlayerController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);

	if (P && (GetNetMode() == NM_Standalone || GetNetMode() == NM_Client))
	{
		InitializeRefs(Cast<AMymCharacter>(P));
	}
}

void AMymPlayerController::InitializeRefs(AMymCharacter* NewMymCharacter)
{
	MymHUD = Cast<AMymHUD>(GetHUD());
	MymCharacter = Cast<AMymCharacter>(NewMymCharacter);
	if (MymCharacter.IsValid())
	{
		MymCharacter->MymPC = this;
		UInteractionTrackerComponent* InteractionTracker = MymCharacter->GetInteractionTracker();

		InteractionTracker->SetHUDReference(MymHUD);
		InteractionTracker->SetCharacterReference(MymCharacter.Get());
		InteractionTracker->SetActive(true);
	}
	
	TArray<AActor*> WidgetHostingActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWidgetHostingActor::StaticClass(), WidgetHostingActors);
	auto LocalPlayer = GetLocalPlayer();
	for (AActor* Actor : WidgetHostingActors)
	{
		if (AWidgetHostingActor* WidgetHost = Cast<AWidgetHostingActor>(Actor))
		{
			WidgetHost->SetOwner(this);
			WidgetHost->GetWidgetComponent()->SetOwnerPlayer(LocalPlayer);
			if (MymCharacter.IsValid())
			{
				WidgetHost->InitializeWidgetHostingActor(MymCharacter->GetShopper(), this);
			}
		}
	}
}
