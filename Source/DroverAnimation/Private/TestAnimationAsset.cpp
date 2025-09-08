// Fill out your copyright notice in the Description page of Project Settings.


#include "TestAnimationAsset.h"
#include "Animation/AnimData/IAnimationDataModel.h"
#include "BonePose.h"

void UTestAnimationAsset::BindAnimationSequence(const UAnimSequence* InAnimSequence)
{
	TScriptInterface<IAnimationDataModel> DataModelInterface = InAnimSequence->GetDataModelInterface();
	TArray<FName> BoneTrackNames;
	DataModelInterface.GetInterface()->GetBoneTrackNames(BoneTrackNames);
}

void UTestAnimationAsset::GetAnimationPose(FAnimationPoseData& OutAnimationPoseData, const FAnimExtractContext& ExtractionContext) const
{
	FCompactPose& OutPose = OutAnimationPoseData.GetPose();
	FBoneContainer& RequiredBones = OutPose.GetBoneContainer();
	const USkeleton* TargetSkeleton = GetSkeleton();

	if (!TargetSkeleton)
	{
		OutPose.ResetToRefPose();
		return;
	}

	const double Time = ExtractionContext.CurrentTime; // TODO: Double check this.
	// TODO: Add support for interpolation types.

	// Generate keys to interpolate between
	int32 KeyIndex1, KeyIndex2;
	float Alpha;

	const int FrameRate = 60; // TODO: Check impact of this.
	const int NumberOfKeys = 2; // TODO: Check impact of this.
	FAnimationRuntime::GetKeyIndicesFromTime(KeyIndex1, KeyIndex2, Alpha, Time, FrameRate, NumberOfKeys);

	bool bShouldInterpolate = true;

	if (Alpha < UE_KINDA_SMALL_NUMBER)
	{
		Alpha = 0.f;
		bShouldInterpolate = false;
	}
	else if (Alpha > 1.f - UE_KINDA_SMALL_NUMBER)
	{
		bShouldInterpolate = false;
		KeyIndex1 = KeyIndex2;
	}
}

void UTestAnimationAsset::GetBonePose(struct FAnimationPoseData& OutAnimationPoseData, const FAnimExtractContext& ExtractionContext, bool bForceUseRawData) const
{

}