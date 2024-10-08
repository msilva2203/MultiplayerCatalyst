// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Components/ScalarAttributeComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UScalarAttributeComponent::UScalarAttributeComponent() :
	DefaultValue(100.0f),
	MinValue(0.0f),
	MaxValue(100.0f),
	bUseMinValue(false),
	bUseMaxValue(false),
	bAllowReplicationByPlayers(true),
	bUseMulticasting(false),
	AttributeReplicationCondition(COND_None),
	bStartLocked(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	// ...
}

void UScalarAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME_CONDITION(UScalarAttributeComponent, AttributeValue, AttributeReplicationCondition);
	DOREPLIFETIME(UScalarAttributeComponent, AttributeValue)
	DOREPLIFETIME(UScalarAttributeComponent, bLocked)
}


// Called when the game starts
void UScalarAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	AttributeValue = DefaultValue;
	PreviousValue = DefaultValue;
	bLocked = bStartLocked;

	if (!IsLocked())
		OnRep_AttributeValue();
	OnRep_bLocked();
}


// Called every frame
void UScalarAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UScalarAttributeComponent::SetAttributeValue(float& UpdatedValue, float& Overflow, float NewValue, bool bForceReplication)
{
	Overflow = 0;

	if (!IsLocked())
	{
		// Clamp with min value
		if (bUseMinValue)
		{
			if (NewValue <= MinValue)
			{
				Overflow = NewValue - MinValue;
				NewValue = MinValue;

				OnMinValueReached.Broadcast(Overflow);
				OnOverflow.Broadcast(Overflow);
			}
		}

		// Clamp with max value
		if (bUseMaxValue)
		{
			if (NewValue >= MaxValue)
			{
				Overflow = NewValue - MaxValue;
				NewValue = MaxValue;

				OnMaxValueReached.Broadcast(Overflow);
				OnOverflow.Broadcast(Overflow);
			}
		}

		if (AttributeValue != NewValue)
		{
			AttributeValue = NewValue;
			if (bForceReplication && bAllowReplicationByPlayers && !(GetOwner()->HasAuthority()))
			{
				Server_SetAttributeValue(AttributeValue);
			}
			GetOwner()->ForceNetUpdate();
			OnRep_AttributeValue();
		}
	}

	UpdatedValue = AttributeValue;
}

void UScalarAttributeComponent::OffsetAttributeValue(float& UpdatedValue, float& Overflow, float Offset)
{
	SetAttributeValue(UpdatedValue, Overflow, AttributeValue + Offset);
}

void UScalarAttributeComponent::ResetAttribute()
{
	float A, B;
	SetAttributeValue(A, B, DefaultValue);
}

void UScalarAttributeComponent::SetLock(bool bNewValue)
{
	bLocked = bNewValue;

	GetOwner()->ForceNetUpdate();
	OnRep_bLocked();
}

void UScalarAttributeComponent::ForceNewValue(float NewValue)
{
	Server_SetAttributeValue(NewValue);
}

float UScalarAttributeComponent::GetAttributeValue() const
{
	return AttributeValue;
}

bool UScalarAttributeComponent::IsLocked() const
{
	return bLocked;
}

void UScalarAttributeComponent::Server_SetAttributeValue_Implementation(float NewValue)
{
	float A, B;
	SetAttributeValue(A, B, NewValue);
	if (bUseMulticasting)
	{
		NetAll_SetAttributeValue(NewValue);
	}
}

bool UScalarAttributeComponent::Server_SetAttributeValue_Validate(float NewValue)
{
	return true;
}

void UScalarAttributeComponent::NetAll_SetAttributeValue_Implementation(float NewValue)
{
	float A, B;
	if (!GetOwner()->HasAuthority())
		SetAttributeValue(A, B, NewValue);
}

void UScalarAttributeComponent::OnRep_AttributeValue()
{
	OnScalarAttributeValueChanged.Broadcast(AttributeValue, PreviousValue);
	PreviousValue = AttributeValue;
}

void UScalarAttributeComponent::OnRep_bLocked()
{
	if (IsLocked())
	{
		OnScalarAttributeLocked.Broadcast();
	}
	else
	{
		OnScalarAttributeUnlocked.Broadcast();
	}
}

