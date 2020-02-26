// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D:\UEprojects\06_Liquid\Liquid\Source\Liquid\Public\ProcLiquidMesh.h"
#include "MyPlayer.generated.h"



UCLASS()
class LIQUID_API AMyPlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPlayer();

	UPROPERTY(EditAnywhere)
		AProcLiquidMesh* ProcLiquidMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
