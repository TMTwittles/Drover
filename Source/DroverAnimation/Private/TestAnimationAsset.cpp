// Fill out your copyright notice in the Description page of Project Settings.


#include "TestAnimationAsset.h"
#include "Animation/AnimData/IAnimationDataModel.h"
#include "BonePose.h"
#include "AnimEncoding.h"
#include "Animation/AnimSequenceHelpers.h"

void UTestAnimationAsset::BindAnimationSequence(const UAnimSequence* InAnimSequence)
{
	AnimDataModelInterface = InAnimSequence->GetDataModelInterface();
	RetargetSourceName = InAnimSequence->GetRetargetTransformsSourceName();
	RetargetTransforms = InAnimSequence->GetRetargetTransforms();
	SetSkeleton(InAnimSequence->GetSkeleton());
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

	UE::Anim::EvaluateFloatCurvesFromModel(AnimDataModelInterface.GetInterface(), OutAnimationPoseData.GetCurve(), ExtractionContext.CurrentTime);

	TMap<FName, const FTransformCurve*> ActiveCurves;
	for (const FTransformCurve& Curve : AnimDataModelInterface->GetTransformCurves())
	{
		// if disabled, do not handle
		if (Curve.GetCurveTypeFlag(AACF_Disabled))
		{
			continue;
		}

		// note we're not checking Curve.GetCurveTypeFlags() yet
		ActiveCurves.FindOrAdd(Curve.GetName(), &Curve);
	}


	const TArray<FBoneAnimationTrack> BoneAnimationTracks = AnimDataModelInterface->GetBoneAnimationTracks();

	const int32 NumAnimationTracks = BoneAnimationTracks.Num();
	const FBoneContainer& RequiredBones = OutPose.GetBoneContainer();

	// TODO: Add support for reading virtual bones?

	FCompactPose Key2Pose;
	Key2Pose.CopyBonesFrom(OutPose);

	// Build retargeting scope.
	const UE::Anim::DataModel::FEvaluationContext EvaluationContext(
		ExtractionContext.CurrentTime,
		AnimDataModelInterface->GetFrameRate(),
		RetargetSourceName,
		RetargetTransforms, 
		EAnimInterpolationType::Linear);
	UE::Anim::Retargeting::FRetargetingScope RetargetingScope(GetSkeleton(), OutPose, EvaluationContext);

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
		// Do we need curve data here?
		// TODO: Hate the auto declaration here, can we fix this
		auto OverrideTransform = ActiveCurves.Find(AnimationTrack.Name); 

		{
			// Bail out (with rather wacky data) if data is empty for some reason.
			if (TrackToExtract.PosKeys.Num() == 0 || TrackToExtract.RotKeys.Num() == 0)
			{
				OutPose[PoseBoneIndex].SetIdentity();
				Key2Pose[PoseBoneIndex].SetIdentity();
			}
			else
			{
				OutPose[PoseBoneIndex] = ExtractTransformFromKeyIndex(KeyIndex1, TrackToExtract);
				Key2Pose[PoseBoneIndex] = ExtractTransformFromKeyIndex(KeyIndex2, TrackToExtract);
			}
		}

		if (OverrideTransform)
		{
			{
				const float TimePerFrame = AnimDataModelInterface->GetFrameRate().AsInterval();
				const FTransform PoseOneAdditive = (*OverrideTransform)->Evaluate(KeyIndex1 * TimePerFrame, 1.0f);
				const FTransform PoseOneLocalTransform = OutPose[PoseBoneIndex];
				OutPose[PoseBoneIndex].SetRotation(PoseOneLocalTransform.GetRotation() * PoseOneAdditive.GetRotation());
				OutPose[PoseBoneIndex].SetTranslation(PoseOneLocalTransform.TransformPosition(PoseOneAdditive.GetTranslation()));
				OutPose[PoseBoneIndex].SetScale3D(PoseOneLocalTransform.GetScale3D() * PoseOneAdditive.GetScale3D());

				const FTransform PoseTwoAdditive = (*OverrideTransform)->Evaluate(KeyIndex2 * TimePerFrame, 1.f);
				const FTransform PoseTwoLocalTransform = Key2Pose[PoseBoneIndex];
				Key2Pose[PoseBoneIndex].SetRotation(PoseTwoLocalTransform.GetRotation() * PoseTwoAdditive.GetRotation());
				Key2Pose[PoseBoneIndex].SetTranslation(PoseTwoLocalTransform.TransformPosition(PoseTwoAdditive.GetTranslation()));
				Key2Pose[PoseBoneIndex].SetScale3D(PoseTwoLocalTransform.GetScale3D() * PoseTwoAdditive.GetScale3D());
			}
		}

		RetargetingScope.AddTrackedBone(PoseBoneIndex, SkeletonBoneIndex);
	}

	// TODO: Build virtual bones?

	for (FCompactPoseBoneIndex BoneIndex : OutPose.ForEachBoneIndex())
	{
		OutPose[BoneIndex].Blend(OutPose[BoneIndex], Key2Pose[BoneIndex], Alpha);
	}
}

FTransform UTestAnimationAsset::ExtractTransformFromKeyIndex(const int32 InKeyIndex, const FRawAnimSequenceTrack& InTrackToExtract) const
{
	static const FVector DefaultScale3D = FVector(1.f);
	const bool bHasScaleKey = InTrackToExtract.ScaleKeys.Num() > 0;

	const int32 PosKeyIndex = FMath::Min(InKeyIndex, InTrackToExtract.PosKeys.Num() - 1);
	const int32 RotKeyIndex = FMath::Min(InKeyIndex, InTrackToExtract.RotKeys.Num() - 1);
	if (bHasScaleKey)
	{
		const int32 ScaleKeyIndex = FMath::Min(InKeyIndex, InTrackToExtract.ScaleKeys.Num() - 1);
		return FTransform(FQuat(InTrackToExtract.RotKeys[RotKeyIndex]), FVector(InTrackToExtract.PosKeys[PosKeyIndex]), FVector(InTrackToExtract.ScaleKeys[ScaleKeyIndex]));
	}
	else
	{
		return FTransform(FQuat(InTrackToExtract.RotKeys[RotKeyIndex]), FVector(InTrackToExtract.PosKeys[PosKeyIndex]), DefaultScale3D);
	}
}
