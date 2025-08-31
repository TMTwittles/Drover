// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimGraphNode_Base.h"
#include "TestAnimNode.h"
#include "MyAnimGraphNode_Test.generated.h"

/**
 * 
 */
UCLASS()
class DROVERANIMATIONEDITOR_API UMyAnimGraphNode_Test : public UAnimGraphNode_Base
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FAnimNode_Test Node;

    // Input pose (connect other animation here)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FPoseLink InPose;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimSequence> InAnimSequence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> TimeBetweenFrames;
};
