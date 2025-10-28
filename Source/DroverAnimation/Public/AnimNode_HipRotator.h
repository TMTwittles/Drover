// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimationAsset.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "AnimNode_HipRotator.generated.h"

/**
 *
 */
USTRUCT(BlueprintInternalUseOnly)
struct DROVERANIMATION_API FAnimNode_HipRotator : public FAnimNode_SkeletalControlBase
{
	GENERATED_USTRUCT_BODY()
	FAnimNode_HipRotator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPoseLink BasePose;

	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
	virtual void Evaluate_AnyThread(FPoseContext& Output) override;
};
