// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h" 
#include "RunnerHUD.generated.h"

/**
 * 
 */
UCLASS()
class RAIDRUNNER_API ARunnerHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	// 十字准星材质
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairTexture;

public:
	// 重写的绘制HUD
	virtual void DrawHUD() override;
};
