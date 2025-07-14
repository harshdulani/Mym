// Fill out your copyright notice in the Description page of Project Settings.
#include "Resource.h"
#include "MYM/Interaction/GrabInteractionComponent.h"

AResource::AResource()
{
	bReplicates = true;
	AActor::SetReplicateMovement(true);
	
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	SetRootComponent(MainMesh);

	GrabComponent = CreateDefaultSubobject<UGrabInteractionComponent>(TEXT("Grab Interaction Component"));
	GrabComponent->SetupAttachment(MainMesh);
	GrabComponent->OwnedPrimitives.Add(MainMesh);
	
	MainMesh->SetAngularDamping(0.3f);
	MainMesh->SetIsReplicated(true);
}

void AResource::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
		MainMesh->SetSimulatePhysics(true);
}

int32 AResource::GetResourceCost() const
{
	return Cost;
}
