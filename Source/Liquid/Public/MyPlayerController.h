// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UObject/UObjectIterator.h"
#include "MyPlayerController.generated.h"

class UProceduralMeshesBase;
class AProcLiquidMesh;

UCLASS()
class LIQUID_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
		
public:
		
	UFUNCTION(BlueprintCallable, Category = "Core Gameplay")
		float GetTimeInPercent();

	UFUNCTION(BlueprintCallable, Category = "Core Gameplay")
		void DrawSpline(FVector HitResult);

	UFUNCTION(BlueprintCallable, Category = "Core Gameplay")
		FVector PassCoordsToCode(float Y, float Z); //TODO передавать координаты из БП в код посредством этой функции 

	UFUNCTION(BlueprintCallable, Category = "Core Gameplay")
		void SetIntrractState();

	UFUNCTION(BlueprintCallable, Category = "Core Gameplay")
		void Stop();

	UFUNCTION(BlueprintCallable, Category = "Core Gameplay")
		bool isGameStartedFunc();

	UFUNCTION(BlueprintCallable, Category = "Core Gameplay")
		void StartPlay();

	UFUNCTION(BlueprintCallable, Category = "Core Gameplay")
		void GenerateTaskShape();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Core Gameplay")
		void OnGenerateTaskShape();

	UFUNCTION(BlueprintImplementableEvent, Category = "Core Gameplay")
		void OnStartPlay();

	UFUNCTION(BlueprintImplementableEvent, Category = "Core Gameplay")
		void OnCorrectShapeDrawed();

	UFUNCTION(BlueprintImplementableEvent, Category = "Core Gameplay")
		void OnIncorrectShapeDrawed();

	UFUNCTION(BlueprintImplementableEvent, Category = "Core Gameplay")
		void OnLoose();

	UFUNCTION(BlueprintCallable, Category = "Figure Visual Presentation")
		TArray<FVector> GetTaskShapePoints();

	UFUNCTION(BlueprintCallable, Category = "Figure Visual Presentation")
		TArray<FVector> GetSecondSplinePoints();
	
	UFUNCTION(BlueprintCallable, Category = "Figure Visual Presentation")
		int GetSplinePointsLastIndex();

	UFUNCTION(BlueprintCallable, Category = "Figure Visual Presentation")
		void DeleteSecondSpline();

	UFUNCTION(BlueprintImplementableEvent, Category = "Figure Visual Presentation")
		void OnGenerateSecondSpline();

	UPROPERTY(BlueprintReadWrite, Category = Setup)
		AProcLiquidMesh* ProceduralLiquidMesh;

	UProceduralMeshesBase* ProcMeshBase;

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void DrawSelectingArea();
	void DrawDebugText(FVector loc, FString text, AActor* mesh);
	void MoveX(float AxisValue);
	void MoveY(float AxisValue);
	
	void CorrectVertexPos();
	void DecreaseTimerValue();
	
	int32 CurrentSplinePoint = 0;
	int32 MaxIncorrectPoints2 = 5;
	int32 IncorrectPoints2 = 0;
	int32 IncorrectShapesDrawed = 0;
	int32 CorrectShapeDrawedCombo = 0;
	const int32 IncorrectShapesDrawedMax = 5;

	float PointsPlaced = 0;
	float SelectRadius = 10;
	float DistanceBetweenPoints = 30;
	float MaxDeviationFromTaskShape = 50;
	const float TimerInit = 8;

	float TimeLast;
	float TimerDecrease;
	float TimerCurrent;
	float Distance;
	float SmallestDistance;
	float DistanceBetweenVerticies;
	float DecreaseShiftByDistance(float Distance);

	bool isGameStarted = false;
	bool Interactable = false;
	/**
	*	false - light | true - hard
	*/
	bool isHard = false;

	bool IsDirDeviationLimit(int32 Key);

	TMap<int32, FVector> SplinePointsMap;
	TMap<int32, float> VerticiesToSelectCenterDistMap;
	TMap<int32, bool> SelectedVerticiesMap;
	TMap<int32, bool> IsSplinePointOccupied;
	TMap<int32, bool> PosedVerticies;

	TArray<FVector> SplinePoints;

	FVector MouseWorldPosition, MouseWorldDirection;

	FVector ConvertTo2D(FVector vectorToConvert);

	FHitResult HitResult;

	FString ActorName, RequiredName = "Actor_1";

	AActor* DebugActor;

};
