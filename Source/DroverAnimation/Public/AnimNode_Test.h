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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeBetweenFrames;

	FCompactPose StartPose;
	FCompactPose EndPose;
	FCompactPose CurrentPose;
	int32 TargetFrame;
	float ElapsedTime;
	bool bExtractedPoses = false;

	FAnimNode_Test();
	
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
	virtual void Evaluate_AnyThread(FPoseContext& Output) override;
	void ExtractAnimPose(const double InSequenceTime, FCompactPose& OutExtractedPose);
};
