// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FHitResult;
struct FCollisionQueryParams;
enum ECollisionChannel;
struct FCollsionShape;

/**
 * Handles sliding logic for complex collisions.
 * Implementation of this task follows LittlePolygon sliding article
 * https://blog.littlepolygon.com/posts/sliding/
 */
class DROVER_API FSlideHandler
{
public:
	FSlideHandler();
	~FSlideHandler();

	bool TryStepPosition(const UWorld* InWorld);

	struct DROVER_API FCollisionConfig
	{
		FCollisionShape Shape;
		FQuat Rotation = FQuat::Identity;
		FCollisionQueryParams QueryParams;
		ECollisionChannel Channel;
	};
	FCollisionConfig CollisionConfig;
	FVector Position;
	FVector RemainingSlideDelta;

private:
	bool InflatedAndPenetrationAdjustedSweep(const UWorld* InWorld, FHitResult& OutHitResult);
	FCollisionShape InflateShape(const FCollisionShape& Shape, float Amount);
	FORCEINLINE bool PerformSweepSingle(const UWorld* InWorld, const FVector& TraceStart, const FVector& TraceEnd, FHitResult& OutHitResult);
	FORCEINLINE bool PerformSweepMulti(const UWorld* InWorld, const FVector& TraceStart, const FVector& TraceEnd, const FCollisionShape& InShape, TArray<FHitResult>& OutHitResults);
	const float CollisionEpsilon = 0.125f;
};
