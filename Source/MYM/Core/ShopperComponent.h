// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShopperComponent.generated.h"


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
	void TryPurchase_Auth(AMymPlayerController* InstigatingPC, int32 Cost, TSubclassOf<AActor> SpawnBPClass, const FTransform& Location);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AShopState> ShopState;
};
