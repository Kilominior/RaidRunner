// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "TP_GunComponent.h"
#include "Gun.generated.h"

/**
 * 枪械类武器
 */
UCLASS()
class RAIDRUNNER_API AGun : public ABaseWeapon
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Component)
	UTP_GunComponent* GunComponent;

public:
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual USkeletalMeshComponent* GetWeaponComponent() override;

	virtual void AttachWeapon(ARunnerCharacter* TargetCharacter) override;
};
