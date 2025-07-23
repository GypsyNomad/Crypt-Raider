#include "Interactor.h"

#include "Interactable.h"

UInteractor::UInteractor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractor::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInteractor::TryInteract()
{
	FHitResult hitRes{};
	auto hasHit = TryHit(hitRes);

	if (hasHit)
	{
		if (auto* const actor = hitRes.GetActor(); actor)
		{
			TArray<UInteractable*> interactables{};
			actor->GetComponents(interactables);
			for (auto* interactable : interactables)
			{
				if (interactable)
				{
					interactable->DoInteract();
				}
			}
		}
	}
}

bool UInteractor::TryHit(FHitResult& hitResult) const
{
	auto const start = GetComponentLocation();
	auto const end = start + GetForwardVector() * MaxInteractDistance;

	FCollisionShape sphere{FCollisionShape::MakeSphere(InteractRadius)};
	return GetWorld()->SweepSingleByChannel(
		hitResult,
		start,
		end,
		FQuat::Identity,
		ECC_GameTraceChannel4,
		sphere);
}