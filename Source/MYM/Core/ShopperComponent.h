// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShopState.h"
#include "Components/ActorComponent.h"
#include "ShopperComponent.generated.h"

enum class EColorOption : uint8;
class AMymPlayerController;
class AShopState;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYM_API UShopperComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShopperComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Shop")
	AShopState* UpdateGetShopState();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Shop")
	void AddItemToCart(FCartItemData ItemData);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Shop")
	void RemoveItemFromCart(FCartItemData ItemData, bool bRemoveCompletely);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Shop")
	void BuyCart(AMymPlayerController* InstigatingPC, const FTransform& SpawnPoint);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Shop")
	void TryPurchaseWood_Auth(AMymPlayerController* InstigatingPC, const FTransform& Location);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Shop")
	void TryPurchasePaint_Auth(AMymPlayerController* InstigatingPC, EColorOption PaintColor, const FTransform& Location);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AShopState> ShopState;
};
