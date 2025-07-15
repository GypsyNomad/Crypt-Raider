// Fill out your copyright notice in the Description page of Project Settings.


#include "Openable.h"
#include "DrawDebugHelpers.h"

#include "Opener.h"

// Sets default values for this component's properties
UOpener::UOpener()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpener::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UOpener::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UOpener::TryOpen()
{
	FHitResult hitRes{};
	auto hasHit = TryHit(hitRes);

	if (hasHit)
	{
		if (auto* const actor = hitRes.GetActor(); actor)
		{
			auto* const component = Cast<UStaticMeshComponent>(hitRes.GetComponent());

			TArray<UOpenable*> openables{};
			actor->GetComponents(openables);
			for (auto* openable : openables)
			{
				if (openable && component == openable->GetRotatable())
				{
					openable->DoOpen();
				}
			}
		}
	}
}

bool UOpener::TryHit(FHitResult& hitResult) const
{
	auto const start = GetComponentLocation();
	auto const end = start + GetForwardVector() * MaxOpenDistance;

	FCollisionShape sphere{FCollisionShape::MakeSphere(OpenRadius)};
	return GetWorld()->SweepSingleByChannel(
		hitResult,
		start,
		end,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		sphere);
}