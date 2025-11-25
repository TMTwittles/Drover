// Fill out your copyright notice in the Description page of Project Settings.
#include "SlideHandler.h"

FSlideHandler::FSlideHandler()
{
	Position = FVector::Zero();
	RemainingSlideDelta = FVector::Zero();
}

FSlideHandler::~FSlideHandler()
{
	
}

bool FSlideHandler::TryStepPosition(
	const UWorld* InWorld, const FVector& InPosition, const FVector& InRemainingSlideDelta,
	const int32& InNumIterations, FVector& OutSteppedPosition)
{
	Position = InPosition;
	RemainingSlideDelta = InRemainingSlideDelta;
	bool bValidPositionFound = false;
	for (int32 StepIter = 0; StepIter < InNumIterations; ++StepIter)
	{
		bValidPositionFound = TryStepPosition(InWorld);
		if (!bValidPositionFound) break;
	}
	OutSteppedPosition = Position;
	return bValidPositionFound;
}

bool FSlideHandler::TryStepPosition(const UWorld* InWorld)
{
	FHitResult Hit;
	const bool bHit = InflatedAndPenetrationAdjustedSweep(InWorld, Hit);

	// Nothing in the way, safely slide to the end, set remainder to 0.
	if (!bHit)
	{
		Position = Hit.TraceEnd;
		RemainingSlideDelta = FVector::Zero();
		return bHit;
	}

	// Are we overlapping another object.
	if (Hit.bStartPenetrating)
	{
		RemainingSlideDelta = FVector::Zero();
		return bHit;
	}

	auto PullBack = [&](const FVector& InVector)
		{
			const float Distance = InVector.Size();

			if (Distance > CollisionEpsilon)
			{
				const float ThresholdSafeDistance = ((Distance - CollisionEpsilon) / Distance);
				return InVector * ThresholdSafeDistance;
			}

			return FVector::Zero();
		};

	// Update position and remainder
	// To deal with arithmetic and float point errors, we cannot directly set the position to the Hit.Location,
	// rather we need to pull back the by small epsilon. If we dont character will always be stuck. 
	Position = Hit.TraceStart + PullBack(Hit.Location - Hit.TraceStart);
	RemainingSlideDelta *= (1.f - Hit.Time);
	RemainingSlideDelta = FVector::VectorPlaneProject(RemainingSlideDelta, Hit.Normal);
    return bHit;
}

FCollisionShape FSlideHandler::InflateShape(const FCollisionShape& Shape, float Amount)
{
	switch (Shape.ShapeType)
	{
	case ECollisionShape::Capsule:
		return FCollisionShape::MakeCapsule(Shape.GetCapsuleRadius() + Amount, Shape.GetCapsuleHalfHeight() + Amount);
	case ECollisionShape::Sphere:
		return FCollisionShape::MakeSphere(Shape.GetSphereRadius() + Amount);
	case ECollisionShape::Box:
		return FCollisionShape::MakeBox(Shape.GetBox() + Amount);
	default:
		return Shape;
	}
}

bool FSlideHandler::PerformSweepSingle(const UWorld* InWorld, const FVector& TraceStart, const FVector& TraceEnd, FHitResult& OutHitResult)
{
	return InWorld->SweepSingleByChannel(OutHitResult, TraceStart, TraceEnd, CollisionConfig.Rotation, CollisionConfig.Channel, CollisionConfig.Shape, CollisionConfig.QueryParams);
}

bool FSlideHandler::PerformSweepMulti(const UWorld* InWorld, const FVector& TraceStart, const FVector& TraceEnd, const FCollisionShape& InShape, TArray<FHitResult>& OutHitResults)
{
	return InWorld->SweepMultiByChannel(OutHitResults, TraceStart, TraceEnd, CollisionConfig.Rotation, CollisionConfig.Channel, InShape, CollisionConfig.QueryParams);
}

bool FSlideHandler::InflatedAndPenetrationAdjustedSweep(const UWorld* InWorld, FHitResult& OutHitResult)
{
	FCollisionShape InflatedCollisionShape = InflateShape(CollisionConfig.Shape, 0.25f);
	TArray<FHitResult> InflatedHitResults;
	const bool bInflatedHit = PerformSweepMulti(InWorld, Position, Position + RemainingSlideDelta, InflatedCollisionShape, InflatedHitResults);

	// No hits, even with inflated shape.
	if (!bInflatedHit)
	{
		OutHitResult.TraceStart = Position;
		OutHitResult.TraceEnd = Position + RemainingSlideDelta;
		OutHitResult.Location = Position + RemainingSlideDelta;
		OutHitResult.Time = 1.0f;
		OutHitResult.bBlockingHit = 0;
		OutHitResult.bStartPenetrating = 0;
		return OutHitResult.bBlockingHit;
	}

	// Determine if any hits were overlapping with another collider.
	{
		int NumHitsInPenetration = 0;
		for (const FHitResult& InflatedHit : InflatedHitResults)
		{
			if (InflatedHit.bStartPenetrating)
			{
				++NumHitsInPenetration;
			}
		}

		if (NumHitsInPenetration == 0)
		{
			OutHitResult = MoveTemp(InflatedHitResults.Last());
			return OutHitResult.bBlockingHit;
		}
	}

	// Iteratively resolve penetrations
	FVector PenetrationAdjustment = FVector::Zero();
	const int MaxNumIterations = 16;
	for (int Iter = 0; Iter < MaxNumIterations; ++Iter)
	{
		float ErrorSum = 0.0f;
		for (const FHitResult& Hit : InflatedHitResults)
		{
			const float SafePenetrationDepth = Hit.PenetrationDepth + CollisionEpsilon;
			// take the dot-product of the Fixed and the Normal to determine how much
			// of the penetration has already been taken care of.
			const float Error = FMath::Max(0.0f, SafePenetrationDepth - (FVector::DotProduct(PenetrationAdjustment, Hit.Normal)));
			ErrorSum += Error;
			// Add error amount to the penetration adjustment vector to ensure output vector no longer overlaps with another collider.
			PenetrationAdjustment += Error * Hit.Normal;
		}
		// If we've found a solution, stop iterating.
		if (ErrorSum < UE_KINDA_SMALL_NUMBER)
		{
			break;
		}
	}

	const FVector PenetrationAdjustedStartTrace = Position + PenetrationAdjustment;
	const FVector PenetrationAdjustedEndTrace = Position + PenetrationAdjustment + RemainingSlideDelta;
	return PerformSweepSingle(InWorld, PenetrationAdjustedStartTrace, PenetrationAdjustedEndTrace, OutHitResult);
}
