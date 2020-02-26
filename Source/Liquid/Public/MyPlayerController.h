// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UObject/UObjectIterator.h"
#include "MyPlayerController.generated.h"


class AProcLiquidMesh;

UCLASS()
class LIQUID_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
		
public:

	UPROPERTY(BlueprintReadWrite, Category = Setup)
		AProcLiquidMesh* ProceduralLiquidMesh;
	
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SelectNearVerticies();
	void Stop();
	void DrawSelectingArea(float i);

	float SelectRadius = 40;

	TArray<int32> SelectedVerticies;

	FVector MouseWorldPosition, MouseWorldDirection;

	bool Interactable = false;
};
