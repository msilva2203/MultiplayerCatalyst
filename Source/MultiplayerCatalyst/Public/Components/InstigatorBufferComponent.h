// © 2024 Marco Silva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStateCatalyst.h"
#include "InstigatorBufferComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERCATALYST_API UInstigatorBufferComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInstigatorBufferComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Instigator Buffer")
	int32 MaxInstigators;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Instigator Buffer")
	void PushInstigator(APlayerStateCatalyst* NewInstigator);

	UFUNCTION(BlueprintCallable, Category = "Instigator Buffer")
	TArray<APlayerStateCatalyst*> GetInstigators();

	UFUNCTION(BlueprintCallable, Category = "Instigator Buffer")
	APlayerStateCatalyst* GetMainInstigator();
	
private:
	UPROPERTY()
	TArray<APlayerStateCatalyst*> Buffer;
};
