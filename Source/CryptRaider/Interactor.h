#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Interactor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UInteractor : public USceneComponent
{
	GENERATED_BODY()

public:
	UInteractor();

protected:
	void BeginPlay() override;

public:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void TryInteract();

	UPROPERTY(EditAnywhere)
	float MaxInteractDistance{300.};

	UPROPERTY(EditAnywhere)
	float InteractRadius{30.};

private:
	bool TryHit(FHitResult& hitResult) const;
};
