// Made by Florian Hulin


#include "CustomCharMovComp.h"
#include "Math/UnrealMathVectorCommon.h"

#include "Engine/World.h"
#include "Engine/Engine.h"

#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

#include "DrawDebugHelpers.h"

void UCustomCharMovComp::PhysCustom(float deltaTime, int32 Iterations)
{
	/////KYsmet
	Super::PhysCustom(deltaTime, Iterations);

	switch ((EMovementModeCustom)CustomMovementMode)
	{
	case EMovementModeCustom::CUSTOM_None:
		break;

	case EMovementModeCustom::CUSTOM_Climbing:
		PhysClimbing(deltaTime, Iterations);
		break;

	default:
		break;
	}
}

void UCustomCharMovComp::PhysClimbing(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	if (Acceleration.IsZero())
	{
		Velocity = FVector::ZeroVector;
	}

	/*
	if (GEngine != nullptr)
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Acceleration = X > %f Y > %f Z > %f    Velocity = X > %f Y > %f Z > %f"), Acceleration.X, Acceleration.Y , Acceleration.Z, Velocity.X, Velocity.Y, Velocity.Z));
	*/

	//FHitResult hit(1.f);

	//if (Count == 0)
	{
		//Count = 10;

		CalcVelocity(deltaTime, 0.f, true, GetMaxBrakingDeceleration());

		const auto Adjusted = Velocity * deltaTime;

		SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentRotation(), true, hit);

		if (hit.bBlockingHit)
		{
			if (hit.Normal.Z >= 0.5f)
			{
				StopClimb();
				return;
			}
			else if (hit.Normal.Y >= 0.5f)
			{
				DetectRight(hit, 100.f);
			}
			else if (hit.Normal.Y <= -0.5f)
			{
				DetectRight(hit, 100.f, -1.0f);
			}
			/*
			return;
			ClimbingNormal = hit.Normal;

			const auto transform = GetCharacterOwner()->GetTransform();
			const auto Rotation = FRotationMatrix::MakeFromXZ(-ClimbingNormal, GetCharacterOwner()->GetActorUpVector()).Rotator();
			SafeMoveUpdatedComponent(FVector::ZeroVector, Rotation, false, hit);
			*/
		}
		else
		{
			DetectForward(hit, 100.f);
		}
	}
	//else
	//{
	//	Count --;
	//}

	if (hit.bBlockingHit)
	{
		OrientFromHit(hit, 0.50f);
	}
}


void UCustomCharMovComp::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	const auto prevMov = MovementMode;
	const auto prevCus = CustomMovementMode;

	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (CustomMovementMode == EMovementModeCustom::CUSTOM_None && !bOrientRotationToMovement)
	{
		//bOrientRotationToMovement = true;
	}

	/*
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("MovementMode : %d Previous : %d"), (uint8)MovementMode, (uint8)prevMov));
	*/
}

void UCustomCharMovComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


bool UCustomCharMovComp::IsClimbing() const
{
	return (MovementMode == MOVE_Custom) && (CustomMovementMode == CUSTOM_Climbing) && UpdatedComponent;
}

void UCustomCharMovComp::StopClimb()
{
	SetMovementMode(MOVE_Walking, CUSTOM_None);

	const auto rotation = FRotationMatrix::MakeFromX(FVector::ForwardVector).Rotator();
	UpdatedComponent->SetWorldRotation(rotation.Quaternion());
	bOrientRotationToMovement = true;
}

void UCustomCharMovComp::OrientFromHit(FHitResult& resultHit, float lerpValue)
{
	FHitResult hit;
	const auto transform = GetCharacterOwner()->GetTransform();
	auto temp = (-resultHit.Normal).ToOrientationRotator();
	auto owner = GetOwner();
	if (owner != nullptr)
	{
		owner->SetActorRotation(FMath::Lerp(owner->GetActorRotation(), temp, lerpValue));
		owner->SetActorLocation(FMath::Lerp(owner->GetActorLocation(), resultHit.ImpactPoint + resultHit.Normal * 50.f, lerpValue));
	}
	//if (GEngine != nullptr)
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Vector x => %f y => %f z => %f"), resultHit.Normal.X, resultHit.Normal.Y, resultHit.Normal.Z));
//const auto Rotation = FRotationMatrix::MakeFromXZ(-resultHit.Normal, GetCharacterOwner()->GetActorUpVector()).Rotator();
	//SafeMoveUpdatedComponent(FVector::ZeroVector, Rotation, false, hit); //resultHit.ImpactPoint + resultHit.Normal
}

void UCustomCharMovComp::Climb()
{
	FHitResult resultHit;

	if (CustomMovementMode == CUSTOM_Climbing)
	{
		StopClimb();
	}
	else if (CanClimb(resultHit))
	{
		ClimbingNormal = resultHit.Normal;
		SetMovementMode(MOVE_Custom, CUSTOM_Climbing);
		bOrientRotationToMovement = false;
		OrientFromHit(resultHit, 1.f);
		Count = 0;
		hit = FHitResult(1.0f);
	}

	//if (GEngine != nullptr && MovementMode == MOVE_Falling)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("New %d")));

}

bool UCustomCharMovComp::DetectForward(FHitResult& resultHit, float deepth, float side) const
{
	if (nullptr != CharacterOwner)
	{
		FCollisionQueryParams queryParams;

		const FVector _StartLocation = CharacterOwner->GetActorLocation();
		const FVector EndRayCast = _StartLocation + (CharacterOwner->GetActorForwardVector() * (CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() + deepth) * side);

		if (nullptr != GetWorld())
		{
			const bool ValidHit = false;
			GetWorld()->LineTraceSingleByChannel(resultHit, _StartLocation, EndRayCast, ECollisionChannel::ECC_Visibility, queryParams);

			return resultHit.bBlockingHit;
		}
	}

	resultHit.bBlockingHit = false;
	return false;
}

bool UCustomCharMovComp::DetectRight(FHitResult& resultHit, float deepth, float side) const
{
	if (nullptr != CharacterOwner)
	{
		FCollisionQueryParams queryParams;

		const FVector _StartLocation = CharacterOwner->GetActorLocation();
		const FVector EndRayCast = _StartLocation + (CharacterOwner->GetActorRightVector() * (CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() + deepth) * side);

		if (nullptr != GetWorld())
		{
			const bool ValidHit = false;
			GetWorld()->LineTraceSingleByChannel(resultHit, _StartLocation, EndRayCast, ECollisionChannel::ECC_Visibility, queryParams);

			return resultHit.bBlockingHit;
		}
	}

	return false;
}

bool UCustomCharMovComp::CanClimb(FHitResult& resultHit) const
{
	auto detect = DetectForward(resultHit, 20.f);

	if (detect && resultHit.bBlockingHit)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Actor : %s"), *GetNameSafe(resultHit.GetActor())));
		DrawDebugLine(GetWorld(), resultHit.TraceStart, resultHit.TraceEnd, FColor::Cyan, false, 2.f, 0.f, 5.f);
		return true;
	}

	return false;
}

uint8 UCustomCharMovComp::GetCustomMode() const
{
	return CustomMovementMode;
}
