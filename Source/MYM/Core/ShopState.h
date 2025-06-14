// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ShopState.generated.h"

// Expose events to blueprint/UI
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBudgetChanged, int32, NewBudget);

UCLASS()
class MYM_API AShopState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AShopState();

	UFUNCTION(BlueprintCallable, Category = "Shop|Budget")
	int32 GetShopBudget() const { return ShopBudget; }

	/** Attempts to make a purchase (server side only) */
	UFUNCTION(BlueprintCallable, Category = "Shop|Budget")
	bool TryPurchase(int32 Cost);

protected:
	UPROPERTY(ReplicatedUsing = OnRep_ShopBudget)
	int32 ShopBudget = 0;

	UFUNCTION()
	void OnRep_ShopBudget();

public:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintAssignable, Category = "Shop")
	FOnBudgetChanged OnBudgetChanged;
	
};
