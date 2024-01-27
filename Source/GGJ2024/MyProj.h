// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProj.generated.h"

UCLASS()
class GGJ2024_API AMyProj : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;
	
public:	
	// Sets default values for this actor's properties
	AMyProj();

	int ForwardDirection;
	int VerticalDirection = 0;

	float Speed;

	int Damage = 10;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
