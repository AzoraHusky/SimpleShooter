// Duncan Keenan-Smith 2020

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_MoveToRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTService_MoveToRandomLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_MoveToRandomLocation();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
