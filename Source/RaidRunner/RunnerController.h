// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RunnerController.generated.h"

/**
 * 
 */
UCLASS()
class RAIDRUNNER_API ARunnerController : public APlayerController
{
	GENERATED_BODY()
	


	/** 默认输入上下文 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

public:
	ARunnerController();

protected:
	virtual void BeginPlay() override;
};
