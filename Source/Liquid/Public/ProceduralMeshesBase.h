// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProceduralMeshComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "ProceduralMeshesBase.generated.h"

class USplineComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIQUID_API UProceduralMeshesBase : public UActorComponent
{
	GENERATED_BODY()

private:
	
	// Light-mode

	void GenerateTriangle(FVector V1, FVector V2, FVector V3);
	void GenerateTriangle();
	void GenerateQuadrangle();
	void GenerateRhombus();
	void GeneratePentagon();
	void GenerateCircle();
	
	// Hard-mode

	void GenerateHexagon();
	void GenerateFourPointedStar();
	void GenerateFivePointedStar();
	void GenerateSixPointedStar();

	// Service

	void CopyToSecondSpline();

	FVector AddPoint(float Y, float Z);

	float PointOffset();

public:	
	// Sets default values for this component's properties
	UProceduralMeshesBase();

	int32 SplinePointsIndex = 0;
	int32 CurrentDifficulty;
	
	/**
	 * Generates random figure
	 * @param	isHard: true - hard | false - light
	 */
	void GenerateFigure(bool isHard);

	/**
	 * Deletes generateg figure
	 */
	void DeleteFigure();
	
	/**
	 * Generates specific figure for debug
	 * @param	FigureName: 1 - Triangle,  2 - Quadrangle, 3 - Pentagon, 4 - Circle, 5 - Hexagon, 6 - FourStar, 7 - FiveStar, 8 - SixStar, 9 - Rhombus
	 */
	void GenerateFigure(int32 FigureNum);
	
	USplineComponent* Spline2;

	/**
	* SecondSpline is used for figure disappear animation
	*/
	USplineComponent* SecondSpline;
};
