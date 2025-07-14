// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionComponent.h"
#include "GrabInteractionComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYM_API UGrabInteractionComponent : public UInteractionComponent
{
	GENERATED_BODY()

public:
	UGrabInteractionComponent();
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Grab")
	void StartGrabbing(UInteractionTrackerComponent* ByTracker);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Grab")
	void TickGrabbing();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Grab")
	void StopGrabbing(UInteractionTrackerComponent* ByTracker);

	UFUNCTION(BlueprintPure, Category="Grab")
	void GetGrabLocationAndRotation(FVector& OutLocation) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grab")
	float GrabDistance = 250.f;

	UPROPERTY(Transient)
	FVector CurrentPlayerCameraForward;

	UFUNCTION(BlueprintPure, Category="Grab")
	UInteractionTrackerComponent* GetGrabber() const { return Grabber; }

private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Grab", meta=(AllowPrivateAccess = true))
	UInteractionTrackerComponent* Grabber = nullptr;

	bool bBlockTick = true;
};
