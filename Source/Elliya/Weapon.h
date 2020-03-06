// Made by Florian Hulin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UStaticMeshComponent;
class UCapsuleComponent;
class AElliyaCharacter;

UCLASS()
class ELLIYA_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* SkinWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* BodyCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* HeadCollider;

	UPROPERTY()
	uint8 bCatchable : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CatchOnGround = true;

public :
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FComponentBeginOverlapSignature OnBodyBeginOverlap;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FComponentEndOverlapSignature OnBodyEndOverlap;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FComponentBeginOverlapSignature OnHeadBeginOverlap;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FComponentEndOverlapSignature OnHeadEndOverlap;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void BodyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
		void BodyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void HeadBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
		void HeadEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		FORCEINLINE UStaticMeshComponent* GetSkinWeapon() { return SkinWeapon; }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE UCapsuleComponent* GetBodyCollider() { return BodyCollider; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCapsuleComponent* GetHeadCollider() { return HeadCollider; }

	UFUNCTION(BlueprintCallable)
	bool GetCatchable() { return bCatchable; }
	UFUNCTION(BlueprintCallable)
	void Catch(AElliyaCharacter* NewCharacter);
};
