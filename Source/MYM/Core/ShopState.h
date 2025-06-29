// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MYM/Order/OrderDataAsset.h"
#include "ShopState.generated.h"

class UOrderDataAsset;
class AMymPlayerController;

// Expose events to blueprint/UI
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBudgetChanged, int32, NewBudget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOrderDelegate, FOrderData, OrderData);

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
	UFUNCTION(BlueprintCallable, Category = "Shop|Budget")
	void TryPurchase(AMymPlayerController* InstigatingPC, int32 Cost, TSubclassOf<AActor> SpawnBPClass, const FTransform& Location);
	
	UFUNCTION(Server, Reliable, Category = "Shop|Budget")
	void TryPurchase_Auth(AMymPlayerController* InstigatingPC, int32 Cost, TSubclassOf<AActor> SpawnBPClass, const FTransform& Location);
	
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
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_ShopBudget)
	int32 ShopBudget = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> SpawnOrderBP;

	UFUNCTION()
	void OnRep_ShopBudget();

public:
	UPROPERTY(EditDefaultsOnly, Category = "Shop|Orders")
	TArray<UOrderDataAsset*> OrderList;
	
	UPROPERTY(BlueprintAssignable, Category = "Shop")
	FOnBudgetChanged OnBudgetUpdated;
	
	// order generation
	FOrderDelegate OnOrderCreated;
};
