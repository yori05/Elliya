// Made by Florian Hulin


#include "Weapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ElliyaCharacter.h"

// Sets default values
AWeapon::AWeapon() :
	bCatchable(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SkinWeapon = CreateDefaultSubobject<UStaticMeshComponent>("SkinWeapon");
	BodyCollider = CreateDefaultSubobject<UCapsuleComponent>("BodyCollider");
	HeadCollider = CreateDefaultSubobject<UCapsuleComponent>("HeadCollider");

	if (RootComponent)
	{
		if (SkinWeapon)
		{
			SkinWeapon->SetupAttachment(RootComponent);
			SkinWeapon->SetCollisionProfileName("NoCollision");
		}
		if (BodyCollider)
		{
			BodyCollider->SetupAttachment(RootComponent);
			BodyCollider->SetCollisionProfileName("OverlapAll");
			BodyCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &AWeapon::BodyBeginOverlap);
			BodyCollider->OnComponentEndOverlap.AddUniqueDynamic(this, &AWeapon::BodyEndOverlap);
		}
		if (HeadCollider)
		{
			HeadCollider->SetupAttachment(RootComponent);
			HeadCollider->SetCollisionProfileName("OverlapAll");
			HeadCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &AWeapon::BodyBeginOverlap);
			HeadCollider->OnComponentEndOverlap.AddUniqueDynamic(this, &AWeapon::BodyEndOverlap);
		}
	}
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Catch(AElliyaCharacter* NewCharacter)
{
	if (!NewCharacter)
		return;

	AttachToComponent(NewCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_lSocket");
	bCatchable = false;
}

void AWeapon::BodyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnBodyBeginOverlap.Broadcast(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (CatchOnGround && bCatchable)
	{
		auto parent = Cast<AElliyaCharacter>(OtherActor);
		Catch(parent);
	}
}

void AWeapon::BodyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OnBodyEndOverlap.Broadcast(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::HeadBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnHeadBeginOverlap.Broadcast(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::HeadEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OnHeadEndOverlap.Broadcast(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
