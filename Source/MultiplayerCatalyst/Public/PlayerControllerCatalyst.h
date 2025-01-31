// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/CatalystInterface.h"
#include "PlayerControllerCatalyst.generated.h"

class UAttributeMapComponent;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUnPossessSignature, APawn*, UnpossessedPawn)

/**
 * 
 */
UCLASS()
class MULTIPLAYERCATALYST_API APlayerControllerCatalyst : public APlayerController, public ICatalystInterface
{
	GENERATED_BODY()
	
public:
	APlayerControllerCatalyst();

	UFUNCTION(BlueprintImplementableEvent, Category = "Player Controller Catalyst")
	void OnSetupPlayerController();

	UFUNCTION(BlueprintImplementableEvent, Category = "Player Controller Catalyst")
	void OnReceivedPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void ReceivedPlayer() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	virtual void SetupPlayerController();
	virtual void OnRep_PlayerState() override;

	virtual uint8 GetTeam() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	UAttributeMapComponent* AttributeMap;

private:
	UPROPERTY()
	APawn* PreviousPawn;
};
