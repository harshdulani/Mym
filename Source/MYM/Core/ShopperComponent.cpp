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

void UShopperComponent::TryPurchase_Auth_Implementation(AMymPlayerController* InstigatingPC, int32 Cost, TSubclassOf<AActor> SpawnBPClass, const FTransform& Location)
{
	UKismetSystemLibrary::PrintString(this,
		FString::Printf(TEXT("TryPurchase_Auth %s"), *UpdateGetShopState()->GetName()));
	if (UpdateGetShopState())
		UpdateGetShopState()->TryPurchase(InstigatingPC, Cost, SpawnBPClass, Location);
}

