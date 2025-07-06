// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ColorPaletteDataAsset.generated.h"

UENUM(BlueprintType)
enum class EColorOption : uint8
{
	ECO_None UMETA(DisplayName = "None"),
	ECO_White UMETA(DisplayName = "White"),
	ECO_Red UMETA(DisplayName = "Red"),
	ECO_Green UMETA(DisplayName = "Green"),
	ECO_Blue UMETA(DisplayName = "Blue"),
	ECO_Yellow UMETA(DisplayName = "Yellow"),
};

UCLASS()
class MYM_API UColorPaletteDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	static FString GetColorOptionString(EColorOption EnumValue)
	{
		const UEnum* EnumPtr = StaticEnum<EColorOption>();
		if (!EnumPtr) return TEXT("Invalid");

		return EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(EnumValue)).ToString();
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EColorOption, FLinearColor> ColorPalette;
};
