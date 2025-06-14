// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MymHUD.generated.h"

UCLASS()
class AMymHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction")
	void ShowInteractionWidget(const FString& GetInteractionString);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction")
	void HideInteractionWidget();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction")
	void ShowCrosshair();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction")
	void HideCrosshair();

};
