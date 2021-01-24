// Duncan Keenan-Smith 2020

#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

#define OUT

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootHandle = CreateDefaultSubobject<USceneComponent>(TEXT("Root Handle"));
	SetRootComponent(RootHandle);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootHandle);
}

void AGun::PullTrigger() 
{
	if (Ammo <= 0)
	{
		UGameplayStatics::SpawnSoundAttached(EmptySound, Mesh, TEXT("MuzzleFlashSocket"));
		return;
	}
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));
	Ammo--;

	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);

	if (bSuccess)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
		
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}

int AGun::GetAmmo() const
{
	return Ammo;
}

void AGun::AddAmmo(int Amount) 
{
	Ammo += Amount;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), AmmoRechargeSound, GetActorLocation());
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection) 
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;

	FVector OwnerLocation;
	FRotator OwnerRotation;

	OwnerController->GetPlayerViewPoint(OUT OwnerLocation, OUT OwnerRotation);
	ShotDirection = -OwnerRotation.Vector();

	FVector EndLocation = OwnerLocation + OwnerRotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(
		OUT Hit,
		OwnerLocation,
		EndLocation,
		ECC_GameTraceChannel1,
		Params
	);
	
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	return OwnerPawn->GetController();
}

