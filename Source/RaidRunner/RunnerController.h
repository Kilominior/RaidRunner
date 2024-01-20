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
	

protected:
	/** 默认输入上下文 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	// Widget类的引用
	UPROPERTY(EditAnywhere, Category = "Class Types")
	TSubclassOf<UUserWidget> WidgetClass;

	// 运行时的UI界面
	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	class UUserWidget* GameplayWidget;

	// 当前的玩家角色
	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	class ARunnerCharacter* CurrentCharacter;

	// 当前的玩家所持武器
	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	class ABaseWeapon* CurrentWeapon;

public:
	ARunnerController();

protected:
	virtual void BeginPlay() override;

protected:
	// TODO: 不在每次检查都重设当前值，改用委托，优化性能
	// 检查是否有玩家Pawn存在
	bool CheckIsPlayerExist();

	// 检查是否有可用的武器存在
	bool CheckIsWeaponExist();

public:
	/** 如下方法提供给Widget用于获取数值 */
	UFUNCTION(BlueprintPure, Category = "Health")
	uint8 GetPlayerCurrentHealth();

	UFUNCTION(BlueprintPure, Category = "Health")
	uint8 GetPlayerMaxHealth();

	UFUNCTION(BlueprintPure, Category = "Weapon")
	uint8 GetPlayerWeaponSlot();

	UFUNCTION(BlueprintPure, Category = "Weapon")
	FString GetWeaponName();

	UFUNCTION(BlueprintPure, Category = "Weapon")
	uint8 GetWeaponCurrentAmmoNum();

	UFUNCTION(BlueprintPure, Category = "Weapon")
	uint8 GetWeaponMagazineCapacity();
};
