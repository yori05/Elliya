// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ElliyaCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Engine/World.h"
#include "Engine/Engine.h"

#include "Elliya/CustomCharMovComp.h"
#include "Components/BoxComponent.h"

//////////////////////////////////////////////////////////////////////////
// AElliyaCharacter

AElliyaCharacter::AElliyaCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharMovComp>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	CustomCharMovComp = Cast<UCustomCharMovComp>(GetMovementComponent());
	ClimbBoxCheck = CreateDefaultSubobject<UBoxComponent>(TEXT("ClimbBoxCheck"));

	if (CustomCharMovComp != nullptr)
	{
		CustomCharMovComp->SetClimbingBox(ClimbBoxCheck);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AElliyaCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AElliyaCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AElliyaCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AElliyaCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AElliyaCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Climb", IE_Pressed, this, &AElliyaCharacter::Climb);
}

void AElliyaCharacter::Climb()
{
	//auto transform = GetTransform();
	if (CustomCharMovComp != nullptr)
	{
		CustomCharMovComp->Climb();
	}
}

void AElliyaCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AElliyaCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

FRotator AElliyaCharacter::GetRotatorFromEnum(uint8 customMode) const
{
	switch (customMode)
	{
	case CUSTOM_Climbing:
		return GetTransform().GetRotation().Rotator();
	default:
		return Controller->GetControlRotation();
	}
}

void AElliyaCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && CustomCharMovComp != nullptr)
	{
		if (CustomCharMovComp != nullptr && CustomCharMovComp->GetCustomMode() == CUSTOM_Climbing)
		{
			const FRotator Rotation = GetRotatorFromEnum(CustomCharMovComp->GetCustomMode());
			const FRotator PitchRotation(0, Rotation.Yaw, 0);


			// get right vector 
			const FVector Direction = FRotationMatrix(PitchRotation).GetUnitAxis(EAxis::Z);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Vector x => %f y => %f z => %f"), Direction.X, Direction.Y, Direction.Z));
			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
		else
		{
			// find out which way is forward
			const FRotator Rotation = GetRotatorFromEnum(CustomCharMovComp->GetCustomMode());
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
	}
}

void AElliyaCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = GetRotatorFromEnum(CustomCharMovComp->GetCustomMode());
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
