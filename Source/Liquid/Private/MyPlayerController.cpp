// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "D:\UEprojects\06_Liquid\Liquid\Source\Liquid\Public\ProcLiquidMesh.h"
#include "D:\UEprojects\06_Liquid\Liquid\Source\Liquid\Public\ProceduralMeshesBase.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/SplineComponent.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/World.h"

void AMyPlayerController::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling BeginPlay via C++..."));
	for (TObjectIterator<AProcLiquidMesh> it; it; ++it)
	{
		ProceduralLiquidMesh = *it;
		UE_LOG(LogTemp, Warning, TEXT("Selected object name is: %s"), *ProceduralLiquidMesh->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Getting verticies... Verticies num: %i"), ProceduralLiquidMesh->GetVerticiesArrayLength());
	}

	for (TObjectIterator<AActor> it; it; ++it)
	{
		it->GetName(ActorName);
		if (ActorName == TEXT("Actor_1")) {
			DebugActor = *it;
			UE_LOG(LogTemp, Warning, TEXT("Selected debug object name is: %s"), *DebugActor->GetName());
		}

		if (ActorName == TEXT("TaskShape_BP_2")) 
		{
			ProcMeshBase = it->FindComponentByClass<UProceduralMeshesBase>();
			UE_LOG(LogTemp, Warning, TEXT("Selected TaskShape name is: %s"), *ActorName);
		}
	}
	
	// filling maps with default values

	ProceduralLiquidMesh->Spline->RemoveSplinePoint(1, true);
	
	for (int i = 0; i < ProceduralLiquidMesh->GetVerticiesArrayLength(); i++) {
		SplinePointsMap.Add(i, FVector::ZeroVector);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("SplinePointsMap length is: %i"), SplinePointsMap.Num());

	for (int i = 0; i <= ProceduralLiquidMesh->GetVerticiesArrayLength(); i++) {
		SelectedVerticiesMap.Add(i, false);
		VerticiesToSelectCenterDistMap.Add(i, 0);
		IsSplinePointOccupied.Add(i, false);
		PosedVerticies.Add(i, false);
	}

	DistanceBetweenVerticies = FVector::Distance(ProceduralLiquidMesh->Verticies[1], ProceduralLiquidMesh->Verticies[2]);

	UE_LOG(LogTemp, Warning, TEXT("PosedVerticies length is: %i"), PosedVerticies.Num() - 1);
	UE_LOG(LogTemp, Warning, TEXT("Verticies length is: %i"), ProceduralLiquidMesh->GetVerticiesArrayLength());
	UE_LOG(LogTemp, Warning, TEXT("Distance between verticies is: %f"), DistanceBetweenVerticies);

	UE_LOG(LogTemp, Warning, TEXT("Incorrect points: %i"), IncorrectPoints2);
	UE_LOG(LogTemp, Warning, TEXT("Max incorrect points: %i"), MaxIncorrectPoints2);

	Interactable = false;
}

void AMyPlayerController::SetIntrractState() 
{
	Interactable = true;
	UE_LOG(LogTemp, Warning, TEXT("SETTING INTERACTABLE STATE"));
}

void AMyPlayerController::StartPlay() 
{
	TimerCurrent = TimerInit;
	TimeLast = TimerCurrent;
	TimerDecrease = 1;
	isGameStarted = true;
	GenerateTaskShape();
	OnStartPlay(); 
}

void AMyPlayerController::Tick(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("Is Interactable: %s"), Interactable ? TEXT("TRUE") : TEXT("FALSE"));
	if (isGameStarted) {
		TimeLast -= DeltaTime;
		//UE_LOG(LogTemp, Warning, TEXT("Time Last: %f"), TimeLast);
		if (TimeLast <= 0) {
			Stop();
		}

		if (Interactable) 
		{
			MoveVerticies(TouchPos, DeltaTime);
			DrawSelectingArea();
		}
		else 
		{
			ReturnVerticies(DeltaTime);
		}
	}
}

