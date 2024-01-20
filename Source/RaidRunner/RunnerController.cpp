// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerController.h"
#include <EnhancedInputSubsystems.h>
#include "Blueprint/UserWidget.h"
#include "RunnerCharacter.h"
#include "BaseWeapon.h"

ARunnerController::ARunnerController()
{
	/*
	// 加载Widget类
	static ConstructorHelpers::FClassFinder<UUserWidget> DefaultWidget(TEXT("'/Game/UI/Gameplay/WBP_DefaultGameplay.WBP_DefaultGameplay_C'"));
	if (DefaultWidget.Succeeded())
	{
		WidgetClass = DefaultWidget.Class;
	}*/
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

	// 加载UI类
	FSoftClassPath softclass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Gameplay/WBP_DefaultGameplay.WBP_DefaultGameplay_C'"));
	WidgetClass = softclass.TryLoadClass<UUserWidget>();

	// 创建游戏UI
	if (IsValid(WidgetClass))
	{
		GameplayWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), WidgetClass));
		if (GameplayWidget != nullptr)
		{
			GameplayWidget->AddToViewport();
		}
	}
}

bool ARunnerController::CheckIsPlayerExist()
{
	CurrentCharacter = Cast<ARunnerCharacter>(GetCharacter());
	if (CurrentCharacter != nullptr)
	{
		return true;
	}
	return false;
}

bool ARunnerController::CheckIsWeaponExist()
{
	if (CheckIsPlayerExist())
	{
		if (CurrentCharacter->GetCurrentWeaponSlot() != 0)
		{
			CurrentWeapon = CurrentCharacter->GetCurrentWeapon();
			if (!CurrentWeapon->IsPendingKill())
			{
				return true;
			}
		}
	}
	return false;
}

uint8 ARunnerController::GetPlayerCurrentHealth()
{
	if (CheckIsPlayerExist())
	{
		return FMath::CeilToInt(CurrentCharacter->GetHealthComponent()->GetCurrentHealth());
	}
	return 0;
}

uint8 ARunnerController::GetPlayerMaxHealth()
{
	if (CheckIsPlayerExist())
	{
		return FMath::CeilToInt(CurrentCharacter->GetHealthComponent()->GetMaxHealth());
	}
	return 1;
}

uint8 ARunnerController::GetPlayerWeaponSlot()
{
	if (CheckIsPlayerExist())
	{
		return CurrentCharacter->GetCurrentWeaponSlot();
	}
	return 0;
}

FString ARunnerController::GetWeaponName()
{
	if (CheckIsWeaponExist())
	{
		return CurrentWeapon->GetWeaponName();
	}
	return TEXT("None");
}

uint8 ARunnerController::GetWeaponCurrentAmmoNum()
{
	if (CheckIsWeaponExist())
	{
		return CurrentWeapon->GetCurrentAmmoNum();
	}
	return 0;
}

uint8 ARunnerController::GetWeaponMagazineCapacity()
{
	if (CheckIsWeaponExist())
	{
		return CurrentWeapon->GetMagazineCapacity();
	}
	return 0;
}
