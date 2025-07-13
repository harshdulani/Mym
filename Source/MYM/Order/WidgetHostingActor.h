// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WidgetHostingActor.generated.h"

class AMymPlayerController;
class UShopperComponent;
class UWidgetComponent;

DECLARE_DYNAMIC_DELEGATE_OneParam(FShoppingInitDelegate, UShopperComponent*, Shopper);

UCLASS()
class MYM_API AWidgetHostingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWidgetHostingActor();

	UFUNCTION(BlueprintPure, Category = "Components")
	UWidgetComponent* GetWidgetComponent() const { return WidgetComponent.Get(); }

	UFUNCTION(BlueprintImplementableEvent)
	void InitializeWidgetHostingActor(UShopperComponent* Shopper, AMymPlayerController* PlayerController);
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> RootScene;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetComponent> WidgetComponent;
};
