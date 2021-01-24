// Duncan Keenan-Smith 2020


#include "BTTask_Shoot.h"
#include "AIController.h"
#include "SimpleShooter/Characters/ShooterCharacter.h"


UBTTask_Shoot::UBTTask_Shoot() 
{
    NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;

    AShooterCharacter* Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (Character == nullptr) EBTNodeResult::Failed;

    Character->Shoot();

    return EBTNodeResult::Succeeded;
}
