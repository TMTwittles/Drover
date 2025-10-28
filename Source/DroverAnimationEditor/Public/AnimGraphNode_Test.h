// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimGraphNode_Base.h"
#include "AnimNode_Test.h"
#include "AnimGraphNode_Test.generated.h"

/**
 * 
 */
UCLASS()
class DROVERANIMATIONEDITOR_API UAnimGraphNode_Test : public UAnimGraphNode_Base
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FAnimNode_Test Node;
};
