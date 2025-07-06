// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ColorPaletteDataAsset.h"
#include "OrderDataAsset.generated.h"

// BPType and BPable, in case designers want to add more options/ details about order item
USTRUCT(BlueprintType)
struct MYM_API FOrderData
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* TargetMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EColorOption Color;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Charge = 0;

	FOrderData()
	{
		TargetMesh = nullptr;
		ItemName = NAME_None;
		Color = EColorOption::ECO_White;
		Charge = 0;
	}
};

UCLASS()
class MYM_API UOrderDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FOrderData Data;
};
