// © 2024 Marco Silva. All Rights Reserved.


#include "Components/InstigatorBufferComponent.h"
#include "Algo/Reverse.h"

// Sets default values for this component's properties
UInstigatorBufferComponent::UInstigatorBufferComponent() :
	MaxInstigators(-1)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInstigatorBufferComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInstigatorBufferComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInstigatorBufferComponent::PushInstigator(APlayerStateCatalyst* NewInstigator)
{
	int Index;
	if (Buffer.Find(NewInstigator, Index))
	{
		if (Index != Buffer.Num() - 1)
		{
			if (Buffer.Remove(NewInstigator) > 0)
			{
				Buffer.Add(NewInstigator);
			}
		}
	}
	else
	{
		Buffer.Add(NewInstigator);
	}
	
	if (MaxInstigators > 0)
	{
		int32 Num = Buffer.Num();
		if (Num > MaxInstigators)
		{
			Algo::Reverse(Buffer);
			int32 NumToRemove = Num - MaxInstigators;
			while (NumToRemove > 0)
			{
				Buffer.RemoveSingle(Buffer.Last(0));
				NumToRemove--;
			}
			Algo::Reverse(Buffer);
		}
	}
}

TArray<APlayerStateCatalyst*> UInstigatorBufferComponent::GetInstigators()
{
	auto Temp = Buffer;
	Algo::Reverse(Temp);
	return Temp;
}

APlayerStateCatalyst* UInstigatorBufferComponent::GetMainInstigator()
{
	if (Buffer.IsEmpty())
		return nullptr;
	return Buffer.Last(0);
}

