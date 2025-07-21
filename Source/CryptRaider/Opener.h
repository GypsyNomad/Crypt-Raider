#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Opener.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UOpener : public USceneComponent
{
	GENERATED_BODY()

public:	
	UOpener();

protected:
	virtual void BeginPlay() override;

public:
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
