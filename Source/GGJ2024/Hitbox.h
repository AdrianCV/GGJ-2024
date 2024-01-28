// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GGJ2024Character.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Hitbox.generated.h"

UCLASS()
class GGJ2024_API AHitbox : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;

	
public:
	// Sets default values for this actor's properties
	AHitbox();

	int Damage;
	int Knockback;
	float KnockbackScaling;
	float VerticalRatio;
	float HorizontalRatio;


	UPROPERTY(EditAnywhere)
	AGGJ2024Character* PlayerThatAttacked;
	
	UPROPERTY(EditAnywhere)
	TArray<AGGJ2024Character*> Players;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult);


	void DealDamage();
};
