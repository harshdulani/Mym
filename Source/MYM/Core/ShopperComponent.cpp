// Fill out your copyright notice in the Description page of Project Settings.
#include "ShopperComponent.h"

#include "MymGameMode.h"
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

void UShopperComponent::AddToActiveOrders_Implementation(FOrderData OrderData)
{
	auto GM = Cast<AMymGameMode>(GetWorld()->GetAuthGameMode());
	GM->AddToActiveOrders(OrderData);
}

AShopState* UShopperComponent::UpdateGetShopState()
{
	if (!ShopState)
	{
		ShopState = GetWorld()->GetGameState<AShopState>();
	}
	if (!ShopState)
		UKismetSystemLibrary::PrintString(this, TEXT("ShopState is null!"));
	return ShopState.Get();
}

void UShopperComponent::BuyCart_Implementation(AMymPlayerController* InstigatingPC, const FTransform& SpawnPoint)
{
	if (UpdateGetShopState())
	{
		UpdateGetShopState()->BuyCart(InstigatingPC, SpawnPoint);
	}
}

void UShopperComponent::AddItemToCart_Implementation(FCartItemData ItemData)
{
	if (UpdateGetShopState())
	{
		UpdateGetShopState()->AddItemToCart_Auth(ItemData);
	}
}

void UShopperComponent::RemoveItemFromCart_Implementation(FCartItemData ItemData, bool bRemoveCompletely)
{
	if (UpdateGetShopState())
	{
		UpdateGetShopState()->RemoveItemFromCart_Auth(ItemData, bRemoveCompletely);
	}
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

