// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ShopState.generated.h"

class AMymPlayerController;
// Expose events to blueprint/UI
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBudgetChanged, int32, NewBudget);

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
	
	UFUNCTION(NetMulticast, Reliable, Category = "Shop|Budget")
	void OnPurchase_Client(int32 NewBudget);
	
protected:
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_ShopBudget)
	int32 ShopBudget = 0;

	UFUNCTION()
	void OnRep_ShopBudget();

public:
	
	UPROPERTY(BlueprintAssignable, Category = "Shop")
	FOnBudgetChanged OnBudgetUpdated;
	
};
