// Fill out your copyright notice in the Description page of Project Settings.


#include "TestAnimUpdaterActorComponent.h"
#include "Test_AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UTestAnimUpdaterActorComponent::UTestAnimUpdaterActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTestAnimUpdaterActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTestAnimUpdaterActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ACharacter* MyChar = Cast<ACharacter>(GetOwner());

	if (!MyChar)
	{
		return;
	}

	UTest_AnimInstance* AnimInst = Cast<UTest_AnimInstance>(MyChar->GetMesh()->GetAnimInstance());
	UCharacterMovementComponent* CharMovementComponent = MyChar->GetCharacterMovement();

	if (CharMovementComponent && AnimInst)
	{
		AnimInst->CurrentSpeed = CharMovementComponent->Velocity.Length();
		AnimInst->MaxSpeed = CharMovementComponent->GetMaxSpeed();
	}
}

