// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MYM/Order/OrderDataAsset.h"
#include "MymGameMode.generated.h"

class AProduct;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOrderDelegate, FOrderData, OrderData);

UCLASS(minimalapi)
class AMymGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMymGameMode();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Orders")
	void GenerateOrder_Auth();

	UFUNCTION(BlueprintCallable, Category = "Orders")
	int GetOrderCharge(const FOrderData& OrderInstance, const FOrderData& OrderSpec) const;

	UFUNCTION(BlueprintCallable, Category = "Orders")
	int32 GetClosestOrderDataIdx(AProduct* TestProduct) const;
	
	void AddToActiveOrders(FOrderData OrderData);
	
	// order generation
	FOrderDelegate OnOrderCreated;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Orders", meta=(AllowPrivateAccess = true))
	TArray<FOrderData> ActiveOrders;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Orders", meta=(AllowPrivateAccess = true))
	int32 LastOrderId = -1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Orders", meta=(AllowPrivateAccess = true))
	TSubclassOf<AActor> SpawnOrderBP;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Orders", meta=(AllowPrivateAccess = true))
	TArray<UOrderDataAsset*> OrderList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Orders", meta=(AllowPrivateAccess = true))
	bool bSpawnRandomOrder = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Orders", meta=(AllowPrivateAccess = true))
	TArray<UStaticMesh*> OrderStaticMeshes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Orders", meta=(AllowPrivateAccess = true))
	TArray<AActor*> SpawnPoints;
};