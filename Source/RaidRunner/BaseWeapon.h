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

protected:
	// 武器名称
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	FString WeaponName;

	// 生成时相对于角色的位置
	UPROPERTY(EditAnyWhere)
	FVector SpawnPositionOffset;

	// 弹仓内当前的子弹数量
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Bullet")
	uint8 CurrentAmmoNum;

	// 弹仓总容量
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet")
	uint8 MagazineCapacity;

public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	// 属性复制
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	// 获取当前武器对应的组件
	UFUNCTION()
	virtual USkeletalMeshComponent* GetWeaponComponent();

	// 将当前武器绑定到角色，调用组件同名方法以实现
	UFUNCTION()
	virtual void AttachWeapon(ARunnerCharacter* TargetCharacter);

	// 将当前武器从角色上解绑，调用组件同名方法以实现
	UFUNCTION()
	virtual void UnbindWeapon();

	// 设置子弹数量，仅在服务器调用
	UFUNCTION(Server, Reliable)
	virtual void SetCurrentAmmoNum(const int& Num);

	UFUNCTION(BlueprintPure, Category = "Data")
	virtual FString GetWeaponName() const { return WeaponName; }

	UFUNCTION(BlueprintPure, Category = "Ammo")
	virtual uint8 GetCurrentAmmoNum() const { return CurrentAmmoNum; }

	UFUNCTION(BlueprintPure, Category = "Ammo")
	virtual uint8 GetMagazineCapacity() const { return MagazineCapacity; }
};
