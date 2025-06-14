// Copyright Epic Games, Inc. All Rights Reserved.

#include "MYMPickUpComponent.h"
#include "Core/MymCharacter.h"

UMYMPickUpComponent::UMYMPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UMYMPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UMYMPickUpComponent::OnSphereBeginOverlap);
}

void UMYMPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AMymCharacter* Character = Cast<AMymCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
