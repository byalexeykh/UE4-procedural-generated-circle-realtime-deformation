// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class LIQUID_API ProceduralMesh : public UObject
{
public:
	ProceduralMesh();
	~ProceduralMesh();

	void GenerateCircle(FVector Center, int32 Radius, int32 Resolution);
	void GenerateTriangle();
};
