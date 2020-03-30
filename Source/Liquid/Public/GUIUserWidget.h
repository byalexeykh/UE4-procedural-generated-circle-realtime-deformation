// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GUIUserWidget.generated.h"

class MyPlayerController;
UCLASS()
class LIQUID_API UGUIUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

private:
	void GetController();

	MyPlayerController *MyController;
};