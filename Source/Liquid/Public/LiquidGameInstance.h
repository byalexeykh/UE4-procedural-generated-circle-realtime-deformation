// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LiquidGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LIQUID_API ULiquidGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	TArray<FVector> Verticies;
};
