#include "InteractionComponent.h"
#include "InteractionTrackerComponent.h"
#include "MYM/Core/MymCharacter.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SphereRadius = 256.f;
}

bool UInteractionComponent::OwnsPrimitive(UPrimitiveComponent* Primitive) const
{
	return OwnedPrimitives.Contains(Primitive);
}

// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	bManageRange = true;
	OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::EnterRange);
	OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::ExitRange);

#if WITH_EDITOR
	if (OwnedPrimitives.Num() == 0)
	{
		TArray<UInteractionComponent*> Components;
		GetOwner()->GetComponents<UInteractionComponent>(Components);
		if (Components.Num() > 1)
		{
			UE_LOG(LogInteraction, Error, TEXT("Interaction Component does not have owned primitives assigned. This will cause unexpected behaviour because you have multiple interactables in this actor"));
		}
	}
#endif
}

void UInteractionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!bManageRange) return;
	bManageRange = false;
	OnComponentBeginOverlap.RemoveDynamic(this, &UInteractionComponent::EnterRange);
	OnComponentEndOverlap.RemoveDynamic(this, &UInteractionComponent::ExitRange);
}

void UInteractionComponent::BeginInteraction(UInteractionTrackerComponent* Tracker)
{
	if (IsDisabled() || IsBlocked()) return;

	if (OnInteractBegin.IsBound())
		OnInteractBegin.Broadcast(Tracker);
}

void UInteractionComponent::EndInteraction(UInteractionTrackerComponent* Tracker)
{
	if (IsDisabled() || IsBlocked()) return;

	if (OnInteractEnd.IsBound())
		OnInteractEnd.Broadcast(Tracker);

	if (bAllowsAllowed) return;
	
	if (!CycleToNextState())
	{
		Tracker->InteractableDisabled(this);
	}
}

bool UInteractionComponent::CycleToNextState()
{
	if (StateCycleOrder.IsEmpty()) return false;
	
	FGameplayTag NextState = StateCycleOrder[0];
	StateCycleOrder.RemoveAt(0);
	if (NextState.IsValid())
	{
		CurrentState = NextState;
	}
	if (StateCycleOrder.IsEmpty() && IsDisabled())
	{
		if (!bManageRange) return false;
		bManageRange = false;
		OnComponentBeginOverlap.RemoveDynamic(this, &UInteractionComponent::EnterRange);
		return false;
	}
	return true;
}

FString UInteractionComponent::GetInteractionString() const
{
	if (CurrentState == StateBlocked)
		return InteractionString + FString(" (Blocked)"); 
	return InteractionString;
}

void UInteractionComponent::EnterRange(UPrimitiveComponent* OverlappedComponent,
									   AActor* OtherActor,
									   UPrimitiveComponent* OtherComp,
									   int32 OtherBodyIndex,
									   bool bFromSweep,
									   const FHitResult& SweepResult)
{
	if (AMymCharacter* PlayerChar = Cast<AMymCharacter>(OtherActor))
	{
		if (!PlayerChar->IsLocallyControlled())
		{
			return;
		}
		
		PlayerChar->GetInteractionTracker()->InteractableEnterRange(this);
	}
}

void UInteractionComponent::ExitRange(UPrimitiveComponent* OverlappedComponent,
									  AActor* OtherActor,
									  UPrimitiveComponent* OtherComp,
									  int32 OtherBodyIndex)
{
	if (AMymCharacter* PlayerChar = Cast<AMymCharacter>(OtherActor))
	{
		if (!PlayerChar->IsLocallyControlled())
		{
			return;
		}

		PlayerChar->GetInteractionTracker()->InteractableExitRange(this);
		if (!bManageRange)
		{
			OnComponentEndOverlap.RemoveDynamic(this, &UInteractionComponent::ExitRange);
		}
	}
}