void AMyPlayerController::Stop()
{
	if (isGameStarted) 
	{
		Interactable = false;
		ProceduralLiquidMesh->Error = false;
		
		//for (auto& elem : SplinePointsMap)
		//{
		//	float DistanceToClosestLocOnTaskShape = FVector::Distance(ProcMeshBase->Spline2->FindLocationClosestToWorldLocation(elem.Value, ESplineCoordinateSpace::World), elem.Value);
		//	//DrawDebugLine
		//	//(
		//	//	GetWorld(),
		//	//	ProcMeshBase->Spline2->FindLocationClosestToWorldLocation(elem.Value, ESplineCoordinateSpace::World),
		//	//	elem.Value,
		//	//	FColor::Yellow,
		//	//	true,
		//	//	10.f,
		//	//	0,
		//	//	2
		//	//);
		//	UE_LOG(LogTemp, Warning, TEXT("DistanceToClosestLocOnTaskShape: %f"), DistanceToClosestLocOnTaskShape);
		//	float RequiredSplineLength = ProcMeshBase->Spline2->GetSplineLength();
		//	float DrawedSplineLength = ProceduralLiquidMesh->Spline->GetSplineLength();
		//
		//	if (DistanceToClosestLocOnTaskShape < MaxDeviationFromTaskShape && DrawedSplineLength >= RequiredSplineLength * 0.7)
		//	{
		//		//DrawDebugSphere(
		//		//	GetWorld(),
		//		//	elem.Value,
		//		//	6,
		//		//	10,
		//		//	FColor::Green,
		//		//	false,
		//		//	10.f,
		//		//	0
		//		//);
		//	}
		//	else if (elem.Value == FVector::ZeroVector) {
		//		//DrawDebugSphere(
		//		//	GetWorld(),
		//		//	elem.Value,
		//		//	6,
		//		//	10,
		//		//	FColor::Yellow,
		//		//	false,
		//		//	10.f,
		//		//	0
		//		//);
		//	}
		//	else {
		//		IncorrectPoints2++;
		//		//DrawDebugSphere(
		//		//	GetWorld(),
		//		//	elem.Value,
		//		//	6,
		//		//	10,
		//		//	FColor::Red,
		//		//	false,
		//		//	10.f,
		//		//	0
		//		//);
		//		
		//	}
		//
		//	elem.Value = FVector::ZeroVector;
		//}
		//
		//for (auto& elem : VerticiesToSelectCenterDistMap) {
		//	elem.Value = 0;
		//}
		
		for (auto elem : ProceduralLiquidMesh->Verticies) // Finding incorrect points
		{
			FVector ClosestPoint = ProcMeshBase->Spline2->FindLocationClosestToWorldLocation(elem, ESplineCoordinateSpace::World);
			float dist = FVector::Distance(elem, ClosestPoint);
			if (dist > DistanceBetweenPoints) {
				IncorrectPoints2++;
			}
		}

		// Checking is correct shape drawed

		if (IncorrectPoints2 < FMath::RoundToInt(ProceduralLiquidMesh->GetResolution() * 0.5))
		{
			UE_LOG(LogTemp, Warning, TEXT("OnCorrectShapeDrawed()"));
			UE_LOG(LogTemp, Warning, TEXT("Incorrect points: %i"), IncorrectPoints2);
			UE_LOG(LogTemp, Warning, TEXT("Max incorrect points: %i"), MaxIncorrectPoints2);
			OnCorrectShapeDrawed();
			CorrectShapeDrawedCombo++;
			CorrectShapeDrawedCombo == IncorrectShapesDrawedMax ? IncorrectShapesDrawed-- : NULL;
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Incorrect points: %i"), IncorrectPoints2);
			UE_LOG(LogTemp, Warning, TEXT("Max incorrect points: %i"), MaxIncorrectPoints2);
			UE_LOG(LogTemp, Warning, TEXT("OnIncorrectShapeDrawed()"));
			OnIncorrectShapeDrawed();
			IncorrectShapesDrawed++;
			CorrectShapeDrawedCombo = 0;
		}

		if (IncorrectShapesDrawed == IncorrectShapesDrawedMax) {
			OnLoose();
			isGameStarted = false;
			IncorrectShapesDrawed = 0;
		}

		DecreaseTimerValue();

		// Reseting values

		OnGenerateSecondSpline(); // TODO Сделать анимацию пропадания (либо изменение толщины линии, либо изменение размера всей фигуры)
		IncorrectPoints2 = 0;
		CurrentSplinePoint = 0;
		PointsPlaced = 0;
		ProceduralLiquidMesh->Spline->ClearSplinePoints(true);
		TimeLast = TimerCurrent;

		UE_LOG(LogTemp, Warning, TEXT("Stopping actions"));
	}
}

