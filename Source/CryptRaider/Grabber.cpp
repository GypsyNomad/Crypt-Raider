// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "Engine/World.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}

bool UGrabber::TryHit(FHitResult& hitResult) const
{
	auto const start = GetComponentLocation();
	auto const end = start + GetForwardVector() * MaxGrabDistance;

	FCollisionShape sphere{FCollisionShape::MakeSphere(GrabRadius)};
	return GetWorld()->SweepSingleByChannel(
		hitResult,
		start,
		end,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		sphere);
}

void UGrabber::Grab()
{
	auto* physicsHandle = GetPhysicsHandle();
	if (nullptr == physicsHandle)
	{
		return;
	}

	FHitResult hitRes{};
	auto const hasHit = TryHit(hitRes);

	if (hasHit)
	{
		if (auto* grabbedComponent = hitRes.GetComponent();
			grabbedComponent)
		{
			grabbedComponent->SetSimulatePhysics(true);
			grabbedComponent->WakeAllRigidBodies();
			if (auto* grabbedActor = hitRes.GetActor();
				grabbedActor)
			{
				grabbedActor->Tags.Add(GrabbedTag);
				grabbedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			}
			physicsHandle->GrabComponentAtLocationWithRotation(
				grabbedComponent,
				NAME_None,
				hitRes.ImpactPoint,
				GetComponentRotation());
		}

		auto const hitName = hitRes.GetActor()->GetActorNameOrLabel();
	}
}

void UGrabber::Release()
{
	auto* physicsHandle = GetPhysicsHandle();
	if (physicsHandle)
	{
		if (auto* grabbedComponent = physicsHandle->GetGrabbedComponent();
			grabbedComponent)
		{
			grabbedComponent->WakeAllRigidBodies();
			physicsHandle->ReleaseComponent();
			if (auto* grabbedActor = grabbedComponent->GetOwner();
				grabbedActor)
			{
				grabbedActor->Tags.Remove(GrabbedTag);
			}
		}
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	auto* physicsHandle = GetPhysicsHandle();
	if (physicsHandle && physicsHandle->GetGrabbedComponent())
	{
		auto targetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		physicsHandle->SetTargetLocationAndRotation(targetLocation, GetComponentRotation());
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	auto* handle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (nullptr == handle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Physics handle is required!"));
	}
	return handle;
}