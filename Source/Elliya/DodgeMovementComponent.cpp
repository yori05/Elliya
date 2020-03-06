// Made by Florian Hulin


#include "DodgeMovementComponent.h"

void UDodgeMovementComponent::Restart(bool InForceUpdate)
{
	UpdateControlPoints(InForceUpdate);
}
