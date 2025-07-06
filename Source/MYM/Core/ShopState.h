// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MymPlayerController.h"
#include "GameFramework/GameStateBase.h"
#include "MYM/Order/OrderDataAsset.h"
#include "MYM/Order/Resource.h"
#include "ShopState.generated.h"

USTRUCT(BlueprintType)
struct FCartItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FText ItemName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EColorOption Color = EColorOption::ECO_None;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 CostPerItem;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Quantity;

	bool operator==(const FCartItemData& That) const
	{
		// check with a cheap int check first and only then compare names
		 return this->CostPerItem == That.CostPerItem &&
				this->Color == That.Color &&
				this->ItemName.EqualTo(That.ItemName); 
	}
};

class UOrderDataAsset;
class AMymPlayerController;

// Expose events to blueprint/UI
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBudgetChanged, int32, NewBudget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOrderDelegate, FOrderData, OrderData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCartDelegate, const TArray<FCartItemData>&, Cart);

UCLASS()
class MYM_API AShopState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AShopState();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Shop|Budget")
	int32 GetShopBudget() const { return ShopBudget; }

	/** Attempts to make a purchase (server side only) */
	UFUNCTION(Server, Reliable, Category = "Shop|Budget")
	void TryPurchaseWood_Auth(AMymPlayerController* InstigatingPC, const FTransform& Location);
	
	UFUNCTION(Server, Reliable, Category = "Shop|Budget")
	void TryPurchasePaint_Auth(AMymPlayerController* InstigatingPC, EColorOption Color, const FTransform& Location);
	
	UFUNCTION(NetMulticast, Reliable, Category = "Shop|Orders")
	void OnOrderCreated_Client(const FOrderData& Order);

	// you shuoldnt be able to call it from a client but still adding a call for any preprocessing one might need
	UFUNCTION(BlueprintCallable, Category = "Shop|Orders")
	void GenerateOrder();
	
	UFUNCTION(Server, Reliable, Category = "Shop|Orders")
	void GenerateOrder_Auth();

	UFUNCTION(BlueprintCallable, Category = "Order")
	int GetOrderCharge(const FOrderData& OrderInstance, const FOrderData& OrderSpec) const;
	
protected:
	UFUNCTION()
	void OnRep_ShopBudget();
	void HandleBudgetUpdated() const;

	UFUNCTION(BlueprintCallable, Category = "Shop")
	int32 GetResourceCost(TSubclassOf<AActor> ResourceBP);

	UFUNCTION(BlueprintCallable, Category = "Shop")
	AResource* SpawnResource(const TSubclassOf<AActor>& ResourceBP, AMymPlayerController* InstigatingPC, const FTransform& Location);
	
	UFUNCTION()
	void OnRep_CurrentCart();
	void HandleCartUpdated() const;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Shop|Orders")
	TArray<UOrderDataAsset*> OrderList;
	
	UPROPERTY(BlueprintAssignable, Category = "Shop")
	FOnBudgetChanged OnBudgetUpdated;
	
	// order generation
	FOrderDelegate OnOrderCreated;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shop")
	UColorPaletteDataAsset* ColorPalette = nullptr;
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Shop")
	void AddItemToCart_Auth(FCartItemData CartItemData);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Shop")
	void RemoveItemFromCart_Auth(FCartItemData CartItemData);	
	
protected:
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_ShopBudget)
	int32 ShopBudget = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> SpawnOrderBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<AResource>> ResourceList;
	
	UPROPERTY(Replicated, VisibleAnywhere, ReplicatedUsing=OnRep_CurrentCart)
	TArray<FCartItemData> CurrentCart;

	UPROPERTY(BlueprintAssignable, Category = "Shop")
	FCartDelegate OnCartUpdated;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shop", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> WoodBP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shop", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> PaintBP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shop", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* PaintMaterial = nullptr;
};
