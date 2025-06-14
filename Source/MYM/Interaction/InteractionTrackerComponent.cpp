// Fill out your copyright notice in the Description page of Project Settings.
#include "InteractionTrackerComponent.h"
#include "InteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MYM/Core/MymHUD.h"

// Sets default values for this component's properties
UInteractionTrackerComponent::UInteractionTrackerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractionTrackerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractionTrackerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (InteractablesInRange.Num() > 0)
		TraceForInteractables();
}

void UInteractionTrackerComponent::InteractBegin()
{
	if (!CurrentInteractable) return;

	CurrentInteractable->BeginInteraction(this);
}

void UInteractionTrackerComponent::InteractEnd()
{
	if (!CurrentInteractable) return;

	CurrentInteractable->EndInteraction(this);
}

void UInteractionTrackerComponent::PauseInteractionTesting()
{
	MymHUD->HideCrosshair();
	SetComponentTickEnabled(false);
}

void UInteractionTrackerComponent::ResumeInteractionTesting()
{
	if (!InteractablesInRange.IsEmpty() && CurrentInteractable)
		MymHUD->ShowInteractionWidget(CurrentInteractable->GetInteractionString());
	
	SetComponentTickEnabled(true);
}

void UInteractionTrackerComponent::InteractableDisabled(UInteractionComponent* Interactable)
{
	auto idx = InteractablesInRange.Find(Interactable);
	if (idx == INDEX_NONE)
	{
		return;
	}
	if (InteractablesInRange[idx] == CurrentInteractable)
	{
		UnsetCurrentInteractable();
	}
			
	InteractablesInRange.RemoveAtSwap(idx);
}

void UInteractionTrackerComponent::TraceForInteractables()
{
	// make sure no interactable has become disabled
	//CheckForDisabledInteractables();
	
	//Get Screen Size
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
		GEngine->GameViewport->GetViewportSize(ViewportSize);

	//Get Crosshair Location
	FVector2D CrosshairLocation(ViewportSize / 2.f);
	FVector CrosshairWorldLocation, CrosshairWorldDirection;

	//Get Screenspace location of crosshairs (top left -> bottom right)	
	if (UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldLocation,
		CrosshairWorldDirection))
	{
		FHitResult ScreenTraceResult;
		FVector End{CrosshairWorldLocation + CrosshairWorldDirection * InteractionTraceRange};
		if (GetWorld()->LineTraceSingleByChannel(ScreenTraceResult,
											 CrosshairWorldLocation,
											 End,
											 ECC_Visibility))
		{
			// Make sure the Visibility channel for at least one Component (usually Static Mesh) for the interactable is set to true in the editor
			if (UInteractionComponent* InteractionComp = ScreenTraceResult.GetActor()->FindComponentByClass<UInteractionComponent>())
			{
				if ((!CurrentInteractable || (CurrentInteractable && CurrentInteractable != InteractionComp)))
				{
					MymHUD->ShowInteractionWidget(InteractionComp->GetInteractionString());
				}
				CurrentInteractable = InteractionComp;
				return;
			}
		}
	}
	UnsetCurrentInteractable();
}

void UInteractionTrackerComponent::CheckForDisabledInteractables()
{
	// no cost going through the list bc ~1-3 interactables would be in range at max
	for (int i = InteractablesInRange.Num() - 1; i >= 0; --i)
	{
		if (!InteractablesInRange[i]->bManageRange
			|| !InteractablesInRange[i]->IsValidLowLevelFast())
		{
			if (InteractablesInRange[i] == CurrentInteractable)
			{
				UnsetCurrentInteractable();
			}
			
			InteractablesInRange.RemoveAtSwap(i);
			continue;
		}
	}
}

void UInteractionTrackerComponent::UnsetCurrentInteractable()
{
	CurrentInteractable = nullptr;
	MymHUD->HideInteractionWidget();
}

void UInteractionTrackerComponent::InteractableEnterRange_Implementation(UInteractionComponent* Interactable)
{
	bool bFound = false;
	for (int i = InteractablesInRange.Num() - 1; i >= 0; --i)
	{
		if (!InteractablesInRange[i]->IsValidLowLevelFast())
		{
			InteractablesInRange.RemoveAt(i);
			continue;
		}
		if (InteractablesInRange[i] == Interactable)
		{
			bFound = true;
			break;
		}
	}
	if (bFound) return;
	
	InteractablesInRange.Add(Interactable);

	if (InteractablesInRange.Num() >= 1)
		MymHUD->ShowCrosshair();
}

void UInteractionTrackerComponent::InteractableExitRange_Implementation(UInteractionComponent* Interactable)
{
	int idx = -1;
	for (int i = InteractablesInRange.Num() - 1; i >= 0; --i)
	{
		if (!InteractablesInRange[i]->IsValidLowLevelFast())
		{
			InteractablesInRange.RemoveAt(i);
			continue;
		}
		if (InteractablesInRange[i] == Interactable)
		{
			idx = i;
			break;
		}
	}
	if (idx < 0) return;
	
	InteractablesInRange.RemoveAtSwap(idx);

	if (InteractablesInRange.IsEmpty())
	{
		MymHUD->HideCrosshair();
	}
	if (InteractablesInRange.IsEmpty() || CurrentInteractable == Interactable)
	{
		UnsetCurrentInteractable();
	}
}