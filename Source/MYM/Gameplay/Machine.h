// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Machine.generated.h"

class UInteractionTrackerComponent;
class UInteractionComponent;
class UTextRenderComponent;

UCLASS()
class MYM_API AMachine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMachine();

protected:

public:	

private:
	
public:

private:	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components", meta=(AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> MachineMesh;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Buttons", meta=(AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> ButtonSocket1;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Buttons", meta=(AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> ButtonMesh1;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Buttons", meta=(AllowPrivateAccess = true))
	TObjectPtr<UInteractionComponent> Interaction1;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Buttons", meta=(AllowPrivateAccess = true))
	TObjectPtr<UTextRenderComponent> Text1;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Buttons", meta=(AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> ButtonSocket2;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Buttons", meta=(AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> ButtonMesh2;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Buttons", meta=(AllowPrivateAccess = true))
	TObjectPtr<UInteractionComponent> Interaction2;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Buttons", meta=(AllowPrivateAccess = true))
	TObjectPtr<UTextRenderComponent> Text2;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Buttons", meta=(AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> ButtonSocket3;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Buttons", meta=(AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> ButtonMesh3;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Buttons", meta=(AllowPrivateAccess = true))
	TObjectPtr<UInteractionComponent> Interaction3;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Buttons", meta=(AllowPrivateAccess = true))
	TObjectPtr<UTextRenderComponent> Text3;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Buttons", meta=(AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> ButtonSocketDone;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Buttons", meta=(AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> ButtonMeshDone;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Buttons", meta=(AllowPrivateAccess = true))
	TObjectPtr<UInteractionComponent> InteractionDone;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Buttons", meta=(AllowPrivateAccess = true))
	TObjectPtr<UTextRenderComponent> TextDone;
};
