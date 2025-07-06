// Fill out your copyright notice in the Description page of Project Settings.
#include "Resource.h"
#include "MYM/Interaction/GrabInteractionComponent.h"

AResource::AResource()
{
	bReplicates = true;
	AActor::SetReplicateMovement(true);
	
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	MainMesh->SetupAttachment(RootScene);

	GrabComponent = CreateDefaultSubobject<UGrabInteractionComponent>(TEXT("Grab Interaction Component"));
	GrabComponent->SetupAttachment(RootScene);

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
