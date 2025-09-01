// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNode_Test.h"

FAnimNode_Test::FAnimNode_Test()
{
}

void FAnimNode_Test::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	BasePose.Initialize(Context);
}

void FAnimNode_Test::Evaluate_AnyThread(FPoseContext& Output)
{
	if (!bExtractedPoses)
	{
		// Get poses from anim sequences.
		ExtractAnimPose(0.0, StartPose);
		ExtractAnimPose(1.0, EndPose);

		TargetFrame = 0;
		ElapsedTime = 0.0f;
		bExtractedPoses = true;
	}

	BasePose.Evaluate(Output);
	
	ElapsedTime += DeltaTime;

	if (ElapsedTime > TimeBetweenFrames)
	{
		ElapsedTime = 0.0f;

		// TODO: Please forgive me whoever see this.
		Output.Pose = TargetFrame == 0 ? StartPose : EndPose;
		TargetFrame = TargetFrame == 0 ? 1 : 0;
	}
}

void FAnimNode_Test::ExtractAnimPose(double InSequenceTime, FCompactPose& OutExtractedPose)
{
	if (!Sequence)
	{
		// TODO: Add warnings.
		return;
	}

	FAnimExtractContext ExtractContext(0.0);
	FBlendedCurve Curve; // TODO: Find out what this does.
	UE::Anim::FStackAttributeContainer AnimAttributes; // TODO: Find out what this does.

	FAnimationPoseData PoseData(OutExtractedPose, Curve, AnimAttributes);
	Sequence->GetAnimationPose(PoseData, ExtractContext);
}
