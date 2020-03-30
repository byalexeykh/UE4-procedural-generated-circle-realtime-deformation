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
		
	UFUNCTION(BlueprintImplementableEvent, Category = "Figure Visual Presentation")
		void OnGenerateTaskShape();

	UFUNCTION(BlueprintImplementableEvent, Category = "Core Gameplay")
		void OnCorrectShapeDrawed();

	UFUNCTION(BlueprintImplementableEvent, Category = "Core Gameplay")
		void OnIncorrectShapeDrawed();

	UFUNCTION(BlueprintCallable, Category = "Figure Visual Presentation")
		TArray<FVector> GetTaskShapePoints();

	UFUNCTION(BlueprintCallable, Category = "Figure Visual Presentation")
		int GetSplinePointsLastIndex();

	UPROPERTY(BlueprintReadWrite, Category = Setup)
		AProcLiquidMesh* ProceduralLiquidMesh;

	UProceduralMeshesBase* ProcMeshBase;

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetInteractState();
	void Stop();
	void DrawSelectingArea();
	void DrawDebugText(FVector loc, FString text, AActor* mesh);
	void MoveX(float AxisValue);
	void MoveY(float AxisValue);
	void DrawSpline(FVector HitResult);
	void CorrectVertexPos();
	void GenerateTaskShape();

	
	int32 CurrentSplinePoint = 0;

	float SelectRadius = 10;
	float DistanceBetweenPoints = 30;
	float MaxDeviationFromTaskShape = 10;
	float MaxIncorrectPoints = 10;
	float IncorrectPoints = 0;

	float Distance;
	float SmallestDistance;
	float DistanceBetweenVerticies;
	float DecreaseShiftByDistance(float Distance);

	bool Interactable = false;

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

	FCollisionQueryParams CollisionQueryParams;

	AActor* DebugActor;

};
