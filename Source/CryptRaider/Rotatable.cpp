#include "Rotatable.h"

void URotatable::BeginPlay()
{
	Super::BeginPlay();

	RotationTotal = RotateSpeed * RotateTime;
}

namespace
{
	double ClampAxis(double current, double target, double maxStep)
	{
		auto const remaining = target - current;
		auto const step = FMath::Clamp(maxStep, -FMath::Abs(remaining), FMath::Abs(remaining));
		return current + step;
	}

	FRotator GetClampedProgress(FRotator const& current, FRotator const& end, FRotator const& speed)
	{
		FRotator NewProgress;
		NewProgress.Pitch = ClampAxis(current.Pitch, end.Pitch, speed.Pitch);
		NewProgress.Yaw   = ClampAxis(current.Yaw,   end.Yaw,   speed.Yaw);
		NewProgress.Roll  = ClampAxis(current.Roll,  end.Roll,  speed.Roll);
		return NewProgress;
	}
}

void URotatable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (RotationRemaining.IsNearlyZero(0.001))
    {
        return;
    }

    auto const currentRotation = RotationTotal - RotationRemaining;
	auto const newRotation = GetClampedProgress(currentRotation, RotationTotal, RotateSpeed * DeltaTime);
	auto const step = newRotation - currentRotation;
	auto const smoothedStep = FMath::RInterpTo(FRotator::ZeroRotator, step, DeltaTime, 100.);

    auto* owner = GetOwner();
    owner->AddActorLocalRotation(smoothedStep);
	RotationRemaining -= smoothedStep;

	if (RotationRemaining.IsNearlyZero(0.0001))
	{
		// finalize rotation
		RotationRemaining = FRotator::ZeroRotator;
        IsRotating = false;
	}
}

void URotatable::DoInteract()
{
    if (IsRotating)
    {
        return;
    }

    RotationRemaining = RotateSpeed;
    IsRotating = true;
}