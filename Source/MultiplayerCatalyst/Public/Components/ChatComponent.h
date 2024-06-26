// © 2024 Marco Silva. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChatComponent.generated.h"


USTRUCT(BlueprintType)
struct FChatMessageComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat Message Component")
	APlayerState* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat Message Component")
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat Message Component")
	uint8 Team;
};

USTRUCT(BlueprintType)
struct FChatMessage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat Message")
	bool bIsServerMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat Message")
	TArray<FChatMessageComponent> MessageComponents;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageReceivedDelegate, FChatMessage, ChatMessage);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERCATALYST_API UChatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Chat")
	void SendChatMessage(FChatMessage ChatMessage);

	UPROPERTY(EditAnywhere, Category = "Chat")
	uint8 ChatID;

	UPROPERTY(BlueprintAssignable, Category = "Chat")
	FOnMessageReceivedDelegate OnMessageReceived;

	UFUNCTION(Client, Reliable)
	void Client_ReceiveMessage(FChatMessage ChatMessage);
	void Client_ReceiveMessage_Implementation(FChatMessage ChatMessage);

private:
	UFUNCTION(Server, Reliable)
	void Server_SendChatMessage(FChatMessage FChatMessageHandle);
	void Server_SendChatMessage_Implementation(FChatMessage ChatMessageHandle);
	bool Server_SendChatMessage_Validate(FChatMessage ChatMessageHandle);


};
