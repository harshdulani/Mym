// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Product.h"
#include "Engine/DataAsset.h"
#include "ProductRecipeDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class MYM_API UProductRecipeDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRecipe ProductRecipe;
};
