// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProj.h"
#include "Components/StaticMeshComponent.h" 

// Sets default values
AMyProj::AMyProj()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetRootComponent(MeshComponent);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TestShape(TEXT("'/Engine/BasicShapes/Cube.Cube'"));

	MeshComponent->SetStaticMesh(TestShape.Object);
	// MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void AMyProj::BeginPlay()
{
	Super::BeginPlay();

    MeshComponent->SetConstraintMode(EDOFMode::YZPlane);
	
    // MeshComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
    MeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
}

// Called every frame
void AMyProj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y + ForwardDirection * Speed, GetActorLocation().Z + VerticalDirection * Speed));
}

