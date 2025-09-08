// Fill out your copyright notice in the Description page of Project Settings.


#include "TestAnimationAsset.h"
#include "Animation/AnimData/IAnimationDataModel.h"
#include "BonePose.h"
#include "AnimEncoding.h"

void UTestAnimationAsset::BindAnimationSequence(const UAnimSequence* InAnimSequence)
{
	TScriptInterface<IAnimationDataModel> DataModelInterface = InAnimSequence->GetDataModelInterface();
	TArray<FName> BoneTrackNames;
	DataModelInterface.GetInterface()->GetBoneTrackNames(BoneTrackNames);
}

void UTestAnimationAsset::GetAnimationPose(FAnimationPoseData& OutAnimationPoseData, const FAnimExtractContext& ExtractionContext) const
{
	FCompactPose& OutPose = OutAnimationPoseData.GetPose();
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

	// TODO: Add support for reading curves. 
	const TArray<FBoneAnimationTrack> BoneAnimationTracks; // TODO: Extract this during bind anim sequnce.

	const int32 NumAnimationTracks = BoneAnimationTracks.Num();
	const FBoneContainer& RequiredBones = OutPose.GetBoneContainer();

	// TODO: Add support for reading virtual bones?

	FCompactPose Key2Pose;
	Key2Pose.CopyBonesFrom(OutPose);

	for (const FBoneAnimationTrack& AnimationTrack : BoneAnimationTracks)
	{
		// Access animation track respective skeleton bone.
		const int32 SkeletonBoneIndex = AnimationTrack.BoneTreeIndex;

		if (SkeletonBoneIndex == INDEX_NONE && SkeletonBoneIndex > MAX_BONES)
		{
			// TODO: Add warnings / error logging here. 
			continue;
		}

		// TODO: This method is deprecated, should use GetCompactPoseIndexFromSkeletonPoseIndex instead.
		const FCompactPoseBoneIndex PoseBoneIndex = RequiredBones.GetCompactPoseIndexFromSkeletonIndex(SkeletonBoneIndex);

		if (PoseBoneIndex == INDEX_NONE)
		{
			// TODO: Add warning / error logging here.
			continue;
		}

		// Extract raw animation data.
		const FRawAnimSequenceTrack& TrackToExtract = AnimationTrack.InternalTrackData;
	}
}

void UTestAnimationAsset::GetBonePose(struct FAnimationPoseData& OutAnimationPoseData, const FAnimExtractContext& ExtractionContext, bool bForceUseRawData) const
{

}