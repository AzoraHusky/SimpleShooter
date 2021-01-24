// Duncan Keenan-Smith 2020


#include "BTService_MoveToRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Math/UnrealMathUtility.h"


UBTService_MoveToRandomLocation::UBTService_MoveToRandomLocation() 
{
    NodeName = TEXT("Move to Random Location");
}

void UBTService_MoveToRandomLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    FVector AILocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
    FVector MoveToLocation = AILocation + FVector::FVector(FMath::RandRange(-300.f, 300.f), 
        FMath::RandRange(-300.f, 300.f), 0);

    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), MoveToLocation);
}
