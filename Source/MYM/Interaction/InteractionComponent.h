// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SphereComponent.h"
#include "InteractionComponent.generated.h"

class UInteractionTrackerComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionDelegate, UInteractionTrackerComponent*, ByTracker);
DECLARE_LOG_CATEGORY_CLASS(LogInteraction, Warning, Log);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYM_API UInteractionComponent : public USphereComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	bool OwnsPrimitive(UPrimitiveComponent* Primitive) const;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void EnterRange(UPrimitiveComponent* OverlappedComponent,
					AActor* OtherActor,
					UPrimitiveComponent* OtherComp,
					int32 OtherBodyIndex,
					bool bFromSweep,
					const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ExitRange(UPrimitiveComponent* OverlappedComponent,
				   AActor* OtherActor,
				   UPrimitiveComponent* OtherComp,
				   int32 OtherBodyIndex);
	
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Broadcasts OnInteract delegate
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void BeginInteraction(UInteractionTrackerComponent* Tracker);
	// Broadcasts OnInteract delegate
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void EndInteraction(UInteractionTrackerComponent* Tracker);

	UFUNCTION(BlueprintCallable, Category = "Interaction|State")
	bool CycleToNextState();
	
	// Delegate to be called via blueprints or when InteractBegin() is called
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FInteractionDelegate OnInteractBegin;
	// Delegate to be called via blueprints or when InteractEnd() is called
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FInteractionDelegate OnInteractEnd;
	
	UFUNCTION(BlueprintPure, Category = "Interaction")
	bool IsAllowed() const { return CurrentState == StateAllowed; }
	UFUNCTION(BlueprintPure, Category = "Interaction")
	bool IsBlocked() const { return CurrentState == StateBlocked; }
	UFUNCTION(BlueprintPure, Category = "Interaction")
	bool IsDisabled() const { return CurrentState == StateDisabled; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	FString GetInteractionString() const;

private:
	
	
public:	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Interaction|State")
	FGameplayTag CurrentState;
	UPROPERTY(Replicated)
	bool bManageRange = true;
	
	// All the primitive components (static meshes, etc) that must be traced, when this interactable wants to be hit.
	// Warning: Unexpected behavior if same primitive is in this list for multiple InteractionComponents in an actor.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UPrimitiveComponent>> OwnedPrimitives;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	FString InteractionString;

	// for state mgmt
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|State", meta = (AllowPrivateAccess = "true"))
	bool bAllowsAllowed = true;
	// State cycle order (for GetNextState functionality)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|State", meta = (AllowPrivateAccess = "true"))
	TArray<FGameplayTag> StateCycleOrder;

	// these are universal default tags, but assignable via the details panel in case some interactable needs it 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|State|Default State Tags", meta = (AllowPrivateAccess = "true"))
	FGameplayTag StateAllowed = FGameplayTag::RequestGameplayTag(FName("Interaction.State.Allowed"));
	// these are universal default tags, but assignable via the details panel in case some interactable needs it 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|State|Default State Tags", meta = (AllowPrivateAccess = "true"))
	FGameplayTag StateBlocked = FGameplayTag::RequestGameplayTag(FName("Interaction.State.Blocked"));
	// these are universal default tags, but assignable via the details panel in case some interactable needs it 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|State|Default State Tags", meta = (AllowPrivateAccess = "true"))
	FGameplayTag StateDisabled = FGameplayTag::RequestGameplayTag(FName("Interaction.State.Disabled"));
	
};
