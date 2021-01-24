	// Duncan Keenan-Smith 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoActor.generated.h"

class UCapsuleComponent;
class UTextRenderComponent;

UCLASS()
class SIMPLESHOOTER_API AAmmoActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoActor();

	UFUNCTION(BlueprintPure)
	int GetAmmo();

	void SetAmmo(int Amount);

	UCapsuleComponent* GetCapsuleComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* TextComponent;

	UPROPERTY(EditAnywhere)
	int Ammo = 30;
};
