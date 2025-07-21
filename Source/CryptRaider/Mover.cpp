#include "Mover.h"

#include <Math/UnrealMathUtility.h>

UMover::UMover()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMover::BeginPlay()
{
	Super::BeginPlay();

    StartLocation = GetOwner()->GetActorLocation();
	TargetLocation = StartLocation + MoveOffset;
}

void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* owner = GetOwner();
	auto const currentLocation = owner->GetActorLocation();
	auto const speed = MoveOffset.Length() / MoveTime;
	auto const& desiredEndLocation = ShouldMove ? TargetLocation : StartLocation;
    auto const newLocation = FMath::VInterpConstantTo(currentLocation, desiredEndLocation, DeltaTime, speed);
	owner->SetActorLocation(newLocation);
}

void UMover::SetShouldMove(bool newShouldMove)
{
	ShouldMove = newShouldMove;
}
