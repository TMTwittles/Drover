// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Test_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DROVERANIMATION_API UTest_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (PinShownByDefault))
	FVector ActorLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (PinShownByDefault))
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (PinShownByDefault))
	float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(PinShownByDefault))
	float MaxSpeed;
	
};
