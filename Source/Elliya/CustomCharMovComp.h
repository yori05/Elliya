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

	/** Climbing on a surface. */
	CUSTOM_Dodge UMETA(DisplayName = "Dodge"),

	CUSTOM_MAX UMETA(Hidden),
};

UENUM(BlueprintType)
enum EDodgeState
{
	/** None (dodge is disabled).*/
	DODGE_None UMETA(DisplayName = "None"),

	/** Initialization of the dodge. */
	DODGE_Init UMETA(DisplayName = "Initialize"),

	/** Update of the dodge. */
	DODGE_Update UMETA(DisplayName = "Update"),

	/** End of the dodge. */
	DODGE_End UMETA(DisplayName = "End"),

	DODGE_Max UMETA(Hidden),
};

UENUM(BlueprintType)
enum EDodgeDirection
{
	DODGE_DIRECTION_None UMETA(DisplayName = "None"),
	DODGE_DIRECTION_Forward UMETA(DisplayName = "Forward"),
	DODGE_DIRECTION_Backward UMETA(DisplayName = "Backward"),
	DODGE_DIRECTION_Left UMETA(DisplayName = "Left"),
	DODGE_DIRECTION_Right UMETA(DisplayName = "Right"),
	DODGE_DIRECTION_Up UMETA(DisplayName = "Up"),
	DODGE_DIRECTION_Down UMETA(DisplayName = "Down"),
	DODGE_DIRECTION_Max UMETA(Hidden),
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dodge")
		bool bIsDodging = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dodge")
		uint8 dodgeDirection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dodge")
		uint8 dodgeState;

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

	UFUNCTION(BlueprintCallable)
		virtual void Dodge(uint8 _DirectionDodge = 0);
};
