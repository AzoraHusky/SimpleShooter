// Duncan Keenan-Smith 2020


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner) 
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    HUD->RemoveFromViewport();
    if (bIsWinner)
    {
        UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
        if (WinScreen != nullptr)
        {
            WinScreen->AddToViewport();
        }
    }
    else
    {
        UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
        if (LoseScreen != nullptr)
        {
            LoseScreen->AddToViewport();
        }
    }
    
    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

void AShooterPlayerController::UnPauseGame() 
{
	if (PauseWidget != nullptr)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
        bShowMouseCursor = false;
        SetInputMode(FInputModeGameOnly());
		PauseWidget->RemoveFromParent();
	}
}

void AShooterPlayerController::BeginPlay() 
{
    Super::BeginPlay();

    HUD = CreateWidget(this, HUDClass);
    if (HUD != nullptr)
    {
        HUD->AddToViewport();
    }
}

void AShooterPlayerController::PauseGame() 
{
	PauseWidget = CreateWidget(this, PauseWidgetClass);
	if (PauseWidget != nullptr)
	{
		PauseWidget->AddToViewport();
        FInputModeUIOnly InputModeUI;
        InputModeUI.SetWidgetToFocus(PauseWidget->TakeWidget());
        InputModeUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        SetInputMode(InputModeUI);
		UGameplayStatics::SetGamePaused(GetWorld(), true);
        bShowMouseCursor = true;
	}
}
