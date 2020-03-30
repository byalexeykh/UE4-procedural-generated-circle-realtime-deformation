// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralMeshesBase.h"
#include "Components/SplineComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

// Sets default values for this component's properties
UProceduralMeshesBase::UProceduralMeshesBase()
{
	PrimaryComponentTick.bCanEverTick = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetClosedLoop(true, true);
}
// Called when the game starts
void UProceduralMeshesBase::BeginPlay()
{
	Super::BeginPlay();

	Controller = GetWorld()->GetFirstPlayerController();
	
}
// Called every frame
void UProceduralMeshesBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UProceduralMeshesBase::AddTriangle(FVector V1, FVector V2, FVector V3)
{
	Spline->AddSplinePoint(V1, ESplineCoordinateSpace::World, true);
	Spline->AddSplinePoint(V2, ESplineCoordinateSpace::World, true);
	Spline->AddSplinePoint(V3, ESplineCoordinateSpace::World, true);
	Spline->SetClosedLoop(true, true);
	SplinePointsIndex += 2;
}

void UProceduralMeshesBase::GenerateTriangle(FVector Center)
{
	AddTriangle(AddPoint(93, -73), AddPoint(-136, -73), AddPoint(0, 115));
	for (int i = 0; i < 3; i++) {
		Spline->SetSplinePointType(i, ESplinePointType::Linear, true);
	}
	UE_LOG(LogTemp, Warning, TEXT("Triangle generated..."));
}

void UProceduralMeshesBase::GenerateFigure()
{
	Spline->ClearSplinePoints(true);
	int32 rnd = FMath::RandRange(0, 5);
	switch (rnd) {
	case 1:
		GenerateTriangle(FVector::ZeroVector);
		break;
	case 2:
		GenerateTriangle(FVector::ZeroVector);
		break;
	case 3:
		GenerateTriangle(FVector::ZeroVector);
		break;
	case 4:
		GenerateTriangle(FVector::ZeroVector);
		break;
	case 5:
		GenerateTriangle(FVector::ZeroVector);
		break;
	default:
		GenerateTriangle(FVector::ZeroVector);
		break;
	}

}

void UProceduralMeshesBase::FigureVisualPresentation()
{
	
}

FVector UProceduralMeshesBase::AddPoint(float Y, float Z)
{
	return FVector(0, Y, Z);
}

