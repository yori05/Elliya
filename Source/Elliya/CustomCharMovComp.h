// Made by Florian Hulin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharMovComp.generated.h"

UENUM(BlueprintType)
enum EMovementModeCustom
{
	/** None (mouvement is disabled).*/
	CUSTOM_None UMETA(DisplayName = "None"),

	/** Climbing on a surface. */
	CUSTOM_Climbing UMETA(DisplayName = "Climbing"),

	CUSTOM_MAX UMETA(Hidden),
};

 //=============================================================================
 /**
  * CustomCharacterMovementComponent (CustomCharMovComp) handles movement logic for the associated Character owner.
  * It supports various movement modes including: walking, falling, swimming, flying, custom.
  *
  * Movement is affected primarily by current Velocity and Acceleration. Acceleration is updated each frame
  * based on the input vector accumulated thus far (see UPawnMovementComponent::GetPendingInputVector()).
  *
  * Networking is fully implemented, with server-client correction and prediction included.
  *
  * @see UCharacterMovementComponent, ACharacter, UPawnMovementComponent
  * @see https://docs.unrealengine.com/latest/INT/Gameplay/Framework/Pawn/Character/
  */

class UBoxComponent;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ELLIYA_API UCustomCharMovComp : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly)
	FVector ClimbingNormal;

	UPROPERTY(BlueprintReadOnly)
	FHitResult hit;

	UPROPERTY(BlueprintReadOnly)
	int Count;

	bool bCanClimb = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBoxComponent* ClimbBoxCheck;

public :

	//void SetClimbingBox(UBoxComponent* _box);
	UBoxComponent* GetClimbingBox() const {return ClimbBoxCheck;};

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual void PhysClimbing(float deltaTime, int32 Iterations);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|Climb")
	virtual bool IsClimbing() const;

	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|Climb")
	virtual void Climb();

	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|Climb")
	virtual bool CanClimb(FHitResult& hitResult) const;

	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|Detection")
	virtual void OrientFromHit(FHitResult& resultHit, float lerpValue);

	UFUNCTION(BlueprintCallable, Category = "Detection")
	virtual bool DetectForward(FHitResult& resultHit, float deepth, float side = 1.0f) const;

	UFUNCTION(BlueprintCallable, Category = "Detection")
	virtual bool DetectRight(FHitResult& resultHit, float deepth, float side = 1.0f) const;

	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|Climb")
	virtual void StopClimb();

	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|Custom")
	uint8 GetCustomMode() const;
};
