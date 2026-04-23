// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "Camera/CameraComponent.h"

#include "InputMappingContext.h"

#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
}
// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{	
			
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
	SetPlayerEnabled(false);

}


// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerController)
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

		RotateTurret(HitResult.ImpactPoint);
		//ImpactPoint;point used to rotate turret mesh
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25.0f, 12, FColor::Green);
	}
}

// Called to bind functionality to input. Every pawn or character class has this function
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)//PIC base version
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))//EIC upgraded version
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::MoveInput); //this tank object
		EIC->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::TurnInput); 
		EIC->BindAction(FireAction, ETriggerEvent::Started, this, &ATank::Fire); //ATank inherits from basepawn
		
	}
	//"I am revealing (Casting) to the engine that this specific object is actually a Smart Hub (EIC), even though it's wearing a Basic Radio (PIC) uniform. If the engine sees that it is indeed a Smart Hub, the EIC tool connects the Move Action Data Asset to this specific Tank's Movement Muscle (the address of MoveInput) whenever the player Triggers it."

	//UInputAction is a data asset ie a file you create in the editor
}

void ATank::MoveInput(const FInputActionValue& Value)// Need to bind this MoveInputACtion to the Set Up PLayer Input Component 
{//FInputActionValue& is a temporary shortcut to a small struct

	float InputValue = Value.Get<float>();
	//UE_LOG(LogTemp, Display, TEXT("InputValue: %f"), InputValue);

	FVector DeltaLocation = FVector(0.0f, 0.0f, 0.0f);//FVector(X,Y,Z)
	DeltaLocation.X = Speed * InputValue * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()); //delta makes framerate independent

	AddActorLocalOffset(DeltaLocation, true);

}

void ATank::TurnInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();

	FRotator DeltaRotation = FRotator(0.0f, 0.0f, 0.0f);//FRotator(Pitch.Yaw,Roll)/yzx
	DeltaRotation.Yaw = TurnRate * InputValue * GetWorld()->GetDeltaSeconds();
	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);//tick function of the tank won't be called anymore
	SetPlayerEnabled(false);

	IsAlive = false; 
}


void ATank::SetPlayerEnabled(bool Enabled)
{
	if (PlayerController)
	{
		if (Enabled)
		{
			EnableInput(PlayerController);
		}
		else
		{
			DisableInput(PlayerController);

		}
		PlayerController->bShowMouseCursor = Enabled;
	}
}




