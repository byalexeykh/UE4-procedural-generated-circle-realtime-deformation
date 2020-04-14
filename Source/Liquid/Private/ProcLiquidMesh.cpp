// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcLiquidMesh.h"
#include "..\Public\ProcLiquidMesh.h"
#include "Components/SplineComponent.h"
#include "DrawDebugHelpers.h"


// Sets default values
AProcLiquidMesh::AProcLiquidMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	mesh->SetupAttachment(RootComponent);

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->bDrawDebug = true;
}

void AProcLiquidMesh::PostLoad()
{
	Super::PostLoad();
	GenerateMesh();
}

void AProcLiquidMesh::PostActorCreated()
{
	Super::PostActorCreated();
	GenerateMesh();
}


void AProcLiquidMesh::AddTriangle(int32 V1, int32 V2, int32 V3)
{
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
}

void AProcLiquidMesh::AddUV(float U, float V)
{
	UVs.Add(FVector2D(U, V));
}

void AProcLiquidMesh::AddVertex(int32& Index, int32 Y, int32 Z) // only 2 coords cuz we are building a circle in 2d space
{
	Verticies.Add(FVector(0, Y, Z));
	Index++;
}

void AProcLiquidMesh::AddVertex(int32 Y, int32 Z) // only 2 coords cuz we are building a circle in 2d space
{
	Verticies.Add(FVector(0, Y, Z));
}

void AProcLiquidMesh::AddVertexColor(FColor Color)
{
	Colors.Add(Color);
}

void AProcLiquidMesh::AddNormal(FVector normal)
{
	Normals.Add(normal);
}

void AProcLiquidMesh::AddTangent(FProcMeshTangent tangent)
{
	Tangents.Add(tangent);
}

void AProcLiquidMesh::AddCircle(FVector Center, int32 kekRadius, int32 kekResolution)
{
	float angleBetweenPointsRads = (360 / Resolution) * PI / 180;
	//UE_LOG(LogTemp, Warning, TEXT("Angle between point in rads is = %f"), angleBetweenPointsRads);
	float currentAngleRads = 0;
	FVector2D UVCenter = FVector2D(0, 0);
	vertexIndex = 0; // resetting VertexIndex
	Verticies.Add(Center); // Adding Center first (Verticies[vertexIndex] = Verticies[0] = Center)
	VerticiesDir.Add(Center);
	UVs.Add(UVCenter);

	for (int i = 0; i < kekResolution; i++)
	{
		float Y = kekRadius * cos(currentAngleRads);
		float Z = kekRadius * sin(currentAngleRads);
		AddVertex(Y, Z);
		//UE_LOG(LogTemp, Warning, TEXT("Added vertex of num %f with coords: Y: %f, Z: %f"), i, Y, Z);
		//UE_LOG(LogTemp, Warning, TEXT("using currentAngleRads = %f"), currentAngleRads);
		vertexIndex++;

		float U = 1 * cos(currentAngleRads);
		float V = 1 * sin(currentAngleRads);
		AddUV(U, V);
		VerticiesDir.Add(FVector(0, U, V));

		FVector ThisNormal = FVector::CrossProduct(GetVertexByID(0), GetVertexByID(i)).GetSafeNormal();
		AddNormal(ThisNormal);
		AddTangent(FProcMeshTangent(0, 1, 0));
		AddVertexColor(FColor::Green);

		currentAngleRads += angleBetweenPointsRads;
	}

	for (int i = 1; i < Resolution; i++)
	{
		AddTriangle(0, i + 1, i); //рассчитать vertexIndex для алгоритма добавления треугольников
	}

	AddTriangle(0, 1, Resolution);
}

FVector AProcLiquidMesh::GetVertexByID(int32 ID)
{
	int32 c = vertexIndex;
	if (ID <= c)
		return Verticies[ID];
	else {
		UE_LOG(LogTemp, Error, TEXT("provided VertexID is out of bounds of Verticies array! Returninig Zero-Vector... (Verticies.Num: %i | Provided ID: %i)"), c, ID);
		return FVector(0, 0, 0);
	}
}

void AProcLiquidMesh::GenerateMesh()
{
	Verticies.Reset();
	Normals.Reset();
	Tangents.Reset();
	UVs.Reset();
	Triangles.Reset();
	Colors.Reset();

	AddCircle(MeshCenter, Radius, Resolution);

	VerticiesDefaultPos.Append(Verticies); // Backup'ing verticies coords

	//UE_LOG(LogTemp, Warning, TEXT("vertexIndex (max) = %i"), vertexIndex);

	mesh->CreateMeshSection(0, Verticies, Triangles, Normals, UVs, Colors, Tangents, true); 
	mesh->bUseAsyncCooking = true;
}

void AProcLiquidMesh::UpdateMesh()
{
	mesh->UpdateMeshSection(0, Verticies, Normals, UVs, Colors, Tangents);
}

void AProcLiquidMesh::AddPointToSpline(FVector NewPointLoc)
{
	Spline->AddSplineLocalPoint(NewPointLoc);
}

float AProcLiquidMesh::GetRadius()
{
	return Radius;
}

int32 AProcLiquidMesh::GetResolution()
{
	return Resolution;
}

int32 AProcLiquidMesh::GetVerticiesArrayLength()
{
	return vertexIndex;
}

void AProcLiquidMesh::ChangeVertexLoc(int32 VertexID, FVector NewLocation)
{
	if (VertexID <= vertexIndex)
		Verticies[VertexID] = NewLocation;
	else {
		UE_LOG(LogTemp, Error, TEXT("provided VertexID (%i) is out of bound of Verticies array (%i)"), VertexID, vertexIndex);
		Error = true;
	}
}