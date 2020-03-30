// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProcLiquidMesh.generated.h"

class USplineComponent;

UCLASS()
class LIQUID_API AProcLiquidMesh : public AActor
{
	GENERATED_BODY()
	
private:
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	TArray<FVector2D> UVs;
	TArray<int32> Triangles;
	TArray<FColor> Colors;

	int32 TrisIndex;
	int32 vertexIndex;

	UProceduralMeshComponent* mesh;

	USceneComponent* Root;

	void PostLoad() override;
	void PostActorCreated() override;

	void AddTriangle(int32 V1, int32 V2, int32 V3);
	void AddUV(float V1, float V2);
	void AddVertex(int32& Index, int32 V2, int32 V3);
	void AddVertex(int32 V2, int32 V3);
	void AddVertexColor(FColor Color);
	void AddNormal(FVector normal);
	void AddTangent(FProcMeshTangent tangent);
	void AddCircle(FVector Center, int32 Radius, int32 Resolution);

protected:

public:	
	USplineComponent* Spline;

	void GenerateMesh();
	void UpdateMesh();
	void AddPointToSpline(FVector NewPointLoc);

	bool Error = false;
	
	FVector MeshCenter = FVector(0, 0, 0);
	
	///Returns vertex by its ID in array
	FVector GetVertexByID(int32 ID);
	
	///Returns Vertex direction relative to center
	TArray<FVector> VerticiesDir;
	///Current vertex location
	TArray<FVector> Verticies;
	///Default vertex location
	TArray<FVector> VerticiesDefaultPos;
	///Returns Verticies array length
	int32 GetVerticiesArrayLength();

	///Changes vertex location
	void ChangeVertexLoc(int32 VertexID, FVector NewLocation);

	// Sets default values for this actor's properties
	AProcLiquidMesh();
};
