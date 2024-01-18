// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerController.h"
#include <EnhancedInputSubsystems.h>

ARunnerController::ARunnerController()
{
}

void ARunnerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		// 显示调试消息五秒
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Hello, this is RunnerController!"));
	}

	// 获取增强输入上下文并添加到子系统
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}
