#include "ShopState.h"

#include "MymPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MYM/Order/OrderDataAsset.h"
#include "MYM/Order/OrderSheet.h"
#include "Net/UnrealNetwork.h"

AShopState::AShopState()
{
	bReplicates = true;
}

void AShopState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShopState, ShopBudget);
}

void AShopState::TryPurchase(AMymPlayerController* InstigatingPC, int32 Cost, TSubclassOf<AActor> SpawnBPClass,
                             const FTransform& Location)
{
	TryPurchase_Auth(InstigatingPC, Cost, SpawnBPClass, Location);
}

void AShopState::GenerateOrder()
{
	GenerateOrder_Auth();
}

int AShopState::GetOrderCharge(const FOrderData& OrderInstance, const FOrderData& OrderSpec) const
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

void AShopState::TryPurchase_Auth_Implementation(AMymPlayerController* InstigatingPC, int32 Cost,
                                                 TSubclassOf<AActor> SpawnBPClass, const FTransform& Location)
{
	if (Cost <= 0 || ShopBudget < Cost)
	{
		return;
	}
	ShopBudget -= Cost;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = InstigatingPC;
	GetWorld()->SpawnActor<AActor>(SpawnBPClass, Location, SpawnParams);

	OnRep_ShopBudget();
}

void AShopState::OnRep_ShopBudget()
{
	if (OnBudgetUpdated.IsBound())
		OnBudgetUpdated.Broadcast(ShopBudget);
}

void AShopState::GenerateOrder_Auth_Implementation()
{
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsWithTag(this, FName(TEXT("OrderTray_In")), SpawnPoints);
	if (SpawnPoints.Num() == 0) return;

	TArray<UActorComponent*> Components = SpawnPoints[0]->GetComponentsByTag(
		USceneComponent::StaticClass(), FName(TEXT("Order_SpawnPoint")));
	if (Components.Num() == 0) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform SpawnTransform = Cast<USceneComponent>(Components[0])->GetComponentTransform();
	auto Actor = GetWorld()->SpawnActor<AActor>(SpawnOrderBP, SpawnTransform);
	if (!Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawned Order Actor is NULL!"));
		return;
	}
	AOrderSheet* Order = Cast<AOrderSheet>(Actor);
	if (OrderList.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("OrderList is empty!"));
		return;
	}
	// encapsulating a FOrderData struct inside it,
	// we can easily go for a make random order from lists of order details
	auto OrderData = OrderList[FMath::RandRange(0, OrderList.Num() - 1)]->Data;
	Order->InitOrder(OrderData);
	//UKismetSystemLibrary::PrintString(
	//	this, FString::Printf(TEXT("Spawned Order Actor Owner: %s"), *Actor->GetOwner()->GetName()));
}

void AShopState::OnOrderCreated_Client_Implementation(const FOrderData& Order)
{
	UE_LOG(LogTemp, Display, TEXT("AShopState::OnOrderCreated"));
}
