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
	MeshComponent->SetSimulatePhysics(true);
	
	const FRotator Rotation = GetActorRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);
    
    // get forward vector
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

    MeshComponent->AddForce(ForwardDirection * 10);

    MeshComponent->SetConstraintMode(EDOFMode::YZPlane);	
}

// Called when the game starts or when spawned
void AMyProj::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyProj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

