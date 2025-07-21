#include "Openable.h"
#include <cmath>

UOpenable::UOpenable()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOpenable::BeginPlay()
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

void UOpenable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (RotationRemaining.IsNearlyZero(0.0001))
	{
		return;
	}

	if (!Rotatable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Rotatable object must be set!"));
		RotationRemaining = FRotator::ZeroRotator;
		return;
	}

	auto const currentRotation = RotationTotal - RotationRemaining;
	auto const newRotation = GetClampedProgress(currentRotation, RotationTotal, RotateSpeed * DeltaTime);
	auto const step = newRotation - currentRotation;
	auto const smoothedStep = FMath::RInterpTo(FRotator::ZeroRotator, step, DeltaTime, 100.);

	std::int8_t const rotationDirection = IsOpen ? 1 : -1;
	Rotatable->AddRelativeRotation(smoothedStep * rotationDirection);
	RotationRemaining -= smoothedStep;

	if (RotationRemaining.IsNearlyZero(0.0001))
	{
		// finalize rotation
		RotationRemaining = FRotator::ZeroRotator;
		IsOpen = !IsOpen;
	}
}

void UOpenable::DoOpen()
{
	if (!RotationRemaining.IsNearlyZero(0.0001))
	{
		IsOpen = !IsOpen;
		RotationRemaining = RotationTotal - RotationRemaining;
		return;
	}

	RotationRemaining = RotationTotal;
}

UStaticMeshComponent const* const UOpenable::GetRotatable() const
{
	return Rotatable;
}

void UOpenable::SetRotatable(UStaticMeshComponent* newRotatable)
{
	Rotatable = newRotatable;
}