// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Engine/TriggerBox.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"

#include "GGJ2024Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AGGJ2024Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	// USpringArmComponent* CameraBoom;
	//
	// /** Follow camera */
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	// UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	const UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	const UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, Category = Input)
	const UInputAction* SleepAction;

	UPROPERTY(EditAnywhere, Category = Input)
	const UInputAction* JabAction;

public:
	float ForwardDirection = 1;

	UPROPERTY(EditAnywhere)
	float DamageTaken;

	UPROPERTY(EditAnywhere)
	float ShotLifespan = 2;

	UPROPERTY(EditAnywhere)
	float ShotSpeed = 2;

	UPROPERTY(EditAnywhere)
	float SleepCooldown = 5;

	UPROPERTY(EditAnywhere)
	float ShootCooldown = 2;

	UPROPERTY(EditAnywhere)
	float JabCooldown = 0.2;

	UPROPERTY(EditAnywhere)
	bool bCanShoot = true;

	UPROPERTY(EditAnywhere)
	bool bCanSleep = true;

	UPROPERTY(EditAnywhere)
	bool bCanJab = true;

	UPROPERTY(EditAnywhere)
	int JabIndex;

	UPROPERTY(EditAnywhere)
	float ResetJabIndexTimer;

	FTimerHandle ResetJabIndexTimerHandle;
	
	UPROPERTY()
	AGGJ2024Character* OtherPlayer;

	float VerticalInputValue;

public:
	AGGJ2024Character();

	
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	void Move2D(const FInputActionValue& Value);
	
	void Shoot();

	void Sleep();

	void Jab();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
								   UPrimitiveComponent* OtherComp, 
								   int32 OtherBodyIndex, 
								   bool bFromSweep, 
								   const FHitResult &SweepResult );

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	

public:

	UFUNCTION()
	void TakeDamage(int DamageToTake);

	UFUNCTION()
	void ResetShootCooldown();

	UFUNCTION()
	void ResetSleepCooldown();

	UFUNCTION()
	void ResetJabCooldown();

	UFUNCTION()
	void ResetJabIndex();

	
	// /** Returns CameraBoom subobject **/
	// FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	// /** Returns FollowCamera subobject **/
	// FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

