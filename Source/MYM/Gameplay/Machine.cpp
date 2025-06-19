#include "Machine.h"
#include "Components/TextRenderComponent.h"
#include "MYM/Interaction/InteractionComponent.h"

AMachine::AMachine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MachineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MachineMesh"));
	SetRootComponent(MachineMesh);

	ButtonSocket1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonSocket1"));
	ButtonSocket1->SetupAttachment(MachineMesh);
	ButtonSocket2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonSocket2"));
	ButtonSocket2->SetupAttachment(MachineMesh);
	ButtonSocket3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonSocket3"));
	ButtonSocket3->SetupAttachment(MachineMesh);
	ButtonSocketDone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonSocketDone"));	
	ButtonSocketDone->SetupAttachment(MachineMesh);
	
	ButtonMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh1"));
	ButtonMesh1->SetupAttachment(ButtonSocket1);
	ButtonMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh2"));
	ButtonMesh2->SetupAttachment(ButtonSocket2);
	ButtonMesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh3"));
	ButtonMesh3->SetupAttachment(ButtonSocket3);	
	ButtonMeshDone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMeshDone"));
	ButtonMeshDone->SetupAttachment(ButtonSocketDone);

	Interaction1 = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction1"));
	Interaction1->SetupAttachment(ButtonSocket1);	
	Interaction2 = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction2"));
	Interaction2->SetupAttachment(ButtonSocket2);
	Interaction3 = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction3"));
	Interaction3->SetupAttachment(ButtonSocket3);
	InteractionDone = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionDone"));
	InteractionDone->SetupAttachment(ButtonSocketDone);

	Interaction1->OwnedPrimitives.Add(ButtonMesh1);
	Interaction2->OwnedPrimitives.Add(ButtonMesh2);
	Interaction3->OwnedPrimitives.Add(ButtonMesh3);
	InteractionDone->OwnedPrimitives.Add(ButtonMeshDone);
	
	Text1 = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text1"));
	Text1->SetupAttachment(ButtonSocket1);
	Text2 = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text2"));
	Text2->SetupAttachment(ButtonSocket2);
	Text3 = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text3"));
	Text3->SetupAttachment(ButtonSocket3);
	TextDone = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextDone"));
	TextDone->SetupAttachment(ButtonSocketDone);
}

// Called when the game starts or when spawned
void AMachine::BeginPlay()
{
	Super::BeginPlay();

	Interaction1->OnInteractBegin.AddDynamic(this, &AMachine::OnInteract1Begin_Event);
	Interaction2->OnInteractBegin.AddDynamic(this, &AMachine::OnInteract2Begin_Event);
	Interaction3->OnInteractBegin.AddDynamic(this, &AMachine::OnInteract3Begin_Event);
	InteractionDone->OnInteractBegin.AddDynamic(this, &AMachine::OnInteractDoneBegin_Event);
}

void AMachine::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	Interaction1->OnInteractBegin.RemoveDynamic(this, &AMachine::OnInteract1Begin_Event);
	Interaction2->OnInteractBegin.RemoveDynamic(this, &AMachine::OnInteract2Begin_Event);
	Interaction3->OnInteractBegin.RemoveDynamic(this, &AMachine::OnInteract3Begin_Event);
	InteractionDone->OnInteractBegin.RemoveDynamic(this, &AMachine::OnInteractDoneBegin_Event);
}

