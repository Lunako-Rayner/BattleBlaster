// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "HealthComponent.h"
#include "BattleBlasterGameInstance.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Towers;//First, we create an empty list specifically designed to hold Actors, and we name this list Towers

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);//We then command the game engine to search the entire game world for anything classified as a Tower and put them all into our Towers list
	TowerCount = Towers.Num();//check how many items are on the list. 
	UE_LOG(LogTemp, Display, TEXT("Number of Towers: %d:"), TowerCount);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		Tank = Cast<ATank>(PlayerPawn);
		if (!Tank)
		{
			UE_LOG(LogTemp, Display, TEXT("UNSUCCESSFUL: Failed to finf the tank actor"));
		}
	}


	int32 LoopIndex = 0;
	while (LoopIndex < TowerCount)
	{
		AActor* TowerActor = Towers[LoopIndex];
		if (TowerActor)
		{
			ATower* Tower = Cast<ATower>(TowerActor);
			if (Tower && Tank)//if both are valid
			{
				Tower->Tank = Tank;//left side tank is empty , right side is the local one from line 20
				UE_LOG(LogTemp, Display, TEXT("%s setting the tank variable"), *Tower->GetActorNameOrLabel());

			}
		}

		LoopIndex++;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		HealthMessageWidget = CreateWidget<UScreenMessage>(PlayerController, HealthMessageClass);
		ScreenMessageWidget = CreateWidget<UScreenMessage>(PlayerController, ScreenMessageClass);
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->AddToPlayerScreen();
			ScreenMessageWidget->SetMessageText("Get Ready!");
		}
		if (HealthMessageWidget)
		{
			HealthMessageWidget->AddToPlayerScreen();
			RefreshHealthWidget();
		}
	}

	CountdownSeconds = CountdownDelay;
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ABattleBlasterGameMode::OnCountdownTimerTimeout, 1.0f, true);
		
}

void ABattleBlasterGameMode::OnCountdownTimerTimeout()
{
	CountdownSeconds -= 1;

	if (CountdownSeconds > 0)
	{
	ScreenMessageWidget->SetMessageText(FString::FromInt(CountdownSeconds));
	}
	else if (CountdownSeconds == 0)
	{
		ScreenMessageWidget->SetMessageText("GO!");
		Tank->SetPlayerEnabled(true);

	}
	else
	{
		//countdownseconds is less than 0
		UE_LOG(LogTemp, Display, TEXT("Clear Timer"));
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ABattleBlasterGameMode::RefreshHealthWidget()
{

	if (HealthMessageWidget && Tank)
	{
		UHealthComponent* TankHealthComp = Tank->FindComponentByClass<UHealthComponent>();
		if (TankHealthComp)
		{
			//Condition ? ResultIfTrue : ResultIfFalse;
			FString HealthString = FString::Printf(TEXT("Remaining Lives: %.0f"), TankHealthComp->Health);
			HealthMessageWidget->SetMessageText(HealthString);
		}
	}
}

void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;
	

	if (DeadActor == Tank)//didn't need to cast tank because we already have a pointer to the tank
	{
		//Tank just died
		Tank->HandleDestruction();
		IsGameOver = true;
	}
	else 
	{
		// A Tower just died
		ATower* DeadTower = Cast<ATower>(DeadActor);
		if (DeadTower) 
		{
			DeadTower->HandleDestruction();

		TowerCount--; 
		}
		if (TowerCount == 0)
		{
			IsGameOver = true; 
			IsVictory = true; 
		}
		
	}
	if (IsGameOver)
	{
		//Ternary Operator
		FString GameOverString = IsVictory ? "Victory" : "Defeat!";

		ScreenMessageWidget->SetMessageText(GameOverString);
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Visible);

		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(GameOverTimerHandle, this, &ABattleBlasterGameMode::OnGameOverTimerTimeout, GameOverDelay, false);
	}
	


}

void ABattleBlasterGameMode::OnGameOverTimerTimeout()
{

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UBattleBlasterGameInstance* BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);
		if (BattleBlasterGameInstance)
		{
			if (IsVictory)
			{
				//load next level
				BattleBlasterGameInstance->LoadNextLevel();

			}
			else
			{
				//reload current level
				BattleBlasterGameInstance->RestartCurrentLevel();
			}
		}


	}
}


