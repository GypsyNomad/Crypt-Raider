// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TriggerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()
	
public:
	UTriggerComponent();

protected:
	void BeginPlay() override;

public:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* newMover);

	UPROPERTY(EditAnywhere)
	FName AcceptableActorTag{};

	UPROPERTY(EditAnywhere)
	TArray<FName> UnacceptableActorTags{};

	UPROPERTY(EditAnywhere)
	bool IsAcceptedActorMoveable{false};

private:
	AActor* GetAcceptableActor() const;

	UMover* mover{nullptr};
};
