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

	// Binding actions and axis

	InputComponent->BindAction("MouseSelect", IE_Pressed, this, &AMyPlayerController::SetInteractState);
	InputComponent->BindAction("MouseSelect", IE_Released, this, &AMyPlayerController::Stop); // Stop actions
	InputComponent->BindAxis("MouseX", this, &AMyPlayerController::MoveX); // When move mouse in X axis
	InputComponent->BindAxis("MouseY", this, &AMyPlayerController::MoveY); // When move mouse in Y axis

	UE_LOG(LogTemp, Warning, TEXT("Incorrect points: %i"), IncorrectPoints);
	UE_LOG(LogTemp, Warning, TEXT("Max incorrect points: %i"), MaxIncorrectPoints);

	GenerateTaskShape();
}

void AMyPlayerController::Tick(float DeltaTime)
{
	
	if (Interactable) 
	{
		DeprojectMousePositionToWorld(MouseWorldPosition, MouseWorldDirection);
		GetWorld()->LineTraceSingleByChannel(HitResult, MouseWorldPosition, MouseWorldPosition + MouseWorldDirection * 1000, ECC_Visibility, CollisionQueryParams);
		DrawSpline(HitResult.ImpactPoint);

		DrawSelectingArea();

		int32 VertexIndex = 0;
		for (FVector v : ProceduralLiquidMesh->Verticies)
		{
			Distance = FVector::Distance(v, HitResult.ImpactPoint); //TODO Переделать систему перемещения вертексов под управление сплайном
			VerticiesToSelectCenterDistMap.Emplace(VertexIndex, Distance);

			if (PosedVerticies.Find(VertexIndex) == nullptr) {
				UE_LOG(LogTemp, Error, TEXT("Cant move vertex: index: %i is out of bounds of PosedVerticies (%i) map"), VertexIndex, PosedVerticies.Num() - 1);
				DrawDebugSphere(
					GetWorld(),
					ProceduralLiquidMesh->Verticies[VertexIndex],
					3,
					3,
					FColor::Red,
					false,
					10.f,
					0
				);
				return;
			}

			if (Distance < SelectRadius && VertexIndex > 0)
			{
				SelectedVerticiesMap.Emplace(VertexIndex, true);
			}
			else 
			{
				SelectedVerticiesMap.Emplace(VertexIndex, false);
				ProceduralLiquidMesh->Error = false;
			}
			
			VertexIndex++;
		}
		ProceduralLiquidMesh->UpdateMesh();
	}
	else 
	{
		for (int32 i = 0; i <= ProceduralLiquidMesh->GetVerticiesArrayLength(); i++)
		{
			if (ProceduralLiquidMesh->Error) {
				UE_LOG(LogTemp, Error, TEXT("Unable to update Mesh because an error occurred"));
			}
			else {
				ProceduralLiquidMesh->ChangeVertexLoc(i, FMath::InterpSinInOut(ProceduralLiquidMesh->Verticies[i], ProceduralLiquidMesh->VerticiesDefaultPos[i], DeltaTime * 20));
				ProceduralLiquidMesh->UpdateMesh();
			}
		}
	}
	
}

void AMyPlayerController::SetInteractState()
{
	Interactable = true;
}

void AMyPlayerController::Stop()
{
	
	
	Interactable = false;
	ProceduralLiquidMesh->Error = false;
	
	for (auto& elem : SelectedVerticiesMap) {
		elem.Value = false;
	}
	
	for (auto& elem : SplinePointsMap) 
	{
		if (FVector::Distance(ProcMeshBase->Spline->FindLocationClosestToWorldLocation(elem.Value, ESplineCoordinateSpace::World), elem.Value) > MaxDeviationFromTaskShape) 
		{
			IncorrectPoints++;
			UE_LOG(LogTemp, Warning, TEXT("IncorrectPoints++: %i"), IncorrectPoints);
			DrawDebugSphere(
				GetWorld(),
				elem.Value,
				6,
				10,
				FColor::Red,
				false,
				10.f,
				0
			);
		}
		else {
			DrawDebugSphere(
				GetWorld(),
				elem.Value,
				6,
				10,
				FColor::Green,
				false,
				10.f,
				0
			);
			
		}

		elem.Value = FVector::ZeroVector;
	}

	if (IncorrectPoints < MaxIncorrectPoints)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCorrectShapeDrawed()"));
		OnCorrectShapeDrawed();
	}
	else 
	{
		MaxIncorrectPoints = 10;
		// выводить на экран корректные и некорректные точки
		UE_LOG(LogTemp, Warning, TEXT("Incorrect points: %i"), IncorrectPoints);
		UE_LOG(LogTemp, Warning, TEXT("Max incorrect points: %i"), MaxIncorrectPoints);
		UE_LOG(LogTemp, Warning, TEXT("OnIncorrectShapeDrawed()"));
		OnIncorrectShapeDrawed();
	}

	for (auto& elem : PosedVerticies) {
		elem.Value = false;
	}

	for (auto& elem : IsSplinePointOccupied) {
		elem.Value = false;
	}

	for (auto& elem : VerticiesToSelectCenterDistMap) {
		elem.Value = 0;
	}
	
	CurrentSplinePoint = 0;
	ProceduralLiquidMesh->Spline->ClearSplinePoints(true);

	UE_LOG(LogTemp, Warning, TEXT("Stopping actions"));
}

