// Fill out your copyright notice in the Description page of Project Settings.
#include "DroverPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "SlideHandler.h"

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

	// Configure slide handler collision configuration
	{
		FSlideHandler::FCollisionConfig DroverPawnCollisionConfig;
		DroverPawnCollisionConfig.Channel = ECC_Visibility;
		DroverPawnCollisionConfig.Rotation = FQuat::Identity;
		DroverPawnCollisionConfig.QueryParams.AddIgnoredActor(this);
		DroverPawnCollisionConfig.Shape =
			FCollisionShape::MakeCapsule(
				CapsuleCollider->GetScaledCapsuleRadius(), 
				CapsuleCollider->GetScaledCapsuleHalfHeight());
		SlideHandler.SetCollisionConfig(DroverPawnCollisionConfig);
	}
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
		EnhancedInputComponent->BindAction(DescendAction, ETriggerEvent::Triggered, this, &ADroverPawn::Descend);
		EnhancedInputComponent->BindAction(AscendAction, ETriggerEvent::Triggered, this, &ADroverPawn::Ascend);
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

void ADroverPawn::Descend()
{
	Velocity.Z -= MoveSpeed;
}

void ADroverPawn::Ascend()
{
	Velocity.Z += MoveSpeed;
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

	// Apply movement input to velocity
	const FVector2D& CurrInput = ConsumeMovementInput();
	Velocity = ((ForwardDir * CurrInput.Y + RightDir * CurrInput.X) * MoveSpeed) + FVector(0.0f, 0.0f, Velocity.Z);
	Velocity *= DeltaTime;
	
	SafeAddActorWorldOffset();
}

void ADroverPawn::SafeAddActorWorldOffset()
{
	// Use the slide handler to perform appropriate collision handling sliding 
	// for character velocity.
	FVector CollisionSlideAdjustedPosition;
	const int32 MaxNumIterations = 5;
	SlideHandler.TryStepPosition(GetWorld(), GetActorLocation(), Velocity, MaxNumIterations, CollisionSlideAdjustedPosition);
	SetActorLocation(CollisionSlideAdjustedPosition);
}
