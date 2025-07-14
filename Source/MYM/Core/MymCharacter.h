// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShopperComponent.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MymCharacter.generated.h"

class UWidgetComponent;
class UPhysicsHandleComponent;
class UInteractionTrackerComponent;
class AMymPlayerController;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class MYM_API AMymCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	AMymCharacter();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	void InteractBegin(const FInputActionValue& Value);
	void InteractEnd(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	virtual void BeginPlay() override;
	// End of APawn interface

public:
	/** Returns Player Controller as KodePlayerController **/
	UFUNCTION(BlueprintPure)
	AMymPlayerController* GetMymPC() const { return MymPC.Get(); }
	UFUNCTION(BlueprintPure)
	UPhysicsHandleComponent* GetGrabHandle() const { return GrabHandle; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE UInteractionTrackerComponent* GetInteractionTracker() const { return InteractionTracker; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE UShopperComponent* GetShopper() const { return Shopper; }
	
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	FVector GetCameraForward() const;

public:
	TWeakObjectPtr<AMymPlayerController> MymPC;

private:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Interact Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Interact Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* WidgetSelectAction;

	// Interactions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UInteractionTrackerComponent* InteractionTracker = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Grab", meta = (AllowPrivateAccess = "true"))
	UPhysicsHandleComponent* GrabHandle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop", meta = (AllowPrivateAccess = "true"))
	UShopperComponent* Shopper = nullptr;
};