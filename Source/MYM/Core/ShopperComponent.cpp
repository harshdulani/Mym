// Fill out your copyright notice in the Description page of Project Settings.
#include "ShopperComponent.h"

#include "MymPlayerController.h"
#include "ShopState.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UShopperComponent::UShopperComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UShopperComponent::BeginPlay()
{
	Super::BeginPlay();

}

AShopState* UShopperComponent::UpdateGetShopState()
{
	if (!ShopState)
	{
		ShopState = Cast<AShopState>(GetWorld()->GetGameState<AShopState>());
	}
	return ShopState.Get();
}

void UShopperComponent::TryPurchaseWood_Auth_Implementation(AMymPlayerController* InstigatingPC, const FTransform& Location)
{
	if (UpdateGetShopState())
		UpdateGetShopState()->TryPurchaseWood_Auth(InstigatingPC, Location);
}

void UShopperComponent::TryPurchasePaint_Auth_Implementation(AMymPlayerController* InstigatingPC, EColorOption PaintColor, const FTransform& Location)
{
	if (UpdateGetShopState())
		UpdateGetShopState()->TryPurchasePaint_Auth(InstigatingPC, PaintColor, Location);
}

