// Copyright (c) 2024, Marco Silva. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/CatalystInterface.h"
#include "CharacterCatalyst.generated.h"

class UAttributeMapComponent;

UCLASS()
class MULTIPLAYERCATALYST_API ACharacterCatalyst : public ACharacter, public ICatalystInterface
{
	GENERATED_BODY()

public:
	ACharacterCatalyst();

	UFUNCTION(BlueprintImplementableEvent, Category = "Character Catalyst")
	void OnSetupCharacter();

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual void BeginPlay() override;
	virtual void NotifyControllerChanged() override;

	virtual void SetupCharacter();
	virtual void SetupCharacterController(AController* OldController, AController* NewController);
	virtual void OnRep_PlayerState() override;

	virtual uint8 GetTeam() override;

public:	

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	UAttributeMapComponent* AttributeMap;

	UPROPERTY()
	bool bCompletedSetup;

private:

	//UPROPERTY(Replicated)
	//float ControlRotationPitch;

};
