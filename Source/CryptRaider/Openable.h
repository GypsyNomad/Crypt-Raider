// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <chrono>

#include "Openable.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UOpenable : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenable();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void DoOpen();

	UStaticMeshComponent const* const GetRotatable() const;

	UFUNCTION(BlueprintCallable)
	void SetRotatable(UStaticMeshComponent* newRotatable);

	UPROPERTY(EditAnywhere)
	bool IsOpen{false};
	
	UPROPERTY(EditAnywhere)
	double RotateTime{5.};
	
	UPROPERTY(EditAnywhere)
	FRotator RotateSpeed{0,20,0};

private:

	UStaticMeshComponent* Rotatable{nullptr};
	FRotator RotationRemaining{};
	FRotator RotationTotal{};

	std::chrono::steady_clock::time_point start{};
};
