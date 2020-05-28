// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class AProcLiquidMesh;

UCLASS()
class LIQUID2_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void HandleInput();

	float Force = 50000, ForceOffset = 0.1;

	AProcLiquidMesh* ProceduralLiquidMesh;

	FVector TouchPos;

public:
	UFUNCTION(BlueprintCallable, Category = "Core Gameplay")
		FVector PassCoordsToCode(float Y, float Z);
};
