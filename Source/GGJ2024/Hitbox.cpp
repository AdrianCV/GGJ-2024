// Fill out your copyright notice in the Description page of Project Settings.


#include "Hitbox.h"


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

	// DrawDebugBox(GetWorld(), BoxComponent->GetComponentLocation(), BoxComponent->GetUnscaledBoxExtent(), FColor::Red, true);
	BoxComponent->SetHiddenInGame(false);

	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &AHitbox::DealDamage, 0.1, false);
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

			const FVector KnockbackDir =
				FVector(0, PlayerThatAttacked->GetActorLocation().Y - Player->GetActorLocation().Y > 0 ? -HorizontalRatio : HorizontalRatio, VerticalRatio)
				* ((((((Player->DamageTaken / 10) + ((Player->DamageTaken * Damage)/20)) * 1.4) + 18) * 1.1) + Knockback);

			Player->LaunchCharacter(KnockbackDir,false, true);
		}	
	}
}



