// Fill out your copyright notice in the Description page of Project Settings.
#include "Product.h"
#include "MYM/Interaction/GrabInteractionComponent.h"

// Sets default values
AProduct::AProduct()
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

void AProduct::PaintProduct_Implementation(const FLinearColor& NewColor)
{
	UMaterialInterface* BaseMaterial = MainMesh->GetMaterial(0);
	MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	MainMesh->SetMaterial(0, MaterialInstance);
	MaterialInstance->SetVectorParameterValue(TEXT("Color"), NewColor);
}
