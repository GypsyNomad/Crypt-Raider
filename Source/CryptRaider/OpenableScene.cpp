// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenableScene.h"

// Sets default values for this component's properties
UOpenableScene::UOpenableScene()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenableScene::BeginPlay()
{
	Super::BeginPlay();

	StartRotation = GetOwner()->GetActorRotation();
	EndRotation = StartRotation + RotateSpeed * RotateTime;
	// EndRotation.Yaw += RotateDegree;
	UE_LOG(LogTemp, Display, TEXT("Start Pitch: %f Yaw: %f Roll: %f"), StartRotation.Pitch, StartRotation.Yaw, StartRotation.Roll);
	UE_LOG(LogTemp, Display, TEXT("End   Pitch: %f Yaw: %f Roll: %f"), EndRotation.Pitch, EndRotation.Yaw, EndRotation.Roll);
}


// Called every frame
void UOpenableScene::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!isMoving)
	{
		return;
	}

	AActor* owner = GetOwner();
	auto name = owner->GetActorNameOrLabel();
	UE_LOG(LogTemp, Display, TEXT("tick owner name %s"), *name);
	for (auto const& child : owner->Children)
	{
		UE_LOG(LogTemp, Display, TEXT("tick child %s"), *child->GetActorNameOrLabel());
	}
	auto const currentRotation = owner->GetActorRotation();
	auto const speed = RotateDegree / RotateTime;
	auto const& desiredEndRotation = isOpen ? StartRotation : EndRotation;
	if (currentRotation == desiredEndRotation)
	{
		UE_LOG(LogTemp, Display, TEXT("End rotation"));
		isMoving = false;
		isOpen = !isOpen;
		return;
	}
	
	// auto const rotationDirection = isOpen ? -1 : 1;
	auto const newRotationVec = FMath::VInterpNormalRotationTo(currentRotation.Vector(), desiredEndRotation.Vector(), DeltaTime, speed);
	auto newRotation = FRotator::MakeFromEuler(newRotationVec);
	UE_LOG(LogTemp, Display, TEXT("Current Pitch: %f Yaw: %f Roll: %f"), currentRotation.Pitch, currentRotation.Yaw, currentRotation.Roll);
	// UE_LOG(LogTemp, Display, TEXT("New     Pitch: %f Yaw: %f Roll: %f"), newRotation.Pitch, newRotation.Yaw, newRotation.Roll);
	// newRotation.Yaw += speed * rotationDirection;
	
	UE_LOG(LogTemp, Display, TEXT("Rotating..."));
	// owner->SetActorRotation(newRotation);
	owner->AddActorLocalRotation(RotateSpeed * DeltaTime);
}

void UOpenableScene::DoOpen()
{
	UE_LOG(LogTemp, Display, TEXT("Being opened!"));
	auto name = GetOwner()->GetActorNameOrLabel();
	UE_LOG(LogTemp, Display, TEXT("owner name %s"), *name);

	if (isMoving)
	{
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("Start rotation"));
	isMoving = true;
}