// Duncan Keenan-Smith 2020

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SimpleShooterGameInstance.generated.h"
/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API USimpleShooterGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* InLoadedWorld);
	
};
