// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNodeBase.h"
#include "Animation/PoseAsset.h"
#include "TestAnimationAsset.h"
#include "AnimNode_Test.generated.h"

/**
 * 
 */
USTRUCT(BlueprintInternalUseOnly)
struct DROVERANIMATION_API FAnimNode_Test : public FAnimNode_Base
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> PoseIndexes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPoseAsset* PoseAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPoseLink BasePose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStrideLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinStrideLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (PinShownByDefault))
	float VelocityNormalized;

	bool bValidPoses;
	int32 CurrPoseIndex;
	int32 TargetPoseIndex;
	float InternalTimeAccumulator;
	float TargetStrideLength;
	bool bExtractedPoses = false;

	FAnimNode_Test();
	
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
	virtual void Evaluate_AnyThread(FPoseContext& Output) override;
	void ExtractPoses(FAnimationPoseData& OutData01, FAnimationPoseData& OutData02);
	FORCEINLINE bool IsValidPoses() const { return PoseAsset && PoseIndexes.Num() > 1; }
};
