// Fill out your copyright notice in the Description page of Project Settings.

#include <algorithm>
#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
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
        // UE_LOG(LogTemp, Warning, TEXT("actor is accepted!"));
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
        // UE_LOG(LogTemp, Warning, TEXT("actor is missed!"));
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