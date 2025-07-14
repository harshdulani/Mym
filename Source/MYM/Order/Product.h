// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Resource.h"
#include "GameFramework/Actor.h"
#include "Product.generated.h"

class AProduct;
class UGrabInteractionComponent;

USTRUCT(BlueprintType)
struct FRecipe
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<AResource>> Ingredients;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AProduct> Result;

	bool EvaluateRecipe(TArray<TSubclassOf<AResource>> CurrentIngredients)
	{
		for (auto& Ingredient : Ingredients)
		{
			if (Ingredients.IsEmpty()) return false;
			if (auto idx = CurrentIngredients.Find(Ingredient))
			{
				CurrentIngredients.RemoveAt(idx);
			}
			else
			{
				// didnt find req ingredient, exiting 
				return false;
			}
		}
		return true;
	}
};

UCLASS()
class MYM_API AProduct : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AProduct();

public:
	UFUNCTION(BlueprintPure, Category = "Product")
	UStaticMesh* GetStaticMesh() const { return MainMesh->GetStaticMesh(); }
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Product")
	void PaintProduct(const FLinearColor& NewColor);
	
private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components", meta=(AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> MainMesh;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Components", meta=(AllowPrivateAccess = true))
	TObjectPtr<UGrabInteractionComponent> GrabComponent;

	UPROPERTY()
	UMaterialInstanceDynamic* MaterialInstance = nullptr;
};
