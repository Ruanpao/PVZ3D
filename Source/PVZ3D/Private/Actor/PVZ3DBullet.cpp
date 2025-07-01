// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PVZ3DBullet.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

APVZ3DBullet::APVZ3DBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	RootComponent = ProjectileMesh;
	ProjectileMesh->SetCollisionProfileName("Projectile");
	ProjectileMesh->SetGenerateOverlapEvents(true);
	ProjectileMesh->SetNotifyRigidBodyCollision(true);

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 1500.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->SetActive(false);

	ProjectileMesh->OnComponentHit.AddDynamic(this, &APVZ3DBullet::OnHit);
}

void APVZ3DBullet::BeginPlay()
{
	Super::BeginPlay();

	DeactivateProjectile();
}

void APVZ3DBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APVZ3DBullet::ActivateProjectile(FVector Location, FRotator Rotation, FVector Direction,float Damage)
{
	DamageAmount = Damage;
	bActive = true;
	
	SetActorLocation(Location);
	SetActorRotation(Rotation);
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	ProjectileMovement->SetActive(true);
	
	Direction.Normalize();
	ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
	
	GetWorld()->GetTimerManager().SetTimer(LifeSpanTimer, this, &APVZ3DBullet::DeactivateProjectile, 3.0f, false);
}

void APVZ3DBullet::DeactivateProjectile()
{
	bActive = false;
	
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	ProjectileMovement->SetActive(false);
	ProjectileMovement->Velocity = FVector::ZeroVector;
	
	GetWorld()->GetTimerManager().ClearTimer(LifeSpanTimer);
}

void APVZ3DBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	
	if (OtherActor == MyOwner || OtherActor == GetInstigator())
	{
		return;
	}
	
	if (OtherActor && OtherActor->IsA(StaticClass()))
	{
		return;
	}
	
	if (OtherActor && OtherActor != this)
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,
			DamageAmount,
			GetInstigatorController(),
			this,
			UDamageType::StaticClass()
		);
	}
	
	DeactivateProjectile();
}

void APVZ3DBullet::IgnoreActor(AActor* Actor)
{
	if (ProjectileMesh && Actor)
	{
		ProjectileMesh->IgnoreActorWhenMoving(Actor, true);
	}
}

void APVZ3DBullet::IgnoreOtherProjectiles(AActor* ProjectileToIgnore)
{
	if (ProjectileMesh && ProjectileToIgnore)
	{
		ProjectileMesh->IgnoreActorWhenMoving(ProjectileToIgnore, true);
	}
}

float APVZ3DBullet::GetProjectileSpeed() const
{
	return ProjectileMovement ? ProjectileMovement->InitialSpeed : 0.0f;
}

void APVZ3DBullet::SetProjectileSpeed(float NewSpeed)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = NewSpeed;
		ProjectileMovement->MaxSpeed = NewSpeed;
		
		if (bActive)
		{
			FVector CurrentDirection = ProjectileMovement->Velocity.GetSafeNormal();
			ProjectileMovement->Velocity = CurrentDirection * NewSpeed;
		}
	}
}