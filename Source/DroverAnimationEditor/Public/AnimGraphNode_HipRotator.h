// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimGraphNode_Base.h"
#include "AnimNode_HipRotator.h"
#include "AnimGraphNode_HipRotator.generated.h"

/**
 * 
 */
UCLASS()
class DROVERANIMATIONEDITOR_API UAnimGraphNode_HipRotator : public UAnimGraphNode_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FAnimNode_HipRotator Node;
};
