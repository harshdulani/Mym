// Copyright Epic Games, Inc. All Rights Reserved.

#include "MymGameMode.h"
#include "MymCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MYM/Order/OrderSheet.h"
#include "UObject/ConstructorHelpers.h"

class AOrderSheet;

AMymGameMode::AMymGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AMymGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsWithTag(this, FName(TEXT("OrderTray_In")), SpawnPoints);
}

void AMymGameMode::GenerateOrder_Auth()
{
	if (SpawnPoints.Num() == 0) return;

	TArray<UActorComponent*> Components = SpawnPoints[0]->GetComponentsByTag(
		USceneComponent::StaticClass(), FName(TEXT("Order_SpawnPoint")));
	if (Components.Num() == 0) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FTransform SpawnTransform = Cast<USceneComponent>(Components[0])->GetComponentTransform();
	auto Actor = GetWorld()->SpawnActor<AActor>(SpawnOrderBP, SpawnTransform);
	if (!Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawned Order Actor is NULL!"));
		return;
	}
	AOrderSheet* Order = Cast<AOrderSheet>(Actor);
	FOrderData OrderData;
	if (!bSpawnRandomOrder && OrderList.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("OrderList is empty!"));
		return;
	}

	if (bSpawnRandomOrder)
	{
		if (OrderStaticMeshes.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("OrderStaticMeshes array is empty!"));
			return;
		}

		// Get random static mesh and its name
		UStaticMesh* RandomMesh = OrderStaticMeshes[FMath::RandRange(0, OrderStaticMeshes.Num() - 1)];
		FString MeshName = RandomMesh->GetName();

		// Trim S_ or SM_ prefix if present
		if (MeshName.StartsWith(TEXT("S_")))
		{
			MeshName.RightChopInline(2);
		}
		else if (MeshName.StartsWith(TEXT("SM_")))
		{
			MeshName.RightChopInline(3);
		}

		// Get random color option
		EColorOption RandomColor = static_cast<EColorOption>(FMath::RandRange(0, static_cast<int32>(EColorOption::ECO_MAX) - 1));
		FString ColorName = UEnum::GetDisplayValueAsText(RandomColor).ToString();

		// Combine mesh name and color
		OrderData.ItemName = FName(MeshName + TEXT(" ") + ColorName);
		OrderData.TargetMesh = RandomMesh;
		OrderData.Color = RandomColor;
		OrderData.Charge = FMath::RandRange(1, 100) * FMath::RandRange(1, ++LastOrderId);
	}
	else
	{
		// encapsulating an FOrderData struct inside it,
		// so we can easily go for a make random order from lists of order details
		OrderData = OrderList[FMath::RandRange(0, OrderList.Num() - 1)]->Data;
	}
	Order->InitOrder(OrderData);
	//UKismetSystemLibrary::PrintString(
	//	this, FString::Printf(TEXT("Spawned Order Actor Owner: %s"), *Actor->GetOwner()->GetName()));
	if (auto ShopState = GetWorld()->GetGameState<AShopState>())
	{
		ShopState->OnOrderCreated_Multicast(OrderData);
	}
}

int AMymGameMode::GetOrderCharge(const FOrderData& OrderInstance, const FOrderData& OrderSpec) const
{
	float Score = 0.f;
	if (OrderInstance.TargetMesh == OrderSpec.TargetMesh)
	{
		Score += 0.5f;
		if (OrderSpec.Color == OrderInstance.Color)
		{
			Score += 0.5f;
		}
	}
	return Score * OrderSpec.Charge;
}

int32 AMymGameMode::GetClosestOrderDataIdx(AProduct* TestProduct) const
{
	for (const auto& ActiveOrder : ActiveOrders)
	{
	}
	return -1;
}

void AMymGameMode::AddToActiveOrders(FOrderData OrderData)
{
	ActiveOrders.Add(OrderData);
}
