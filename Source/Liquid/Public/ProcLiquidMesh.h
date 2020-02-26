// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProcLiquidMesh.generated.h"

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
	FVector MeshCenter = FVector(0, 0, 0);

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
	FVector GetVertexByID(int32 ID);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:	
	int32 GetVertexNum();
	void GenerateMesh();

	TArray<FVector> Verticies;

	// Sets default values for this actor's properties
	AProcLiquidMesh();
};
