// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNode_Test.h"

FAnimNode_Test::FAnimNode_Test()
{
}

void FAnimNode_Test::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	BasePose.Initialize(Context);
}

void FAnimNode_Test::Update_AnyThread(const FAnimationUpdateContext& Context)
{
	BasePose.Update(Context);

	ElapsedTime += Context.GetDeltaTime();
}

void FAnimNode_Test::Evaluate_AnyThread(FPoseContext& Output)
{
	BasePose.Evaluate(Output);

	FAnimExtractContext ExtractContext(TargetFrame == 0 ? 0.0 : 1.0);
	FAnimationPoseData PoseData(Output);
	Sequence->GetAnimationPose(PoseData, ExtractContext);
	ElapsedTime += 1.0f;

	if (ElapsedTime > TimeBetweenFrames)
	{
		ElapsedTime = 0.0f;
		TargetFrame = TargetFrame == 0 ? 1 : 0;
	}
}

void FAnimNode_Test::ExtractAnimPose(const double InSequenceTime, FCompactPose& OutExtractedPose)
{
	if (!Sequence)
	{
		// TODO: Add warnings.
		return;
	}

	FAnimExtractContext ExtractContext(InSequenceTime);
	FBlendedCurve Curve = FBlendedCurve(); // TODO: Find out what this does.
	UE::Anim::FStackAttributeContainer AnimAttributes = UE::Anim::FStackAttributeContainer(); // TODO: Find out what this does.

	FAnimationPoseData PoseData(OutExtractedPose, Curve, AnimAttributes);
	Sequence->GetAnimationPose(PoseData, ExtractContext);
}
