#include "ShopState.h"

#include "MymPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
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

void AShopState::TryPurchase(AMymPlayerController* InstigatingPC, int32 Cost, TSubclassOf<AActor> SpawnBPClass, const FTransform& Location)
{
	TryPurchase_Auth(InstigatingPC, Cost, SpawnBPClass, Location);
}

void AShopState::OnPurchase_Client_Implementation(int32 NewBudget)
{
}

void AShopState::TryPurchase_Auth_Implementation(AMymPlayerController* InstigatingPC, int32 Cost, TSubclassOf<AActor> SpawnBPClass, const FTransform& Location)
{
	if (Cost <= 0 || ShopBudget < Cost)
	{
		return;
	}
	ShopBudget -= Cost;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = InstigatingPC;
	auto Ator = GetWorld()->SpawnActor<AActor>(SpawnBPClass, Location, SpawnParams);
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Spawned Actor Owner: %s"), *Ator->GetOwner()->GetName()));
	
    OnRep_ShopBudget();
}

void AShopState::OnRep_ShopBudget()
{
	if (OnBudgetUpdated.IsBound())
		OnBudgetUpdated.Broadcast(ShopBudget);
}
