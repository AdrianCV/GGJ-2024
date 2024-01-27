// Fill out your copyright notice in the Description page of Project Settings.


#include "Hitbox.h"

#include "Components/CapsuleComponent.h"

// Sets default values
AHitbox::AHitbox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AHitbox::BeginOverlap);

	SetRootComponent(BoxComponent);
}

// Called when the game starts or when spawned
void AHitbox::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &AHitbox::DealDamage, 0.15, false);
}

// Called every frame
void AHitbox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHitbox::BeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult)
{
	if (AGGJ2024Character* Character = Cast<AGGJ2024Character>(Actor); IsValid(Character) && !Players.Contains(Character))
	{
		Players.Add(Character);	
	}	
}

void AHitbox::DealDamage()
{
	for (auto Player : Players)
	{
		if (Player != PlayerThatAttacked)
		{
			Player->TakeDamage(Damage);

			const FVector KnockbackDir = FVector(0, PlayerThatAttacked->GetActorLocation().Y - Player->GetActorLocation().Y ? 1 : -1, 0.5) * Knockback;

			Player->GetMesh()->SetSimulatePhysics(true);
			// Player->GetMesh()->SetPhysicsLinearVelocity(KnockbackDir);
			Player->GetMesh()->AddImpulse(KnockbackDir);
			// UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerThatAttacked->GetActorLocation().ToString());
			// UE_LOG(LogTemp, Warning, TEXT("%s"), *Player->GetActorLocation().ToString());
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Player->GetMesh()->GetPhysicsLinearVelocity().ToString());
		}	
	}
}



