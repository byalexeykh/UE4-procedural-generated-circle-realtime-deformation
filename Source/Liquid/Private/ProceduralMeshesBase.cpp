// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralMeshesBase.h"
#include "Components/SplineComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"

// Sets default values for this component's properties
UProceduralMeshesBase::UProceduralMeshesBase()
{
	PrimaryComponentTick.bCanEverTick = true;

	Spline2 = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SecondSpline = CreateDefaultSubobject<USplineComponent>(TEXT("SecondSpline"));
}
// Generates random  figure
void UProceduralMeshesBase::GenerateFigure(bool isHard)
{
	Spline2->ClearSplinePoints(true);
	int32 rnd = FMath::RandRange(1, 5);
	int32 HighRnd = FMath::RandRange(-20, 20);
	int32 HighRnd2 = FMath::RandRange(-20, 20);
	if (isHard) 
	{
		switch (rnd) {
		case 1:
			GenerateTriangle();
			break;
		case 2:
			GenerateTriangle();
			break;
		case 3:
			GenerateTriangle();
			break;
		case 4:
			GenerateTriangle();
			break;
		case 5:
			GenerateTriangle();
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("ProceduralMeshesBase - GenerateFigure - if - default"));
			break;
		}
	}
	else 
	{
		switch (rnd) {
		case 1:
			if (HighRnd == HighRnd2) {
				GenerateFivePointedStar();
			}
			else {
				GenerateTriangle();
			}
			break;
		case 2:
			if (HighRnd == HighRnd2) {
				GenerateFourPointedStar();
			}
			else {
				GenerateQuadrangle();
			}
			break;
		case 3:
			if (HighRnd == HighRnd2) {
				GenerateHexagon();
			}
			else {
				GenerateRhombus();
			}
			break;
		case 4:
			if (HighRnd == HighRnd2) {
				GenerateSixPointedStar();
			}
			else {
				GeneratePentagon();
			}
			break;
		case 5:
			if (HighRnd == HighRnd2) {
				GenerateHexagon();
			}
			else {
				GenerateCircle();
			}
			break;
		}
	}
}
// Generates selected figure (for debug mostly)
void UProceduralMeshesBase::GenerateFigure(int32 FigureNum)
{
	Spline2->ClearSplinePoints(true);
	switch (FigureNum) {
	case 1:
		GenerateTriangle();
		break;
	case 2:
		GenerateQuadrangle();
		break;
	case 3:
		GeneratePentagon();
		break;
	case 4:
		GenerateCircle();
		break;
	case 5:
		GenerateHexagon();
		break;
	case 6:
		GenerateFourPointedStar();
		break;
	case 7:
		GenerateFivePointedStar();
		break;
	case 8:
		GenerateSixPointedStar();
		break;
	case 9:
		GenerateRhombus();
		break;
	default:
		break;
	}
}
// deletes generated figure
void UProceduralMeshesBase::DeleteFigure()
{
	Spline2->ClearSplinePoints(true);
	SplinePointsIndex = 0;
}


// Light-Mode

