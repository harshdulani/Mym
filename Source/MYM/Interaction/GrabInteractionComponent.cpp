// Fill out your copyright notice in the Description page of Project Settings.
#include "GrabInteractionComponent.h"
#include "InteractionTrackerComponent.h"
#include "Camera/CameraComponent.h"
#include "MYM/Core/MymCharacter.h"
#include "MYM/Core/MymHUD.h"
#include "MYM/Core/MymPlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

UGrabInteractionComponent::UGrabInteractionComponent()
{
	InteractionString = "Grab";

	PrimaryComponentTick.bCanEverTick = true;
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

	if (!Grabber.IsValid())
	{
		return;
	}

	FVector Location;
	FRotator Rotation;
	GetGrabLocationAndRotation(Location, Rotation);
	Grabber->GetCharacter()->GetGrabHandle()->SetTargetLocationAndRotation(Location, Rotation);
}

void UGrabInteractionComponent::StartGrabbing(UInteractionTrackerComponent* ByTracker)
{
	if (Grabber.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("GrabbingActor already set!"));
		return;
	}
	Grabber = ByTracker;
	UPrimitiveComponent* GrabComponent = Cast<UPrimitiveComponent>(GetAttachmentRoot());
	if (!GrabComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Root component doesn't derive from PrimitiveComponent"));
		return;
	}
	Grabber->GetCharacter()->GetGrabHandle()->GrabComponentAtLocation(GrabComponent, FName("None"), GrabComponent->GetComponentLocation());
	SetComponentTickEnabled(true);

	ByTracker->PauseInteractionTesting();
}

void UGrabInteractionComponent::StopGrabbing(UInteractionTrackerComponent* ByTracker)
{
	if (!Grabber.IsValid() || ByTracker != Grabber.Get())
	{
		UE_LOG(LogTemp, Warning, TEXT("GrabbingActor not set/ doesn't match tracker!"));
		return;
	}
	Grabber->GetCharacter()->GetGrabHandle()->ReleaseComponent();
	
	ByTracker->ResumeInteractionTesting();
	SetComponentTickEnabled(false);
	Grabber.Reset();
}

void UGrabInteractionComponent::GetGrabLocationAndRotation(FVector& OutLocation, FRotator& OutRotation) const
{
	if (!Grabber.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("GrabbingActor not set!"));
		return;
	}
	AMymCharacter* MymCharacter = Grabber->GetCharacter();
	
	OutRotation = MymCharacter->GetActorRotation();
	OutLocation = MymCharacter->GetActorLocation() + (MymCharacter->GetFirstPersonCameraComponent()->GetForwardVector() * GrabDistance);
}
