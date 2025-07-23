#include "Interactable.h"

UInteractable::UInteractable()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractable::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInteractable::DoInteract()
{
	// do nothing, to be implemented in child classes
}