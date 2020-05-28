// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProcLiquidMesh.generated.h"

UCLASS()
class LIQUID2_API AProcLiquidMesh : public AActor
{
	GENERATED_BODY()
	
private:	
	UProceduralMeshComponent* mesh;

	USceneComponent* Root;

	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	TArray<FVector2D> UVs;
	TArray<int32> Triangles;
	TArray<FColor> Colors;

	int32 TrisIndex;
	int32 vertexIndex;
	int32 Resolution = 70;

	float Radius = 140;
	float SpringForce = 100;
	float Damping = 7;
	float GlobalDeltaTime;

	void PostLoad() override;
	void PostActorCreated() override;

	void AddTriangle(int32 V1, int32 V2, int32 V3);
	void AddCircle(FVector Center, int32 Radius, int32 Resolution);
	void GenerateMesh();
	void AddForceToVertex(int32 VertexID, float ForceToApply, FVector PlaceToApplyForce);
	void UpdateVertex(int32 VertexID);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	AProcLiquidMesh();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DeformMesh(float Force, FVector PlaceToApplyForce);

	UPROPERTY(EditAnywhere) 
		UMaterialInterface* Material;

	///Returns Vertex direction relative to center
	TArray<FVector> VerticiesDir;
	///Current vertex location
	TArray<FVector> Verticies;
	///Default vertex location
	TArray<FVector> VerticiesDefaultPos;
	///Velocities of verticies
	TArray<FVector>VerticiesVelocity;

};