void AMyPlayerController::DrawSelectingArea()
{
	DrawDebugSphere(
		GetWorld(),
		HitResult.ImpactPoint,
		SelectRadius,
		10,
		FColor::Green,
		false,
		-1.f,
		0
	);
}

void AMyPlayerController::DrawDebugText(FVector loc, FString text, AActor* actor)
{
	DrawDebugString
	(
		GetWorld(),
		loc,
		text,
		actor,
		FColor::Black,
		-1.f,
		false,
		2
	);
}

void AMyPlayerController::DrawSpline(FVector hitResult)
{
	if (CurrentSplinePoint == 0) {
		SplinePointsMap.Emplace(0, hitResult);
		SplinePoints.Add(hitResult);
		ProceduralLiquidMesh->Spline->AddSplinePoint(FVector(0, hitResult.Y, hitResult.Z), ESplineCoordinateSpace::World, true);
		
		DrawDebugSphere(
			GetWorld(),
			ProceduralLiquidMesh->Spline->GetLocationAtSplinePoint(CurrentSplinePoint, ESplineCoordinateSpace::World),
			5,
			6,
			FColor::White,
			false,
			5.f,
			2
		);
		
		CurrentSplinePoint++;
	}
	else 
	{
		if (FVector::Distance(SplinePointsMap[CurrentSplinePoint], hitResult) > DistanceBetweenPoints && CurrentSplinePoint < ProceduralLiquidMesh->GetVerticiesArrayLength()) {
			CurrentSplinePoint++;
			
			SplinePointsMap.Emplace(CurrentSplinePoint, hitResult);
			ProceduralLiquidMesh->Spline->AddSplinePoint(FVector(0, hitResult.Y, hitResult.Z), ESplineCoordinateSpace::World, true);
	
			DrawDebugSphere(
				GetWorld(),
				ProceduralLiquidMesh->Spline->GetLocationAtSplinePoint(CurrentSplinePoint, ESplineCoordinateSpace::World),
				5,
				5,
				FColor::White,
				false,
				5.f,
				2
			);
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

void AMyPlayerController::CorrectVertexPos()
{
	
}

void AMyPlayerController::GenerateTaskShape()
{
	ProcMeshBase->GenerateFigure();
	OnGenerateTaskShape();
	
	DrawDebugSphere(
		GetWorld(),
		ProcMeshBase->Spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World),
		4,
		3,
		FColor::Red,
		false,
		100.f,
		0
	);

	DrawDebugSphere(
		GetWorld(),
		ProcMeshBase->Spline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::World),
		4,
		3,
		FColor::Red,
		false,
		100.f,
		0
	);

	DrawDebugSphere(
		GetWorld(),
		ProcMeshBase->Spline->GetLocationAtSplinePoint(2, ESplineCoordinateSpace::World),
		4,
		3,
		FColor::Red,
		false,
		100.f,
		0
	);
}

float AMyPlayerController::DecreaseShiftByDistance(float distance)
{
	return distance == 0 ? 1 : 1 / distance;
}

bool AMyPlayerController::IsDirDeviationLimit(int32 Key)
{
	return false;
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
		Points.Add(ProcMeshBase->Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
		DrawDebugSphere(
			GetWorld(),
			ProcMeshBase->Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World),
			7,
			6,
			FColor::Magenta,
			false,
			30.f,
			0
		);
	}
	UE_LOG(LogTemp, Warning, TEXT("Points array last index is: %i"), ProcMeshBase->SplinePointsIndex);
	UE_LOG(LogTemp, Warning, TEXT("Points array NUM index is: %i"), Points.Num());

	return Points;
}

int AMyPlayerController::GetSplinePointsLastIndex()
{
	return ProcMeshBase->SplinePointsIndex;
}
