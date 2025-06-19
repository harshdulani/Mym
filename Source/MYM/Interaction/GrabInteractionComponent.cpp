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
}

void UGrabInteractionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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
	
	if (GetOwner()->HasAuthority())
		TickGrabbing();
}

void UGrabInteractionComponent::StartGrabbing_Implementation(UInteractionTrackerComponent* ByTracker)
{
	if (Grabber.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("GrabbingActor already set!"));
		return;
	}
	Grabber = ByTracker;
	Grabber->SetGrabbable(this);
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Grabbing started %s %s"), *ByTracker->GetName(), *Grabber->GetName()));
	GetOwner()->SetOwner(ByTracker->GetCharacter());
	UPrimitiveComponent* GrabComponent = Cast<UPrimitiveComponent>(GetAttachmentRoot());
	if (!GrabComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Grab Unsuccessful: Root component doesn't derive from PrimitiveComponent"));
		return;
	}
	Grabber->GetCharacter()->GetGrabHandle()->GrabComponentAtLocation(GrabComponent, FName("None"), GrabComponent->GetComponentLocation());
	SetComponentTickEnabled(true);

	Grabber->PauseInteractionTesting();
}

void UGrabInteractionComponent::TickGrabbing_Implementation()
{
	if (!Grabber.IsValid())
	{
		return;
	}

	FVector Location;
	FRotator Rotation;
	GetGrabLocationAndRotation(Location, Rotation);
	Grabber->GetCharacter()->GetGrabHandle()->SetTargetLocationAndRotation(Location, Rotation);
}

void UGrabInteractionComponent::StopGrabbing_Implementation(UInteractionTrackerComponent* ByTracker)
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Grabbing stopped %s %s"), *ByTracker->GetName(), (Grabber.IsValid() ? *Grabber->GetName() : TEXT("None"))));
	if (Grabber.IsValid() && ByTracker != Grabber.Get())
	{
		UE_LOG(LogTemp, Warning, TEXT("GrabbingActor not set/ doesn't match tracker!"));
		return;
	}
	ByTracker->GetCharacter()->GetGrabHandle()->ReleaseComponent();
	ByTracker->SetGrabbable(nullptr);
	
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
	OutLocation = MymCharacter->GetActorLocation() + (MymCharacter->GetCameraForward() * GrabDistance);
}
