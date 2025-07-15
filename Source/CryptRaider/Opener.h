// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Opener.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UOpener : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpener();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void TryOpen();

	UPROPERTY(EditAnywhere)
	float MaxOpenDistance{300.};

	UPROPERTY(EditAnywhere)
	float OpenRadius{30.};

private:
	bool TryHit(FHitResult& hitResult) const;
};
