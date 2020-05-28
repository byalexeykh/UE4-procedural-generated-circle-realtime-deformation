// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "..\Public\MyPlayerController.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "ProcLiquidMesh.h"

void AMyPlayerController::BeginPlay() {
	for (TObjectIterator<AProcLiquidMesh> it; it; ++it)
	{
		ProceduralLiquidMesh = *it;
		UE_LOG(LogTemp, Warning, TEXT("Selected object name is: %s"), *ProceduralLiquidMesh->GetName());
	}
}
void AMyPlayerController::Tick(float DeltaTime)
{
}

void AMyPlayerController::HandleInput()
{
	FVector HitPoint = TouchPos;
	HitPoint += TouchPos.GetSafeNormal() * ForceOffset;
	ProceduralLiquidMesh->DeformMesh(Force, HitPoint);
}


FVector AMyPlayerController::PassCoordsToCode(float Y, float Z)
{
	TouchPos = FVector(0, Y, Z);
	HandleInput();
	return TouchPos;
}
