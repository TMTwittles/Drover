// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNode_HipRotator.h"
#include "Animation/AnimInstanceProxy.h"
#include "Test_AnimInstance.h"

FAnimNode_HipRotator::FAnimNode_HipRotator()
{
}

void FAnimNode_HipRotator::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	BasePose.Initialize(Context);
}

void FAnimNode_HipRotator::Update_AnyThread(const FAnimationUpdateContext& Context)
{
	BasePose.Update(Context);

	UTest_AnimInstance* TestAnimInstance = Cast<UTest_AnimInstance>(Context.AnimInstanceProxy->GetAnimInstanceObject());

	
}

void FAnimNode_HipRotator::Evaluate_AnyThread(FPoseContext& Output)
{
	BasePose.Evaluate(Output);

	Output.
}
