// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RunnerWidget.generated.h"

/**
 * 
 */
UCLASS()
class RAIDRUNNER_API URunnerWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

	// 加载Widget
	UFUNCTION(BlueprintCallable)
	void LoadWidget();

};
