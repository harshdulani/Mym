// Fill out your copyright notice in the Description page of Project Settings.
#include "InteractionTrackerComponent.h"
#include "GrabInteractionComponent.h"
#include "InteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MYM/Core/MymHUD.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInteractionTrackerComponent::UInteractionTrackerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Set inactive right now, PlayerController sets it active on possession,
	// as we dont need to track interations on remotely controlled characters
	UActorComponent::SetActive(false);
}

void UInteractionTrackerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInteractionTrackerComponent, CurrentGrabbable);
	DOREPLIFETIME(UInteractionTrackerComponent, bInteractionHeld);
	DOREPLIFETIME(UInteractionTrackerComponent, CurrentInteractable);
}

// Called every frame
void UInteractionTrackerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// let the interaction subclasses define what their tick is. had i been making an OnInteractionHeld event, I wouldve used it. but right now adding a delegate broadcast on tick() seems unnecessary
	if (bInteractionHeld) return;
	
	if (InteractablesInRange.Num() > 0)
		TraceForInteractables();
}

void UInteractionTrackerComponent::InteractBegin()
{
	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("InteractBegin: CurrentInteractable %s"), (CurrentInteractable ? *CurrentInteractable->GetName() : TEXT("None"))));
	if (!CurrentInteractable) return;

	CurrentInteractable->BeginInteraction(this);
	bInteractionHeld = true;
}

void UInteractionTrackerComponent::InteractEnd()
{	
	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("InteractEnd: CurrentInteractable %s"), (CurrentInteractable ? *CurrentInteractable->GetName() : TEXT("None"))));
	bInteractionHeld = false;
	// @todo: if current grabbable is bad code, we can do better
	if (CurrentInteractable)
	{
		CurrentInteractable->EndInteraction(this);
	}
	else if (CurrentGrabbable)
	{
		CurrentGrabbable->EndInteraction(this);
	}
}

void UInteractionTrackerComponent::PauseInteractionTesting()
{
	if (MymHUD)
		MymHUD->HideCrosshair();
	SetComponentTickEnabled(false);
}

void UInteractionTrackerComponent::ResumeInteractionTesting()
{
	if (!InteractablesInRange.IsEmpty() && MymHUD && CurrentInteractable)
	{
		MymHUD->ShowInteractionWidget(CurrentInteractable->GetInteractionString());
	}
	
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
		//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("DisabledInform: UnsetCurrentInteractable from %s"), (CurrentInteractable ? *CurrentInteractable->GetName() : TEXT("None"))));
		UnsetCurrentInteractable();
	}
			
	InteractablesInRange.RemoveAtSwap(idx);
}

void UInteractionTrackerComponent::SetGrabbable(UGrabInteractionComponent* Grabbable)
{
	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Set Grabbable: %s"), (Grabbable ? *Grabbable->GetName() : TEXT("None"))));
	CurrentGrabbable = Grabbable;
	UnsetCurrentInteractable();
}

void UInteractionTrackerComponent::TraceForInteractables()
{
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
			TArray<UInteractionComponent*> InteractablesInActor;
			ScreenTraceResult.GetActor()->GetComponents<UInteractionComponent>(InteractablesInActor);
			
			// If actor has only one InteractionComponent, just go ahead with that instance
			if (InteractablesInActor.Num() == 1)
			{
				if (UInteractionComponent* InteractionComp = InteractablesInActor[0])
				{
					if ((!CurrentInteractable || (CurrentInteractable && CurrentInteractable != InteractionComp)) && MymHUD)
					{
						MymHUD->ShowInteractionWidget(InteractionComp->GetInteractionString());
					}
					CurrentInteractable = InteractionComp;
					return;
				}
			}
			else if (InteractablesInActor.Num() > 1) // If actor has multiple InteractionComponents, check for which one owns this mesh
			{
				for (const auto& Interactable : InteractablesInActor)
				{
					if (Interactable->OwnsPrimitive(ScreenTraceResult.Component.Get()))
					{
						if ((!CurrentInteractable || (CurrentInteractable && CurrentInteractable != Interactable)) && MymHUD)
						{
							MymHUD->ShowInteractionWidget(Interactable->GetInteractionString());
						}
						CurrentInteractable = Interactable;
						return;
					}
				}
			}
		}
	}
	FString InteractionString = TEXT("None");
	if (CurrentInteractable)
		InteractionString = CurrentInteractable->GetName();
	
	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("No tracehit: UnsetCurrentInteractable from %s %i"), *InteractionString, bInteractionHeld));
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
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("DisabledCheck: UnsetCurrentInteractable from %s"), (CurrentInteractable ? *CurrentInteractable->GetName() : TEXT("None"))));
				if (CurrentGrabbable == CurrentInteractable)
				{
					SetGrabbable(nullptr);
				}
				else
				{
					UnsetCurrentInteractable();
				}
			}
			
			InteractablesInRange.RemoveAtSwap(i);
		}
	}
}

void UInteractionTrackerComponent::UnsetCurrentInteractable()
{	
	CurrentInteractable = nullptr;
	if (MymHUD)
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
	{
		if (MymHUD)
			MymHUD->ShowCrosshair();
	}
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
		if (MymHUD)
			MymHUD->HideCrosshair();
	}
	if ((InteractablesInRange.IsEmpty() || CurrentInteractable == Interactable)
		&& (CurrentGrabbable != Interactable)) // grabbables will be a certain distance off which might be out of their overlap range
	{
		FString GrabStr = TEXT("None");
		if (CurrentGrabbable)
			GrabStr = CurrentGrabbable->GetName();
		// UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("exit range: UnsetCurrentInteractable from %s, grab %s eq %i"),
			//(CurrentInteractable ? *CurrentInteractable->GetName() : TEXT("None")), *GrabStr, CurrentGrabbable == CurrentInteractable)); 
		UnsetCurrentInteractable();
	}
}