// Fill out your copyright notice in the Description page of Project Settings.
#include "DroverPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"

// Sets default values
ADroverPawn::ADroverPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	SetRootComponent(CapsuleCollider);
	
	SkelMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("PawnMesh");
	SkelMeshComp->SetupAttachment(RootComponent);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 400.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void ADroverPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADroverPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickMovement(DeltaTime);
}

// Called to bind functionality to input
void ADroverPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADroverPawn::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ADroverPawn::Look);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADroverPawn::Look);
	}
}

void ADroverPawn::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	Move(MovementVector);
}

void ADroverPawn::Move(const FVector2D& Value)
{
	MovementInput = Value;
}

void ADroverPawn::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	Look(LookAxisVector);
}

void ADroverPawn::Look(const FVector2D& Value)
{
	if (GetController())
	{
		AddControllerYawInput(Value.X);
		AddControllerPitchInput(Value.Y);
	}
}

FVector2D ADroverPawn::ConsumeMovementInput()
{
	const FVector2D PreMovementInput = MovementInput;
	MovementInput = FVector2D::Zero();
	return PreMovementInput;
}

void ADroverPawn::TickMovement(const float DeltaTime)
{
	const FRotator& CurrCameraRotator = CameraComp->GetComponentRotation();
	const FRotator& YawRotation = FRotator(0.f, CurrCameraRotator.Yaw, 0.f);
	const FVector& ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector& RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Apply movement input
	const FVector2D& CurrInput = ConsumeMovementInput();
	Velocity = (ForwardDir * CurrInput.Y + RightDir * CurrInput.X) * MoveSpeed;
	Velocity.Z -= Gravity;
	Velocity *= DeltaTime;

	TArray<FHitResult> Hits;
	if (PerformSweep(GetActorLocation(), GetActorLocation() + Velocity, Hits))
	{
		for (const FHitResult& Hit : Hits)
		{
			float Dot = FVector::DotProduct(Velocity, Hit.Normal);
			if (Dot < 0.f)
			{
				Velocity -= Dot * Hit.Normal;
			}
		}
	}

	AddActorWorldOffset(Velocity, false);
}

inline bool ADroverPawn::PerformSweep(const FVector& StartTrace, const FVector& EndTrace, TArray<FHitResult>& OutHits)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	return GetWorld()->SweepMultiByChannel(
		OutHits,
		StartTrace,
		EndTrace,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeCapsule(CapsuleCollider->GetScaledCapsuleRadius(), CapsuleCollider->GetScaledCapsuleHalfHeight()),
		Params
	);
}

inline bool ADroverPawn::PerformSweep(const FVector& StartTrace, const FVector& EndTrace, FHitResult& Hit)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	return GetWorld()->SweepSingleByChannel(
		Hit,
		StartTrace,
		EndTrace,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeCapsule(CapsuleCollider->GetScaledCapsuleRadius(), CapsuleCollider->GetScaledCapsuleHalfHeight()),
		Params
	);
}

