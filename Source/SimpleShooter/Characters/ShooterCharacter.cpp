// Duncan Keenan-Smith 2020

#include "ShooterCharacter.h"
#include "Components/InputComponent.h"
#include "SimpleShooter/Weapons/Gun.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooter/SimpleShooterGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SimpleShooter/Characters/ShooterPlayerController.h"
#include "SimpleShooter/Actors/AmmoActor.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AShooterCharacter::BeginOverlap);
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	
	for (int GunIndex = 0; GunIndex < GunClasses.Num(); GunIndex++)
	{
		Guns.Add(GetWorld()->SpawnActor<AGun>(GunClasses[GunIndex]));
		GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
		Guns[GunIndex]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Guns[GunIndex]->SetOwner(this);
		Guns[GunIndex]->SetActorHiddenInGame(true);
	}

	Guns[ActiveIndex]->SetActorHiddenInGame(false);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0.f;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

int AShooterCharacter::GetAmmoAmmount() const
{
	return Guns[ActiveIndex]->GetAmmo();
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AShooterCharacter::Shoot);
	
	PlayerInputComponent->BindAxis(TEXT("SwapWeapons"), this, &AShooterCharacter::SwapWeapons);
	PlayerInputComponent->BindAction(TEXT("Pause"), IE_Pressed, this, &AShooterCharacter::PauseGame);
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) 
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageApplied = FMath::Min(Health, DamageApplied);
	Health -= DamageApplied;
	if (GetOwner() != nullptr)
		UE_LOG(LogTemp, Warning, TEXT("%s's health is at %f!"), *GetOwner()->GetName(), Health);

	if (IsDead() && GetOwner() != nullptr)
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (AmmoDropClass != NULL)
		{
			AAmmoActor* AmmoPickup = GetWorld()->SpawnActor<AAmmoActor>(AmmoDropClass);
			if (AmmoPickup != nullptr)
			{
				FVector SpawnLocation = GetActorLocation() - FVector::FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
					 - AmmoPickup->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
				AmmoPickup->SetActorLocation(SpawnLocation);
				AmmoPickup->SetAmmo(FMath::RandRange(5, 20));
			}
		}
	}

	return DamageApplied;
}

void AShooterCharacter::MoveForward(float AxisValue) 
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue) 
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue) 
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->DeltaTimeSeconds);
}

void AShooterCharacter::LookRightRate(float AxisValue) 
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->DeltaTimeSeconds);
}

void AShooterCharacter::PauseGame() 
{
	AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(GetController());
	PlayerController->PauseGame();
}

void AShooterCharacter::Shoot() 
{
	Guns[ActiveIndex]->PullTrigger();
}

void AShooterCharacter::SwapWeapons(float SwapIndex) 
{
	if (SwapIndex != 0) 
	{
		Guns[ActiveIndex]->SetActorHiddenInGame(true);

		ActiveIndex += SwapIndex;
		if (ActiveIndex > GunClasses.Num() - 1) ActiveIndex = 0;
		else if (ActiveIndex < 0) ActiveIndex = GunClasses.Num() - 1;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponSwitchSound, GetActorLocation());

		Guns[ActiveIndex]->SetActorHiddenInGame(false);
	}
}

void AShooterCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, 
					  AActor* OtherActor, 
					  UPrimitiveComponent* OtherComp, 
					  int32 OtherBodyIndex, 
					  bool bFromSweep, 
					  const FHitResult &SweepResult) 
{
	AAmmoActor* AmmoActor = Cast<AAmmoActor>(OtherActor);
	if (AmmoActor != nullptr)
	{
		Guns[ActiveIndex]->AddAmmo(AmmoActor->GetAmmo());
		AmmoActor->Destroy();
	}
}