// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MymPlayerController.generated.h"

class AMymCharacter;
class AMymHUD;
/**
 * 
 */
UCLASS()
class MYM_API AMymPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void AcknowledgePossession(class APawn* P) override;

public:
	UFUNCTION(BlueprintPure, Category = "HUD")
	AMymHUD* GetMymHUD() const { return MymHUD;}
	
	/** Returns Player Controller as MymPlayerController **/
	FORCEINLINE TWeakObjectPtr<AMymCharacter> GetMymCharacter() const { return MymCharacter; }

private:
	UPROPERTY(BlueprintReadWrite, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	AMymHUD* MymHUD;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMymHUD> MymHUDClassRef;
	
	TWeakObjectPtr<AMymCharacter> MymCharacter;
};
