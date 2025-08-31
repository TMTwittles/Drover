// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNodeBase.h"
#include "AnimNode_Test.generated.h"

/**
 * 
 */
USTRUCT(BlueprintInternalUseOnly)
struct DROVERANIMATION_API FAnimNode_Test : public FAnimNode_Base
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPoseLink BasePose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (PinShownByDefault))
	UAnimSequence* Sequence;

	FAnimNode_Test();
	
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void Evaluate_AnyThread(FPoseContext& Output) override;
};
