// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

/**
 * 武器基类
 */
UCLASS()
class RAIDRUNNER_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

	// 生成时相对于角色的位置
	UPROPERTY(EditAnyWhere)
	FVector SpawnPositionOffset;

public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 获取当前武器对应的组件
	UFUNCTION()
	virtual USkeletalMeshComponent* GetWeaponComponent();

	// 将当前武器绑定到角色，调用组件同名方法以实现
	UFUNCTION()
	virtual void AttachWeapon(ARunnerCharacter* TargetCharacter);

	// 将当前武器从角色上解绑，调用组件同名方法以实现
	UFUNCTION()
	virtual void UnbindWeapon();
};
