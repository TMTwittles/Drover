// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimationAsset.h"
#include "Animation/AnimSequence.h"
#include "TestAnimationAsset.generated.h"

/**
 * 
 */
UCLASS()
class DROVERANIMATION_API UTestAnimationAsset : public UAnimationAsset
{
	GENERATED_BODY() 

public:
	void BindAnimationSequence(const UAnimSequence* InAnimSequence);
	void GetAnimationPose(FAnimationPoseData& OutAnimationPoseData, const FAnimExtractContext& ExtractionContext) const;
	void GetBonePose(struct FAnimationPoseData& OutAnimationPoseData, const FAnimExtractContext& ExtractionContext, bool bForceUseRawData = false) const;

	FORCEINLINE USkeleton* GetSkeleton() const { return Skeleton; }

private:
	UPROPERTY()
	USkeleton* Skeleton;
};
