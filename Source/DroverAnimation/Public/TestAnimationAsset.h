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
private:
	FTransform ExtractTransformFromKeyIndex(const int32 InKeyIndex, const FRawAnimSequenceTrack& InTrackToExtract) const;

	FName RetargetSourceName;
	TArray<FTransform> RetargetTransforms;
	TScriptInterface<IAnimationDataModel> AnimDataModelInterface;
};
