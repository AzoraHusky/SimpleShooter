// Duncan Keenan-Smith 2020


#include "SLoadingWidget.h"

#define LOCTEXT_NAMESPACE "LoadingScreen"

void SLoadingWidget::Construct(const FArguments& InArgs) 
{
    const FMargin ContentPadding = FMargin(1000.f, 300.f);

    static const FName LoadingScreenName(TEXT("/Game/Textures/Menu/LoadingScreen.LoadingScreen"));

    LoadingScreenBrush = MakeShareable(new FSimpleShooterLoadingScreenBrush(LoadingScreenName, FVector2D(1920,1080)));

    const FText TitleText = LOCTEXT("LoadingText", "Loading...");

    FSlateFontInfo TextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
    TextStyle.Size = 20.f;

    ChildSlot
        [
            SNew(SOverlay)
            + SOverlay::Slot()
            .VAlign(VAlign_Fill)
            .HAlign(HAlign_Fill)
            [
                SNew(SImage)
                .Image(LoadingScreenBrush.Get())
            ]
            + SOverlay::Slot()
            .VAlign(VAlign_Bottom)
            .HAlign(HAlign_Fill)
            [
                SNew(SOverlay)  
                + SOverlay::Slot()
                .VAlign(VAlign_Fill)
                .HAlign(HAlign_Fill)
                [
                    SNew(SImage)
                    .ColorAndOpacity(FColor::FColor(0, 0, 0, 128))
                ]  
                + SOverlay::Slot()
                .VAlign(VAlign_Bottom)
                .HAlign(HAlign_Left)
                .Padding(10.f)
                [
                    SNew(STextBlock)
                    .Font(TextStyle)
                    .Text(TitleText)
                    .Justification(ETextJustify::Center)
                ]
                + SOverlay::Slot()
                .VAlign(VAlign_Bottom)
                .HAlign(HAlign_Right)
                .Padding(10.f)
                [
                    SNew(SCircularThrobber)
                ]
            ]
        ];
}

#define LOCTEXT_NAMESPACE