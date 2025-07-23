#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Rotatable.generated.h"

UCLASS()
class CRYPTRAIDER_API URotatable : public UInteractable
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;

public:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void DoInteract() override;

private:
	UPROPERTY(EditAnywhere)
	double RotateTime{1.};

	UPROPERTY(EditAnywhere)
	FRotator RotateSpeed{0, 45., 0};

	FRotator RotationRemaining{};
	FRotator RotationTotal{};
	bool IsRotating{false};
};
