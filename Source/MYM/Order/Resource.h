// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Resource.generated.h"

class UGrabInteractionComponent;

UCLASS()
class MYM_API AResource : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	AResource();
	
	UFUNCTION(BlueprintPure, Category = "Resource")
	int32 GetResourceCost() const;

	UFUNCTION(BlueprintPure, Category = "Resource")
	UStaticMeshComponent* GetMainMesh() const { return MainMesh; }
	UFUNCTION(BlueprintPure, Category = "Resource")
	UGrabInteractionComponent* GetGrabComponent() const { return GrabComponent; }
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
	int32 Cost = 0;

private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components", meta=(AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> RootScene;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components", meta=(AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> MainMesh;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components", meta=(AllowPrivateAccess = true))
	TObjectPtr<UGrabInteractionComponent> GrabComponent;
};