void AMyPlayerController::DrawSelectingArea()
{
	//DrawDebugSphere(
	//	GetWorld(),
	//	HitResult.ImpactPoint,
	//	SelectRadius,
	//	10,
	//	FColor::Green,
	//	false,
	//	-1.f,
	//	0
	//);
}

void AMyPlayerController::DrawDebugText(FVector loc, FString text, AActor* actor)
{
	//DrawDebugString
	//(
	//	GetWorld(),
	//	loc,
	//	text,
	//	actor,
	//	FColor::Black,
	//	-1.f,
	//	false,
	//	2
	//);
}

void AMyPlayerController::DrawSpline(FVector hitResult)
{
	if (CurrentSplinePoint == 0) {
		SplinePointsMap.Emplace(0, hitResult);
		ProceduralLiquidMesh->Spline->AddSplinePoint(FVector(0, hitResult.Y, hitResult.Z), ESplineCoordinateSpace::World, true);
		
		//DrawDebugSphere(
		//	GetWorld(),
		//	ProceduralLiquidMesh->Spline->GetLocationAtSplinePoint(CurrentSplinePoint, ESplineCoordinateSpace::World),
		//	5,
		//	6,
		//	FColor::White,
		//	false,
		//	5.f,
		//	2
		//);
		PointsPlaced++;
		CurrentSplinePoint++;
	}
	else 
	{
		if (FVector::Distance(SplinePointsMap[CurrentSplinePoint], hitResult) > DistanceBetweenPoints && CurrentSplinePoint < ProceduralLiquidMesh->GetVerticiesArrayLength()) {
			CurrentSplinePoint++;
			PointsPlaced++;
			SplinePointsMap.Emplace(CurrentSplinePoint, hitResult);
			ProceduralLiquidMesh->Spline->AddSplinePoint(FVector(0, hitResult.Y, hitResult.Z), ESplineCoordinateSpace::World, true);
	
			//DrawDebugSphere(
			//	GetWorld(),
			//	ProceduralLiquidMesh->Spline->GetLocationAtSplinePoint(CurrentSplinePoint, ESplineCoordinateSpace::World),
			//	5,
			//	5,
			//	FColor::White,
			//	false,
			//	5.f,
			//	2
			//);
		}
	}
}

void AMyPlayerController::MoveX(float AxisValue)
{
	//if (Interactable) {
	//	for (auto elem : SelectedVerticiesMap) {
	//		if (elem.Value) {
	//			ProceduralLiquidMesh->ChangeVertexLoc(
	//				elem.Key,
	//				FVector(
	//					ProceduralLiquidMesh->GetVertexByID(elem.Key).X,
	//					ProceduralLiquidMesh->GetVertexByID(elem.Key).Y - (AxisValue /* * DecreaseShiftByDistance(elem.Key)*/),
	//					ProceduralLiquidMesh->GetVertexByID(elem.Key).Z
	//				)
	//			);
	//			//UE_LOG(LogTemp, Warning, TEXT("x"));
	//		}
	//	}
	//	CorrectVertexPos();
	//	ProceduralLiquidMesh->UpdateMesh();
	//}
}

void AMyPlayerController::MoveY(float AxisValue)
{
	//if (Interactable) {
	//	for (auto elem : SelectedVerticiesMap) {
	//		if (elem.Value) {
	//			ProceduralLiquidMesh->ChangeVertexLoc(
	//				elem.Key,
	//				FVector(
	//					ProceduralLiquidMesh->GetVertexByID(elem.Key).X,
	//					ProceduralLiquidMesh->GetVertexByID(elem.Key).Y,
	//					ProceduralLiquidMesh->GetVertexByID(elem.Key).Z + (AxisValue/* * DecreaseShiftByDistance(elem.Key)*/)
	//				)
	//			);
	//			//UE_LOG(LogTemp, Warning, TEXT("y"));
	//		}
	//	}
	//	CorrectVertexPos();
	//	ProceduralLiquidMesh->UpdateMesh();
	//}
}

void AMyPlayerController::ReturnVerticies(float MyDeltaTime)
{
	for (int32 i = 1; i <= ProceduralLiquidMesh->GetVerticiesArrayLength(); i++)
	{
		ProceduralLiquidMesh->Verticies[i] = FMath::InterpExpoInOut(ProceduralLiquidMesh->Verticies[i], ProceduralLiquidMesh->VerticiesDefaultPos[i], MyDeltaTime * 100);
	}
	ProceduralLiquidMesh->UpdateMesh();
}

