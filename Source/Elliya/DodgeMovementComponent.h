// Made by Florian Hulin

#pragma once

#include "CoreMinimal.h"
#include "Components/InterpToMovementComponent.h"
#include "DodgeMovementComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent), HideCategories = Velocity)
class ELLIYA_API UDodgeMovementComponent : public UInterpToMovementComponent
{
	GENERATED_BODY()
	
public :

	UFUNCTION(BlueprintCallable, Category = "Control")
		virtual void Restart(bool InForceUpdate);
};

