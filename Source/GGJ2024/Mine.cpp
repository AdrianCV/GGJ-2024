// Fill out your copyright notice in the Description page of Project Settings.


#include "Mine.h"

// Sets default values
AMine::AMine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	SetRootComponent(BoxComponent);
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMine::BeginOverlap);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TestShape(TEXT("'/Engine/BasicShapes/Cube.Cube'"));

	Mesh->SetStaticMesh(TestShape.Object);
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	// Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	// Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Mesh->SetConstraintMode(EDOFMode::YZPlane);
	// Mesh->SetSimulatePhysics(true);
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// BoxComponent->Mobility = EComponentMobility::Movable;
	// Mesh->Mobility = EComponentMobility::Movable;

	BoxComponent->SetHiddenInGame(false);
	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
}

// Called when the game starts or when spawned
void AMine::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetWorldScale3D(FVector(1, 1, 0.39));
	SetActorScale3D(FVector(1, 1, 0.4));
	
	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &AMine::ActivateHitbox, 0.1, false);
}

// Called every frame
void AMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMine::BeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult)
{
	AGGJ2024Character* Player = Cast<AGGJ2024Character>(Actor);
	if (!IsValid(Player))
	{
		return;	
	}
	
	if (Player != PlayerThatAttacked)
    {
    	Player->TakeDamage(Damage);

    	const FVector KnockbackDir =
    		FVector(0, GetActorLocation().Y - Player->GetActorLocation().Y > 0 ? -HorizontalRatio : HorizontalRatio, VerticalRatio)
    		* ((((((Player->DamageTaken / 10) + ((Player->DamageTaken * Damage)/20)) * 1.4) + 18) * 1.1) + Knockback);

    	Player->LaunchCharacter(KnockbackDir,true, true);

		PlayerThatAttacked->ResetMineCooldown();
		Destroy();
    }
}

void AMine::ActivateHitbox()
{
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	UE_LOG(LogTemp, Warning, TEXT("Hitbox overlap now"))
}