//service for GenerateTriangle()
void UProceduralMeshesBase::GenerateTriangle(FVector V1, FVector V2, FVector V3)
{
	Spline2->AddSplinePoint(V1, ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(V2, ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(V3, ESplineCoordinateSpace::World, true);
	Spline2->SetClosedLoop(true, true);
	SplinePointsIndex += 2;
}
void UProceduralMeshesBase::GenerateTriangle()
{
	GenerateTriangle(
		AddPoint(100 + PointOffset(), -70 + PointOffset()), 
		AddPoint(-100 + PointOffset(), -70 + PointOffset()), 
		AddPoint(0 + PointOffset(), 100 + PointOffset())
	);
	CopyToSecondSpline();
	for (int i = 0; i < 3; i++) {
		Spline2->SetSplinePointType(i, ESplinePointType::Linear, true);
	}
	UE_LOG(LogTemp, Warning, TEXT("Triangle generated..."));
}
void UProceduralMeshesBase::GenerateQuadrangle()
{
	Spline2->AddSplinePoint(AddPoint(100 + PointOffset(), 100 + PointOffset()), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-100 + PointOffset(), 100 + PointOffset()), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-100 + PointOffset(), -100 + PointOffset()), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(100 + PointOffset(), -100 + PointOffset()), ESplineCoordinateSpace::World, true);
	Spline2->SetClosedLoop(true, true);
	SplinePointsIndex += 3;
	CopyToSecondSpline();
	for (int i = 0; i < SplinePointsIndex + 1; i++) {
		Spline2->SetSplinePointType(i, ESplinePointType::Linear, true);
	}
}
void UProceduralMeshesBase::GenerateRhombus()
{
	Spline2->AddSplinePoint(AddPoint(0 + PointOffset() / 3, 100 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(100 + PointOffset() / 3, 0 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(0 + PointOffset() / 3, -100 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-100 + PointOffset() / 3, 0 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->SetClosedLoop(true, true);
	SplinePointsIndex += 3;
	CopyToSecondSpline();
	for (int i = 0; i < SplinePointsIndex + 1; i++) {
		Spline2->SetSplinePointType(i, ESplinePointType::Linear, true);
	}
}
void UProceduralMeshesBase::GeneratePentagon()
{
	Spline2->AddSplinePoint(AddPoint(0 + PointOffset(), 100 + PointOffset()), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(100 + PointOffset(), 50 + PointOffset()), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(50 + PointOffset(), -100 + PointOffset()), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-50 + PointOffset(), -100 + PointOffset()), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-100 + PointOffset(), 50 + PointOffset()), ESplineCoordinateSpace::World, true);
	Spline2->SetClosedLoop(true, true);
	SplinePointsIndex += 4;
	CopyToSecondSpline();
	for (int i = 0; i < SplinePointsIndex + 1; i++) {
		Spline2->SetSplinePointType(i, ESplinePointType::Linear, true);
	}
}
void UProceduralMeshesBase::GenerateCircle()
{
	float angleBetweenPointsRads = (360 / 40) * PI / 180;
	float currentAngleRads = 0;
	float Radius = 90 + PointOffset();
	for (int i = 0; i < 40; i++)
	{
		float Y = Radius * cos(currentAngleRads);
		float Z = Radius * sin(currentAngleRads);

		Spline2->AddSplinePoint(AddPoint(Y, Z), ESplineCoordinateSpace::World, true);

		SplinePointsIndex++;

		currentAngleRads += angleBetweenPointsRads;
	}
	SplinePointsIndex--;
	CopyToSecondSpline();
	for (int i = 0; i < SplinePointsIndex + 1; i++) {
		Spline2->SetSplinePointType(i, ESplinePointType::Linear, true);
	}
	Spline2->SetClosedLoop(true, true);
}

// Hard-mode

void UProceduralMeshesBase::GenerateHexagon()
{
	Spline2->AddSplinePoint(AddPoint(0 + PointOffset() / 3, 100 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(100 + PointOffset() / 3, 50 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(100 + PointOffset() / 3, -50 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(0 + PointOffset() / 3, -100 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-100 + PointOffset() / 3, -50 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-100 + PointOffset() / 3, 50 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->SetClosedLoop(true, true);
	SplinePointsIndex += 5;
	CopyToSecondSpline();
	for (int i = 0; i < SplinePointsIndex + 1; i++) {
		Spline2->SetSplinePointType(i, ESplinePointType::Linear, true);
	}
}
void UProceduralMeshesBase::GenerateFourPointedStar()
{
	Spline2->AddSplinePoint(AddPoint(0 + PointOffset() / 3, 100 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(30 + PointOffset() / 3, 30 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(100 + PointOffset() / 3, 0 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(30 + PointOffset() / 3, -30 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(0 + PointOffset() / 3, -100 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-30 + PointOffset() / 3, -30 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-100 + PointOffset() / 3, 0 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-30 + PointOffset() / 3, 30 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->SetClosedLoop(true, true);
	SplinePointsIndex += 7;
	CopyToSecondSpline();
	for (int i = 0; i < SplinePointsIndex + 1; i++) {
		Spline2->SetSplinePointType(i, ESplinePointType::Linear, true);
	}
}
void UProceduralMeshesBase::GenerateFivePointedStar()
{
	Spline2->AddSplinePoint(AddPoint(0 + PointOffset() / 4, 100 + PointOffset() / 4), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(30 + PointOffset() / 4, 30 + PointOffset() / 4), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(100 + PointOffset() / 4, 30 + PointOffset() / 4), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(50 + PointOffset() / 4, -30 + PointOffset() / 4), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(70 + PointOffset() / 4, -100 + PointOffset() / 4), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(0 + PointOffset() / 4, -50 + PointOffset() / 4), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-70 + PointOffset() / 4, -100 + PointOffset() / 4), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-50 + PointOffset() / 4, -30 + PointOffset() / 4), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-100 + PointOffset() / 4, 30 + PointOffset() / 4), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-30 + PointOffset() / 4, 30 + PointOffset() / 4), ESplineCoordinateSpace::World, true);
	Spline2->SetClosedLoop(true, true);
	SplinePointsIndex += 9;
	CopyToSecondSpline();
	for (int i = 0; i < SplinePointsIndex + 1; i++) {
		Spline2->SetSplinePointType(i, ESplinePointType::Linear, true);
	}
}
void UProceduralMeshesBase::GenerateSixPointedStar() // TODO вместо 6-конечной спавнится 5-и конечная
{
	Spline2->AddSplinePoint(AddPoint(0 + PointOffset() / 3, 100 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(30 + PointOffset() / 3, 50 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(100 + PointOffset() / 3, 50 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(70 + PointOffset() / 3, 0 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(100 + PointOffset() / 3, -50 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(30 + PointOffset() / 3, -50 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(0 + PointOffset() / 3, -100 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-30 + PointOffset() / 3, -50 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-100 + PointOffset() / 3, -50 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-70 + PointOffset() / 3, 0 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-100 + PointOffset() / 3, 50 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->AddSplinePoint(AddPoint(-30 + PointOffset() / 3, 50 + PointOffset() / 3), ESplineCoordinateSpace::World, true);
	Spline2->SetClosedLoop(true, true);
	SplinePointsIndex += 11;
	CopyToSecondSpline();
	for (int i = 0; i < SplinePointsIndex + 1; i++) {
		Spline2->SetSplinePointType(i, ESplinePointType::Linear, true);
	}
}

// Service functions

void UProceduralMeshesBase::CopyToSecondSpline()
{
	SecondSpline->ClearSplinePoints(true);
	for (int i = 0; i <= SplinePointsIndex; i++) {
		SecondSpline->AddSplinePoint(Spline2->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World), ESplineCoordinateSpace::World, true);
		SecondSpline->SetSplinePointType(i, ESplinePointType::Linear, true);
	}
	SecondSpline->SetClosedLoop(true, true);
}

FVector UProceduralMeshesBase::AddPoint(float Y, float Z)
{
	return FVector(0, Y, Z);
}

float UProceduralMeshesBase::PointOffset()
{
	return FMath::RandRange(-20, 20);
}

