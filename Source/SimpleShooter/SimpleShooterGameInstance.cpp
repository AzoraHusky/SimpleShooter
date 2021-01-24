// Duncan Keenan-Smith 2020


#include "SimpleShooterGameInstance.h"
#include "MoviePlayer.h"
#include "SimpleShooter/Slates/SLoadingWidget.h"
#include "Widgets/SWeakWidget.h"


void USimpleShooterGameInstance::Init() 
{
    Super::Init();

    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &USimpleShooterGameInstance::BeginLoadingScreen);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &USimpleShooterGameInstance::EndLoadingScreen);
}

void USimpleShooterGameInstance::BeginLoadingScreen(const FString& MapName) 
{
    if (!IsRunningDedicatedServer())
    {
        FLoadingScreenAttributes LoadingScreen;
        LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
        LoadingScreen.WidgetLoadingScreen = SNew(SLoadingWidget);

        GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
    }
}

void USimpleShooterGameInstance::EndLoadingScreen(UWorld* InLoadedWorld) 
{
    
}
