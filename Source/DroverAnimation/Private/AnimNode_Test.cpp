// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNode_Test.h"
#include "Animation/AnimInstanceProxy.h"
#include "Test_AnimInstance.h"

FAnimNode_Test::FAnimNode_Test()
{
	int32 A = 0;
}

void FAnimNode_Test::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	BasePose.Initialize(Context);

	

	bValidPoses = IsValidPoses();

	if (!bValidPoses) { return; }

	CurrPoseIndex = 0;
	TargetPoseIndex = CurrPoseIndex + 1;
}

void FAnimNode_Test::Update_AnyThread(const FAnimationUpdateContext& Context)
{
	BasePose.Update(Context);

	if (!bValidPoses) { return; }

	UTest_AnimInstance* TestAnimInstance = Cast<UTest_AnimInstance>(Context.AnimInstanceProxy->GetAnimInstanceObject());
	
	VelocityNormalized = TestAnimInstance->CurrentSpeed / TestAnimInstance->MaxSpeed;
	
	// Update internal time accumulator.
	InternalTimeAccumulator += Context.GetDeltaTime();
	
	const float InverseVelocityNormalized = 1.0f - VelocityNormalized;
	TargetStrideLength = FMath::Clamp(InverseVelocityNormalized * MaxStrideLength, MinStrideLength, MaxStrideLength);

	if (InternalTimeAccumulator >= TargetStrideLength)
	{
		InternalTimeAccumulator = 0.0f;
		CurrPoseIndex = TargetPoseIndex;
		const bool bLoopPoseIndex = TargetPoseIndex == PoseIndexes.Num() - 1;
		TargetPoseIndex = bLoopPoseIndex ? 0 : TargetPoseIndex + 1;
	}
}

void FAnimNode_Test::Evaluate_AnyThread(FPoseContext& Output)
{
	FPoseContext BaseContext(Output);

	if (!bValidPoses)
	{
		//Output = BaseContext;
		return;
	}

	BasePose.Evaluate(Output);

	FPoseContext ToPose(BaseContext);
	FPoseContext FromPose(BaseContext);
	FAnimationPoseData ToPoseData(ToPose);
	FAnimationPoseData FromPoseData(FromPose);
	ExtractPoses(ToPoseData, FromPoseData);

	const float Alpha = InternalTimeAccumulator / TargetStrideLength;
	
	FAnimationPoseData OutputPoseData(Output);

	FAnimationRuntime::BlendTwoPosesTogether(ToPoseData, FromPoseData, Alpha, OutputPoseData);

	Output.Pose = OutputPoseData.GetPose();
}

void FAnimNode_Test::ExtractPoses(FAnimationPoseData& OutData01, FAnimationPoseData& OutData02)
{
	FAnimExtractContext ExtractContext;
	FPoseCurve PoseCurve;
	ExtractContext.PoseCurves.Add(PoseCurve);
	
	ExtractContext.PoseCurves[0].PoseIndex = PoseIndexes[CurrPoseIndex];
	ExtractContext.PoseCurves[0].Value = 1.0;
	PoseAsset->GetAnimationPose(OutData01, ExtractContext);
	
	ExtractContext.PoseCurves[0].PoseIndex = PoseIndexes[TargetPoseIndex];
	ExtractContext.PoseCurves[0].Value = 1.0;
	PoseAsset->GetAnimationPose(OutData02, ExtractContext);
}
