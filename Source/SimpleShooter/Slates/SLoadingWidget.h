// Duncan Keenan-Smith 2020

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

/**
 * 
 */
struct FSimpleShooterLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject
{
	FSimpleShooterLoadingScreenBrush(const FName InTextureName, const FVector2D& InImageSize)
		: FSlateDynamicImageBrush(InTextureName, InImageSize)
	{
		SetResourceObject(LoadObject<UObject>( NULL, *InTextureName.ToString()));
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector)
	{
		FSlateBrush::AddReferencedObjects(Collector);
	}
};

class SLoadingWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLoadingWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	/** loading screen image brush */
	TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
};
