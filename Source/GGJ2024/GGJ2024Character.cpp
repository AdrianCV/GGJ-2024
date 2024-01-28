// Copyright Epic Games, Inc. All Rights Reserved.

#include "GGJ2024Character.h"
#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Hitbox.h"
#include "InputActionValue.h"
#include "MyProj.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGGJ2024Character



AGGJ2024Character::AGGJ2024Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AGGJ2024Character::BeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AGGJ2024Character::EndOverlap);

		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	GetMesh()->SetConstraintMode(EDOFMode::YZPlane);


	// // Create a camera boom (pulls in towards the player if there is a collision)
	// CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	// CameraBoom->SetupAttachment(RootComponent);
	// CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	// CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	//
	// // Create a follow camera
	// FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	// FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AGGJ2024Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set Collision
	// GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1);
	// GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	//
	// GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	// GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AGGJ2024Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGGJ2024Character::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const AMyProj* Proj = Cast<AMyProj>(OtherActor); IsValid(Proj))
	{
		DamageTaken += Proj->Damage; 
	}
	else if (AGGJ2024Character* OverlappedPlayer = Cast<AGGJ2024Character>(OtherActor); IsValid(OverlappedPlayer))
	{
		OtherPlayer = OverlappedPlayer;
	}
}

void AGGJ2024Character::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (const AGGJ2024Character* OverlappedPlayer = Cast<AGGJ2024Character>(OtherActor); IsValid(OverlappedPlayer))
 	{
 		OtherPlayer = nullptr;
 	}
}

void AGGJ2024Character::TakeDamage(int DamageToTake)
{
	DamageTaken += DamageToTake;
}

void AGGJ2024Character::ResetShootCooldown()
{
	bCanShoot = true;
}

void AGGJ2024Character::ResetSleepCooldown()
{
	bCanSleep = true;
}

void AGGJ2024Character::ResetJabCooldown()
{
	bCanJab = true;
}

void AGGJ2024Character::ResetJabIndex()
{
	JabIndex = 0;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGGJ2024Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGGJ2024Character::Move2D);

		// Looking
		// EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGGJ2024Character::Look);

		// Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AGGJ2024Character::Shoot);

		// Sleep
		EnhancedInputComponent->BindAction(SleepAction, ETriggerEvent::Triggered, this, &AGGJ2024Character::Sleep);

		// Jab
		EnhancedInputComponent->BindAction(JabAction, ETriggerEvent::Triggered, this, &AGGJ2024Character::Jab);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AGGJ2024Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		// ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		// AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGGJ2024Character::Move2D(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	ForwardDirection = MovementVector.X > 0 ? 1 : -1;
	VerticalInputValue = MovementVector.Y;
	
	// const FRotator Rotation = Controller->GetControlRotation();
	// const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	// const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(FVector(0.0, 1.0, 0.0), MovementVector.X * 0.1);
	// UE_LOG(LogTemp, Warning, TEXT("%f"), MovementVector.Y)
	
	SetActorRotation(MovementVector.X > 0 ? FRotator(0.0, 90.0, 0.0) : FRotator(0.0, -90.0, 0.0));
}

void AGGJ2024Character::Shoot()
{
	if (!bCanShoot)
	{
		return;	
	}
	
	const FVector SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y + ForwardDirection * 100, GetActorLocation().Z);
	
	AMyProj* SpawnedProjectile = GetWorld()->SpawnActor<AMyProj>(AMyProj::StaticClass(), SpawnLocation, GetActorRotation());

	SpawnedProjectile->ForwardDirection = ForwardDirection;
	SpawnedProjectile->Speed = ShotSpeed;

	bCanShoot = false;

	SpawnedProjectile->SetLifeSpan(ShotLifespan);

	FTimerHandle Timer;
	// FTimerDelegate TimerDelegate;

	// TimerDelegate.BindUFunction(this, FName("ResetCooldown"), bHasShot, bCanShoot);
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AGGJ2024Character::ResetShootCooldown, ShootCooldown, false);
}

void AGGJ2024Character::Sleep()
{
	if (VerticalInputValue < -10)
	{
		if (!bCanSleep)
     	{
     		return;
     	}
		
		FVector SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z -10);	
		AHitbox* Hitbox = GetWorld()->SpawnActor<AHitbox>(AHitbox::StaticClass(), SpawnLocation, GetActorRotation());

		Hitbox->PlayerThatAttacked = this;
		Hitbox->Damage = 30;
		Hitbox->Knockback = 500;
		Hitbox->KnockbackScaling = 5;
		Hitbox->HorizontalRatio = 3;
		Hitbox->VerticalRatio = 0.55;

		Hitbox->SetLifeSpan(0.3);

		bCanSleep = false;	

		FTimerHandle Timer;

		GetWorld()->GetTimerManager().SetTimer(Timer, this, &AGGJ2024Character::ResetSleepCooldown, SleepCooldown, false);
	}
}

void AGGJ2024Character::Jab()
{
	if (!bCanJab)
	{
		return;
	}

	FVector SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y + (60 * ForwardDirection) , GetActorLocation().Z);

	AHitbox* Hitbox = GetWorld()->SpawnActor<AHitbox>(AHitbox::StaticClass(), SpawnLocation, GetActorRotation());

	Hitbox->PlayerThatAttacked = this;
	Hitbox->HorizontalRatio = 1;
    Hitbox->VerticalRatio = 0.3;

	Hitbox->SetLifeSpan(0.3);
	
	FTimerHandle CooldownTimer;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &AGGJ2024Character::ResetJabCooldown, JabCooldown, false);
	
	switch (JabIndex)
	{
	case 0:
		Hitbox->Damage = 1;
		Hitbox->Knockback = 200;
		Hitbox->KnockbackScaling = 1;
		
		Hitbox->SetActorScale3D(FVector(1.5, 1, 2));
		
		JabIndex++;
		
		break;
	case 1:
		Hitbox->Damage = 3;
		Hitbox->Knockback = 300;
		Hitbox->KnockbackScaling = 1;

		Hitbox->SetActorScale3D(FVector(2, 1, 2));
		
		JabIndex++;
		
		break;
	case 2:
		Hitbox->Damage = 5;
		Hitbox->Knockback = 750;
		Hitbox->KnockbackScaling = 2;

		Hitbox->SetActorScale3D(FVector(2.5, 1, 2));
		
		JabIndex = 0;
		
		GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &AGGJ2024Character::ResetJabCooldown, 0.7, false);
		break;
	default:
		break;
	}

	bCanJab = false;

	GetWorld()->GetTimerManager().SetTimer(ResetJabIndexTimerHandle, this, &AGGJ2024Character::ResetJabIndex, ResetJabIndexTimer, false);
}
