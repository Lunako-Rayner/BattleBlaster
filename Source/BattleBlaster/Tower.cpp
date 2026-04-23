// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
//Super:: Makes calls to the begin play & Tick of parent class
void ATower::BeginPlay()
{
	Super::BeginPlay();


	FTimerHandle FireTimerHandle;
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);//this is a point to to this insance of this class
	//true allows us to loop timer after 2 seconds is done. Keeps calling checkfirecondition until you clear timer manually 
	//getworldmanager is more global function across ue5 not specific to the towerclass thats why we have to use scope resolution operator ::
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsInFireRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}

}

void ATower::CheckFireCondition()
{
	
	if (Tank && Tank->IsAlive && IsInFireRange())
	{
		Fire();
	}
}

bool ATower::IsInFireRange()
{
	bool Result = false;

	if (Tank)
	{
		float DistanceToTank = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
		Result = (DistanceToTank <= FireRange);
	
	}
	return Result;
}


void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}




