// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "D:\UEprojects\06_Liquid\Liquid\Source\Liquid\Public\ProcLiquidMesh.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"


void AMyPlayerController::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling BeginPlay via C++..."));
	for (TObjectIterator<AProcLiquidMesh> it; it; ++it)
	{
		ProceduralLiquidMesh = *it;
		UE_LOG(LogTemp, Warning, TEXT("Selected object name is: %s"), *ProceduralLiquidMesh->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Getting verticies... Verticies num: %i"), ProceduralLiquidMesh->GetVertexNum());
	}

	InputComponent->BindAction("MouseSelect", IE_Pressed, this, &AMyPlayerController::SelectNearVerticies); // Vertex selection next to cursor
	InputComponent->BindAction("MouseSelect", IE_Released, this, &AMyPlayerController::Stop); // Stop actions
	InputComponent->BindAxis("Mouse", this, &AMyPlayerController::DrawSelectingArea); // Stop actions
}

void AMyPlayerController::Tick(float DeltaTime)
{
	if (Interactable) 
	{

	}
}

void AMyPlayerController::SelectNearVerticies()
{
	Interactable = true;
	DeprojectMousePositionToWorld(MouseWorldPosition, MouseWorldDirection);
	MouseWorldPosition *= FVector(0, 20, 20);
	UE_LOG(LogTemp, Warning, TEXT("Mouse world location is: X: %f, Y: %f, Z: %f"), MouseWorldPosition.X, MouseWorldPosition.Y, MouseWorldPosition.Z);
	int32 VertexIndex = 0;
	int32 SelectedVertexIndex = 0;
	for (FVector v : ProceduralLiquidMesh->Verticies)
	{
		VertexIndex++;
		if (FVector::Distance(v, FVector(0, MouseWorldPosition.Y, MouseWorldPosition.Z)) < SelectRadius)
		{
			SelectedVerticies.Add(VertexIndex);
			SelectedVertexIndex++;

			DrawDebugSphere(
				GetWorld(),
				ProceduralLiquidMesh->Verticies[VertexIndex],
				20,
				6,
				FColor::Red,
				false,
				2.f,
				0
			);

		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Selecting near verticies..."));
	UE_LOG(LogTemp, Warning, TEXT("Selected %i verticies"), SelectedVertexIndex);

	
}

void AMyPlayerController::Stop()
{
	Interactable = true;
	//UE_LOG(LogTemp, Warning, TEXT("Stopping actions"));
	SelectedVerticies.Reset();
}

void AMyPlayerController::DrawSelectingArea(float i)
{
	DrawDebugSphere(
		GetWorld(),
		FVector(0, MouseWorldPosition.Y, MouseWorldPosition.Z) ,
		SelectRadius,
		6,
		FColor::Red,
		false,
		2.f,
		0
	);
}
