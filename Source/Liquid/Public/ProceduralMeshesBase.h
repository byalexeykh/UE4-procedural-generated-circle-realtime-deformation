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
	void AddTriangle(FVector V1, FVector V2, FVector V3);
	void GenerateTriangle(FVector Center);
	void FigureVisualPresentation();

	APlayerController* Controller;



	FVector AddPoint(float Y, float Z);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Sets default values for this component's properties
	UProceduralMeshesBase();

	int32 SplinePointsIndex = 0;
	
	void GenerateFigure();
	
	USplineComponent* Spline;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
