#include "OrderSheet.h"

#include "OrderDataAsset.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "MYM/Interaction/GrabInteractionComponent.h"

// Sets default values
AOrderSheet::AOrderSheet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(PaperMesh);
	
	PaperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PaperMesh"));
	PaperMesh->SetupAttachment(RootScene);
	
	ShapeText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ShapeText"));
	ColorText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ColorText"));
	ChargeText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ChargeText"));
	GrabInteraction = CreateDefaultSubobject<UGrabInteractionComponent>(TEXT("GrabInteraction"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	
	ShapeText->SetupAttachment(PaperMesh);
	ColorText->SetupAttachment(PaperMesh);
	ChargeText->SetupAttachment(PaperMesh);
	GrabInteraction->SetupAttachment(PaperMesh);
	BoxCollision->SetupAttachment(PaperMesh);
}

// Called when the game starts or when spawned
void AOrderSheet::BeginPlay()
{
	Super::BeginPlay();
}

void AOrderSheet::InitOrder(const FOrderData& OrderData) const
{
	ShapeText->SetText(FText::FromString(FString(TEXT("Shape: ")) + OrderData.ItemName.ToString()));
	ColorText->SetText(FText::FromString(FString::Printf(TEXT("Color: %s"), *UColorPaletteDataAsset::GetColorOptionString(OrderData.Color))));
	ChargeText->SetText(FText::FromString(FString(TEXT("Charge: ")) + FString::FromInt(OrderData.Charge)));
}

