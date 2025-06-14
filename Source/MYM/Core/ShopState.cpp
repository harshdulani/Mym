#include "ShopState.h"
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

bool AShopState::TryPurchase(int32 Cost)
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot purchase from a non-authoritative state"));
		return false;
	}
	
	if (Cost <= 0 || ShopBudget < Cost)
	{
		return false;
	}

	ShopBudget -= Cost;
	OnRep_ShopBudget(); // update locally for listen server
	return true;
}

void AShopState::OnRep_ShopBudget()
{
	OnBudgetChanged.Broadcast(ShopBudget);
}