void AMyPlayerController::MoveVerticies(FVector TouchLoc, float MyDeltaTime)
{
	for (int32 i = 1; i <= ProceduralLiquidMesh->GetVerticiesArrayLength(); i++)
	{
		float DistBetween = FVector::Distance(TouchLoc, ProceduralLiquidMesh->Verticies[i]);
		if (DistBetween < 20)
		{
			FVector Destination = FVector(0, TouchLoc.Y, TouchLoc.Z) * 4;
			if(!IsDirDeviationLimit(i))
				ProceduralLiquidMesh->Verticies[i] = FMath::InterpExpoInOut(ProceduralLiquidMesh->Verticies[i], Destination, MyDeltaTime * 100);
		}
	}
	ProceduralLiquidMesh->UpdateMesh();
}

void AMyPlayerController::DecreaseTimerValue()
{
	if (TimerDecrease > 0.1) {
		TimerDecrease -= 0.03f;
		TimerCurrent = TimerInit * TimerDecrease;
	}
}

void AMyPlayerController::GenerateTaskShape()
{
	ProcMeshBase->GenerateFigure(isHard);
	OnGenerateTaskShape();
	//for (int i = 0; i <= ProcMeshBase->SplinePointsIndex; i++) {
	//	DrawDebugSphere(
	//		GetWorld(),
	//		ProcMeshBase->Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World),
	//		4,
	//		3,
	//		FColor::Red,
	//		false,
	//		100.f,
	//		0
	//	);
	//}
}

float AMyPlayerController::DecreaseShiftByDistance(float distance)
{
	return distance == 0 ? 1 : 1 / distance;
}

bool AMyPlayerController::IsDirDeviationLimit(int32 Key)
{
	FVector Difference = ProceduralLiquidMesh->VerticiesDir[Key] - ProceduralLiquidMesh->Verticies[Key].GetSafeNormal();
	return Difference.X > 0.7 || Difference.Y > 0.7;
}

FVector AMyPlayerController::ConvertTo2D(FVector vectorToConvert)
{
	return FVector(0, vectorToConvert.Y, vectorToConvert.Z);
}

TArray<FVector> AMyPlayerController::GetTaskShapePoints()
{
	TArray<FVector>Points;
	for (int i = 0; i <= ProcMeshBase->SplinePointsIndex; i++)
	{
		Points.Add(ProcMeshBase->Spline2->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
		//DrawDebugSphere(
		//	GetWorld(),
		//	ProcMeshBase->Spline2->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World),
		//	7,
		//	6,
		//	FColor::Magenta,
		//	false,
		//	30.f,
		//	0
		//);
	}
	UE_LOG(LogTemp, Warning, TEXT("Points array last index is: %i"), ProcMeshBase->SplinePointsIndex);
	UE_LOG(LogTemp, Warning, TEXT("Points array NUM index is: %i"), Points.Num());

	return Points;
}

TArray<FVector> AMyPlayerController::GetSecondSplinePoints()
{
	TArray<FVector>Points;
	for (int i = 0; i <= ProcMeshBase->SplinePointsIndex; i++)
	{
		Points.Add(ProcMeshBase->SecondSpline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
		//DrawDebugSphere(
		//	GetWorld(),
		//	ProcMeshBase->Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World),
		//	7,
		//	6,
		//	FColor::Magenta,
		//	false,
		//	30.f,
		//	0
		//);
	}
	UE_LOG(LogTemp, Warning, TEXT("SecondSpline's Points array NUM index is: %i"), Points.Num());

	return Points;
}

int AMyPlayerController::GetSplinePointsLastIndex()
{
	return ProcMeshBase->SplinePointsIndex;
}

void AMyPlayerController::DeleteSecondSpline()
{
	ProcMeshBase->SecondSpline->ClearSplinePoints(true);
}

bool AMyPlayerController::isGameStartedFunc() {
	return isGameStarted;
}

FVector AMyPlayerController::PassCoordsToCode(float Y, float Z) 
{
	TouchPos = FVector(0, Y, Z);
	return TouchPos;
}

float AMyPlayerController::GetTimeInPercent() 
{
	//UE_LOG(LogTemp, Warning, TEXT("Timer percent: %f"), TimeLast / TimerCurrent * 100);
	return TimeLast / TimerCurrent;
}