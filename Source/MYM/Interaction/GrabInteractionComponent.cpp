// Fill out your copyright notice in the Description page of Project Settings.
#include "GrabInteractionComponent.h"
#include "InteractionTrackerComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MYM/Core/MymCharacter.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

UGrabInteractionComponent::UGrabInteractionComponent()
{
	InteractionString = "Grab";

	PrimaryComponentTick.bCanEverTick = true;
	UActorComponent::SetComponentTickEnabled(true);
}

void UGrabInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OnInteractBegin.AddDynamic(this, &UGrabInteractionComponent::StartGrabbing);
	OnInteractEnd.AddDynamic(this, &UGrabInteractionComponent::StopGrabbing);
}

void UGrabInteractionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	OnInteractBegin.RemoveDynamic(this, &UGrabInteractionComponent::StartGrabbing);
	OnInteractEnd.RemoveDynamic(this, &UGrabInteractionComponent::StopGrabbing);
}

void UGrabInteractionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bBlockTick) return;
	
	if (GetOwner()->HasAuthority())
		TickGrabbing();
}

void UGrabInteractionComponent::StartGrabbing_Implementation(UInteractionTrackerComponent* ByTracker)
{
	if (Grabber)
	{
		UE_LOG(LogInteraction, Warning, TEXT("GrabbingActor already set!"));
		return;
	}
	Grabber = ByTracker;
	Grabber->SetGrabbable_Client(this);
	Grabber->SetGrabbable(this);
	UPrimitiveComponent* GrabComponent = Cast<UPrimitiveComponent>(GetAttachmentRoot());
	if (!GrabComponent)
	{
		UE_LOG(LogInteraction, Error, TEXT("Grab Unsuccessful: Root component doesn't derive from PrimitiveComponent"));
		return;
	}
	Grabber->GetCharacter()->GetGrabHandle()->GrabComponentAtLocation(GrabComponent, FName("None"), GrabComponent->GetComponentLocation());
	bBlockTick = false;

	Grabber->PauseInteractionTesting();
}

void UGrabInteractionComponent::TickGrabbing_Implementation()
{
	if (!Grabber)
	{
		return;
	}

	FVector Location;
	GetGrabLocationAndRotation(Location);
	Grabber->GetCharacter()->GetGrabHandle()->SetTargetLocation(Location);
}

void UGrabInteractionComponent::StopGrabbing_Implementation(UInteractionTrackerComponent* ByTracker)
{
	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Grabbing stopped %s %s"), *ByTracker->GetName(), (Grabber ? *Grabber->GetName() : TEXT("None"))));
	if (Grabber && ByTracker != Grabber)
	{
		UE_LOG(LogInteraction, Warning, TEXT("GrabbingActor not set/ doesn't match tracker!"));
		return;
	}
	ByTracker->GetCharacter()->GetGrabHandle()->ReleaseComponent();
	ByTracker->SetGrabbable(nullptr);
	ByTracker->SetGrabbable_Client(nullptr);
	
	ByTracker->ResumeInteractionTesting();
	
	bBlockTick = true;
	Grabber = nullptr;
}

void UGrabInteractionComponent::GetGrabLocationAndRotation(FVector& OutLocation) const
{
	if (!Grabber || !Grabber->GetCharacter())
	{
		UE_LOG(LogInteraction, Error, TEXT("Grabber/ Grabbing Actor not set!"));
		return;
	}
	OutLocation = Grabber->GetCharacter()->GetActorLocation() + (CurrentPlayerCameraForward * GrabDistance);
}
