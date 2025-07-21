#include "TriggerComponent.h"
#include <algorithm>

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!mover)
    {
        UE_LOG(LogTemp, Warning, TEXT("Mover isn't set!"));
        return;
    }

    auto* actor = GetAcceptableActor();
    if (actor)
    {
        if (IsAcceptedActorMoveable)
        {
            auto newLocation = GetComponentTransform().GetLocation();
            newLocation.Z -= Bounds.GetBox().GetSize().Z/2;
            actor->SetActorLocation(newLocation);
            // Reset actor rotation to face the player
            actor->SetActorRotation(FRotator{});
        }

        auto* primitive = Cast<UPrimitiveComponent>(actor->GetRootComponent());
        if (primitive)
        {
            primitive->SetSimulatePhysics(false);
        }
        actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        mover->SetShouldMove(true);
    }
    else
    {
        mover->SetShouldMove(false);
    }
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
    TArray<AActor*> actors{};
    GetOverlappingActors(actors);

    for (auto* actor : actors)
    {
        auto const noUnacceptableTags = !std::any_of(UnacceptableActorTags.begin(), UnacceptableActorTags.end(),
            [&](FName const& tag)
            {
                return actor->ActorHasTag(tag);
            });
        if (actor->ActorHasTag(AcceptableActorTag) && noUnacceptableTags)
        {
            return actor;
        }
    }

    return nullptr;
}

void UTriggerComponent::SetMover(UMover* newMover)
{
    mover = newMover;
}