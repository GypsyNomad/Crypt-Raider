// Fill out your copyright notice in the Description page of Project Settings.


#include "Openable.h"
#include <cmath>

// Sets default values for this component's properties
UOpenable::UOpenable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
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

// Called every frame
void UOpenable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (RotationRemaining.IsNearlyZero(0.0001))
	{
		// skip tick function
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

	// UE_LOG(LogTemp, Display, TEXT("current  p %f y %f r %f"), currentRotation.Pitch, currentRotation.Yaw, currentRotation.Roll);
	// UE_LOG(LogTemp, Display, TEXT("newRotation p %f y %f r %f"), newRotation.Pitch, newRotation.Yaw, newRotation.Roll);
	// UE_LOG(LogTemp, Display, TEXT("step     p %f y %f r %f"), step.Pitch, step.Yaw, step.Roll);
	// UE_LOG(LogTemp, Display, TEXT("smoothedStep p %f y %f r %f"), smoothedStep.Pitch, smoothedStep.Yaw, smoothedStep.Roll);
	// UE_LOG(LogTemp, Display, TEXT("."));

	std::int8_t const rotationDirection = IsOpen ? 1 : -1;
	Rotatable->AddRelativeRotation(smoothedStep * rotationDirection);
	RotationRemaining -= smoothedStep;

	if (RotationRemaining.IsNearlyZero(0.0001))
	{
		// finalize rotation
		RotationRemaining = FRotator::ZeroRotator;
		IsOpen = !IsOpen;

		namespace ch = std::chrono;
		auto const endT = ch::steady_clock::now();
		auto const dur = endT.time_since_epoch() - start.time_since_epoch();
		auto const sec = ch::duration_cast<ch::milliseconds>(dur);
		UE_LOG(LogTemp, Display, TEXT("Passed time: %d ms"), sec.count());
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
	start = std::chrono::steady_clock::now();
}

UStaticMeshComponent const* const UOpenable::GetRotatable() const
{
	return Rotatable;
}

void UOpenable::SetRotatable(UStaticMeshComponent* newRotatable)
{
	Rotatable = newRotatable;
}