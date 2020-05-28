// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcLiquidMesh.h"
#include "..\Public\ProcLiquidMesh.h"

// LOADING FUNCS ============================================================================
AProcLiquidMesh::AProcLiquidMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	mesh->SetupAttachment(RootComponent);
}
void AProcLiquidMesh::BeginPlay()
{
	Super::BeginPlay();
	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(Material, mesh);
	mesh->SetMaterial(0, DynMaterial);
	for (int i = 0; i <= Resolution; i++) {
		VerticiesVelocity.Add(FVector::ZeroVector);
	}
	UE_LOG(LogTemp, Warning, TEXT("vertexIndex = %i"), vertexIndex);
	UE_LOG(LogTemp, Warning, TEXT("Verticies size = %i"), Verticies.Num());
}
void AProcLiquidMesh::PostLoad() {
	Super::PostLoad();
	GenerateMesh();
}
void AProcLiquidMesh::PostActorCreated() {
	Super::PostActorCreated();
	GenerateMesh();
}
// ==========================================================================================


// SERVICE FUNCS ============================================================================
void AProcLiquidMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GlobalDeltaTime = DeltaTime;
	for (int32 i = 1; i <= vertexIndex; i++) {
		UpdateVertex(i);
	}
	mesh->UpdateMeshSection(0, Verticies, Normals, UVs, Colors, Tangents);
}
// ==========================================================================================


// DEFORMING MESH IN REALTIME FUNCS =========================================================
void AProcLiquidMesh::DeformMesh(float Force, FVector PlaceToApplyForce)
{
	for (int32 i = 1; i <= vertexIndex; i++) {
		AddForceToVertex(i, Force, PlaceToApplyForce);
	}
}
void AProcLiquidMesh::AddForceToVertex(int32 VertexID, float ForceToApply, FVector PlaceToApplyForce)
{
	FVector FromCenterToTouch = PlaceToApplyForce - FVector::ZeroVector;
	float AttinuatedForce;
	FVector DirToMove = PlaceToApplyForce - Verticies[VertexID];
	if(FromCenterToTouch.Size() > Radius)
		AttinuatedForce = DirToMove.Size() > 20 ? ForceToApply / (1 + DirToMove.Size() / 2) : 0;
	else
		AttinuatedForce = DirToMove.Size() > 20 ? ForceToApply / (1 - DirToMove.Size() / 2) : 0;
	float Velocity = AttinuatedForce * GlobalDeltaTime;
	VerticiesVelocity[VertexID] += DirToMove.GetSafeNormal() * Velocity;
}
void AProcLiquidMesh::UpdateVertex(int32 VertexID)
{
	FVector velocity = VerticiesVelocity[VertexID];
	FVector displacement = Verticies[VertexID] - VerticiesDefaultPos[VertexID];
	velocity -= displacement * SpringForce * GlobalDeltaTime;
	velocity *= 1 - Damping * GlobalDeltaTime;
	VerticiesVelocity[VertexID] = velocity;
	Verticies[VertexID] += velocity * GlobalDeltaTime;
}
// ==========================================================================================


// MESH GENERATING FUNCS ====================================================================
void AProcLiquidMesh::AddTriangle(int32 V1, int32 V2, int32 V3)
{
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
}
void AProcLiquidMesh::AddCircle(FVector Center, int32 radius, int32 resolution)
{
	float angleBetweenPointsRads = (360 / Resolution) * PI / 180;
	float currentAngleRads = 0;
	FVector2D UVCenter = FVector2D(0, 0);
	vertexIndex = 0; // resetting VertexIndex
	Verticies.Add(Center); // Adding Center first (Verticies[vertexIndex] = Verticies[0] = Center)
	VerticiesDir.Add(Center);
	UVs.Add(UVCenter);

	for (int i = 0; i < resolution; i++)
	{
		float Y = radius * cos(currentAngleRads);
		float Z = radius * sin(currentAngleRads);
		Verticies.Add(FVector(0, Y, Z));
		vertexIndex++;

		float U = 1 * cos(currentAngleRads);
		float V = 1 * sin(currentAngleRads);
		UVs.Add(FVector2D(U, V));
		VerticiesDir.Add(FVector(0, U, V));

		FVector ThisNormal = FVector::CrossProduct(Verticies[0], Verticies[i]).GetSafeNormal();
		Normals.Add(ThisNormal);
		Tangents.Add(FProcMeshTangent(0, 1, 0));
		Colors.Add(FColor::Green);

		currentAngleRads += angleBetweenPointsRads;
	}

	for (int i = 1; i < Resolution; i++)
	{
		AddTriangle(0, i + 1, i);
	}

	AddTriangle(0, 1, Resolution);
}
void AProcLiquidMesh::GenerateMesh()
{
	Verticies.Reset();
	Normals.Reset();
	Tangents.Reset();
	UVs.Reset();
	Triangles.Reset();
	Colors.Reset();

	AddCircle(FVector(0, 0, 0), Radius, Resolution);

	VerticiesDefaultPos.Append(Verticies); // Backup'ing verticies coords

	mesh->CreateMeshSection(0, Verticies, Triangles, Normals, UVs, Colors, Tangents, true);
	mesh->bUseAsyncCooking = true;
}
// ==========================================================================================