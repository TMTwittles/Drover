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
	BasePose.Evaluate(Output);
	
	if (Sequence)
	{
		FAnimExtractContext ExtractContext(0.0);
		FAnimationPoseData PoseData(Output);
		Sequence->GetAnimationPose(PoseData, ExtractContext);
	}
}
