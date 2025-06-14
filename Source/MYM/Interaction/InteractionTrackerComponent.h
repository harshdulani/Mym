// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionTrackerComponent.generated.h"

class AMymCharacter;
class AMymHUD;
class UInteractionComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYM_API UInteractionTrackerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionTrackerComponent();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void InteractBegin();
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void InteractEnd();

	
	// don't trace for interactables WHILE interacting with one, helps in cases like grab and move
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void PauseInteractionTesting();
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ResumeInteractionTesting();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void InteractableEnterRange(UInteractionComponent* Interactable);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void InteractableExitRange(UInteractionComponent* Interactable);

	void SetHUDReference(AMymHUD* HUD) { MymHUD = HUD; }
	void SetCharacterReference(AMymCharacter* Character) { MymCharacter = Character; }
	AMymCharacter* GetCharacter() const { return MymCharacter; }
	void InteractableDisabled(UInteractionComponent* Interactable);

protected:
	void TraceForInteractables();
	void CheckForDisabledInteractables();

	void UnsetCurrentInteractable();
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	TArray<UInteractionComponent*> InteractablesInRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UInteractionComponent* CurrentInteractable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	float InteractionTraceRange = 256.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	AMymHUD* MymHUD = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	AMymCharacter* MymCharacter = nullptr;
};
