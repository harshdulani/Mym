// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionTrackerComponent.generated.h"

class UGrabInteractionComponent;
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
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Reliable, Category = "Interaction")
	void Auth_InteractBegin();
	UFUNCTION(Server, Reliable, Category = "Interaction")
	void Auth_InteractEnd();

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

	// set locally from client to itself
	void SetGrabbable(UGrabInteractionComponent* Grabbable);
	// set from client to server
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interaction|Grab")
	void SetGrabbable_Auth(UGrabInteractionComponent* Grabbable);
	// set to server from client
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Interaction|Grab")
	void SetGrabbable_Client(UGrabInteractionComponent* Grabbable);

	// Unreliable because we dont care if we miss a couple of Rotation updates
	UFUNCTION(Server, Unreliable, BlueprintCallable, Category = "Interaction|Grab")
	void SendCameraForward_Auth(FVector NewForward);
	
	UFUNCTION(BlueprintPure, Category = "Interaction|Grab")
	UGrabInteractionComponent* GetGrabbable() const { return CurrentGrabbable.Get(); }

	
protected:
	void TraceForInteractables();
	void CheckForDisabledInteractables();

	void SetCurrentInteractable(UInteractionComponent* NewInteractable);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Interaction|Refs")
	void SetCurrentInteractable_Auth(UInteractionComponent* NewInteractable);
	
private:
	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly, Category = "Interaction|Refs", meta = (AllowPrivateAccess = "true"))
	TArray<UInteractionComponent*> InteractablesInRange;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly, Category = "Interaction|Refs", meta = (AllowPrivateAccess = "true"))
	UInteractionComponent* CurrentInteractable = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Refs", meta = (AllowPrivateAccess = "true"))
	AMymHUD* MymHUD = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Refs", meta = (AllowPrivateAccess = "true"))
	AMymCharacter* MymCharacter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	float InteractionTraceRange = 256.f;

	UPROPERTY()
	TObjectPtr<UGrabInteractionComponent> CurrentGrabbable;
	
	bool bInteractionHeld = false;
	bool bInteractionTrace = true;
};
