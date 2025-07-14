#include "ShopState.h"

#include "MymPlayerController.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "MYM/Order/OrderDataAsset.h"
#include "MYM/Order/OrderSheet.h"
#include "MYM/Order/Resource.h"
#include "Net/UnrealNetwork.h"

AShopState::AShopState()
{
	bReplicates = true;
}

void AShopState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShopState, ShopBudget);
	DOREPLIFETIME(AShopState, CurrentCart);
}

void AShopState::AddItemToCart_Auth_Implementation(const FCartItemData& CartItemData)
{
	auto CartItemIdx = CurrentCart.Find(CartItemData);
	if (CartItemIdx == INDEX_NONE)
	{
		CurrentCart.Add(CartItemData);
	}
	else
	{
		++CurrentCart[CartItemIdx].Quantity;
	}
	if (HasAuthority() && GetNetMode() == NM_ListenServer)
		HandleCartUpdated();
}

void AShopState::RemoveItemFromCart_Auth_Implementation(const FCartItemData& CartItemData, bool bRemoveCompletely)
{
	auto CartItemIdx = CurrentCart.Find(CartItemData);
	if (CartItemIdx == INDEX_NONE)
	{
		return;
	}
	if (--CurrentCart[CartItemIdx].Quantity <= 0 || bRemoveCompletely)
		CurrentCart.RemoveAt(CartItemIdx);
	
	if (HasAuthority() && GetNetMode() == NM_ListenServer)
		HandleCartUpdated();
}

void AShopState::BuyCart_Implementation(AMymPlayerController* InstigatingPC, const FTransform& SpawnPoint)
{
	float Delay = 0.f;
	for (const auto& CartItem : CurrentCart)
	{
		// if cart item is wood
		if (CartItem.Color == EColorOption::ECO_None)
		{
			for (int32 i = 0; i < CartItem.Quantity; ++i)
			{
				FTimerHandle TimerHandle;
				GetWorldTimerManager().SetTimer(TimerHandle, [this, SpawnPoint, InstigatingPC]() -> void
					{
						TryPurchaseWood_Auth(InstigatingPC, SpawnPoint);
					}, Delay, false);
				// Delay to allow assets to fall down and not cause collision problems
				Delay += 0.5f;
			}
		}
		else
		{
			for (int32 i = 0; i < CartItem.Quantity; ++i)
			{
				FTimerHandle TimerHandle;
				GetWorldTimerManager().SetTimer(TimerHandle, [this, SpawnPoint, InstigatingPC, CartItem]() -> void
					{
						TryPurchasePaint_Auth(InstigatingPC, CartItem.Color, SpawnPoint);
					}, Delay, false);
				// Delay to allow assets to fall down and not cause collision problems
				Delay += 0.5f;
			}
		}
	}
	ClearCart();
}

void AShopState::TryPurchaseWood_Auth_Implementation(AMymPlayerController* InstigatingPC, const FTransform& Location)
{
	AResource* Resource = SpawnResource(WoodBP, InstigatingPC, Location);
}

void AShopState::TryPurchasePaint_Auth_Implementation(AMymPlayerController* InstigatingPC, EColorOption Color, const FTransform& Location)
{
	AResource* Resource = SpawnResource(PaintBP, InstigatingPC, Location);
	if (!Resource) return;

	if (auto ColorVal = ColorPalette->ColorPalette.Find(Color))
	{
		auto PaintMaterialInstance = Resource->GetMainMesh()->CreateDynamicMaterialInstance(0, PaintMaterial);
		PaintMaterialInstance->SetVectorParameterValue(FName("Color"), *ColorVal);
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Color not set up in data asset")));
	}
}

void AShopState::OnOrderCreated_Multicast_Implementation(const FOrderData& Order)
{
	UKismetSystemLibrary::PrintString(this, FString("New Order Arrived!"),
	true, true, FLinearColor::Green, 5.f);
}

void AShopState::OnRep_ShopBudget()
{
	HandleBudgetUpdated();
}

void AShopState::HandleBudgetUpdated() const
{
	if (OnBudgetUpdated.IsBound())
		OnBudgetUpdated.Broadcast(ShopBudget);
}

int32 AShopState::GetResourceCost(TSubclassOf<AActor> ResourceBP)
{
	if (const AResource* DefaultResourceInstance = ResourceBP->GetDefaultObject<AResource>())
	{
		return DefaultResourceInstance->Cost;
	}
	return 0;
}

AResource* AShopState::SpawnResource(const TSubclassOf<AActor>& ResourceBP, AMymPlayerController* InstigatingPC, const FTransform& Location)
{
	int Cost = GetResourceCost(ResourceBP);
	if (Cost <= 0)
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Cost not set up")));
		return nullptr;
	}
	if (ShopBudget < Cost)
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Cannot buy, insufficient funds")));
		return nullptr;
	}
	ShopBudget -= Cost;
	HandleBudgetUpdated();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = InstigatingPC;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	return GetWorld()->SpawnActor<AResource>(ResourceBP, Location, SpawnParams);
}


void AShopState::OnRep_CurrentCart()
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Cart Updated")));
	HandleCartUpdated();
}

void AShopState::HandleCartUpdated() const
{
	if (OnCartUpdated.IsBound())
		OnCartUpdated.Broadcast(CurrentCart);
}

void AShopState::ClearCart()
{
	CurrentCart.Empty();
	if (HasAuthority() && GetNetMode() == NM_ListenServer)
		HandleCartUpdated();
}
