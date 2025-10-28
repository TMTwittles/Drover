// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TweenieDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DROVERANIMATION_API UTweenieDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimSequence> SourceAnimSequence;
};
