// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OrderDataAsset.generated.h"

UENUM(BlueprintType)
enum class EOrderColor : uint8
{
	EOC_Clear UMETA(DisplayName = "Clear"),
	EOC_Yellow UMETA(DisplayName = "Yellow"),
	EOC_White UMETA(DisplayName = "White"),
	EOC_Blue UMETA(DisplayName = "Blue"),
	EOC_Red UMETA(DisplayName = "Red"),
};

// BPType and BPable, in case designers want to add more options/ details about order item
USTRUCT(BlueprintType)
struct MYM_API FOrderData
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* TargetMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EOrderColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Charge = 0;

	FOrderData()
	{
		TargetMesh = nullptr;
		ItemName = NAME_None;
		Color = EOrderColor::EOC_Clear;
		Charge = 0;
	}
};

/**
 * 
 */
UCLASS()
class MYM_API UOrderDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FOrderData Data;
};
