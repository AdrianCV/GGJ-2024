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
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TestShape(TEXT("'/Engine/BasicShapes/Cube.Cube'"));

	Mesh->SetStaticMesh(TestShape.Object);
}

// Called when the game starts or when spawned
void AMine::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(FVector(1, 1, 0.3));
}

// Called every frame
void AMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMine::BeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult)
{
	
}

