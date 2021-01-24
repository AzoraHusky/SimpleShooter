// Duncan Keenan-Smith 2020


#include "AmmoActor.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Internationalization/Text.h"

// Sets default values
AAmmoActor::AAmmoActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	RootComponent = CapsuleComponent;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Hover Text"));
	TextComponent->SetupAttachment(BaseMesh);

}

int AAmmoActor::GetAmmo() 
{
	return Ammo;
}

void AAmmoActor::SetAmmo(int Amount) 
{
	Ammo = Amount;
	TextComponent->SetText(FText::Format(FText::FromString(TEXT("Ammo: {0}")), FText::AsNumber(Ammo)));
}

UCapsuleComponent* AAmmoActor::GetCapsuleComponent() 
{
	return CapsuleComponent;
}

// Called when the game starts or when spawned
void AAmmoActor::BeginPlay()
{
	Super::BeginPlay();
	
}

