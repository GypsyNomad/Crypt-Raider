// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "OpenableScene.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UOpenableScene : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenableScene();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void DoOpen();

private:
	bool isOpen{false};
	bool isMoving{false};

	double RotateDegree{90.};
	double RotateTime{5.};
	FRotator RotateSpeed{0,20,0};

	FRotator StartRotation{};
	FRotator EndRotation{};
};
