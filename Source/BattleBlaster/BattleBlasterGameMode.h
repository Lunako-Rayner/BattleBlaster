// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Tank.h"
#include "ScreenMessage.h"

#include "BattleBlasterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ABattleBlasterGameMode : public AGameModeBase
{
	GENERATED_BODY()


protected:
	
	virtual void BeginPlay() override;

public: 

	UPROPERTY(EditAnywhere)
	TSubclassOf<UScreenMessage> ScreenMessageClass; //TSubclassOf is a type (defining what something is) not running anything thats why there aren't ().

	UScreenMessage* ScreenMessageWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UScreenMessage> HealthMessageClass;

	UScreenMessage* HealthMessageWidget; 

	UPROPERTY(EditAnywhere)
	float GameOverDelay = 3.0f;

	UPROPERTY(EditAnywhere)
	int32 CountdownDelay = 3;// number of seconds we're going to wait

	int32 CountdownSeconds;//actual variable that keeps track of the countdown

	FTimerHandle CountdownTimerHandle;

	bool IsVictory = false;

	ATank* Tank;
	int32 TowerCount;

	void ActorDied(AActor* DeadActor);

	void OnGameOverTimerTimeout(); 

	void OnCountdownTimerTimeout();


	void RefreshHealthWidget();


	
};
