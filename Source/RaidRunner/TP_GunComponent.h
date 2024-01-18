// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RunnerCharacter.h"
#include "DefaultProjectile.h"
#include "TP_GunComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RAIDRUNNER_API UTP_GunComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

protected:
	/** 枪支对应的发射物类 */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
	TSubclassOf<class ADefaultProjectile> ProjectileClass;

	/** 连续射击的速度 */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float FireRate;

	/** 若为true，则正处于发射子弹的延迟 */
	bool bIsFiringWeapon;

	/** 定时器句柄，用于提供生成间隔时间内的射速延迟 */
	FTimerHandle FiringTimer;

	/** 枪口相对于相机位置的偏移 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	FVector MuzzleOffset;

	// 弹仓内当前的子弹数量
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Bullet")
	uint8 CurrentAmmoNum;
	
	// 弹仓总容量
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet")
	uint8 MagazineCapacity;

	// 子弹伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	float BulletDamage;

	/** 枪械输入上下文 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* GunMappingContext;

	/** 开枪输入 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShootAction;

	/** 装弹输入 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

private:
	/* 被一个角色持有 */
	UPROPERTY(ReplicatedUsing = OnRep_HasOwner)
	bool bHasOwner;

	/** 持有本枪械的角色 */
	UPROPERTY(Replicated)
	ARunnerCharacter* OwnerCharacter;

public:	
	// Sets default values for this component's properties
	UTP_GunComponent();

	// 属性复制
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 将武器绑定到目标角色
	UFUNCTION(BlueprintCallable, Category = "Binding")
	void AttachWeapon(ARunnerCharacter* TargetCharacter);

	// 将武器与角色解绑
	UFUNCTION(BlueprintCallable, Category = "Binding")
	void UnbindWeapon();

protected:
	// 绑定武器输入上下文，只在客户端执行
	void BindInput();

	// 解绑武器输入上下文，只在客户端执行
	void UnbindInput();

	/** 启动武器射击 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartFire();

	/** 结束武器射击，同时允许武器再次启动射击 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();

	/** 尝试发射子弹，仅在服务器调用 */
	UFUNCTION(Server, Reliable)
	void HandleFire();

	// 装弹
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void Reload();

	// 设置子弹数量，仅在服务器调用
	UFUNCTION(Server, Reliable)
	void SetCurrentAmmoNum(const int& Num);

	// RepNotify 函数，向客户端同步武器主人变化
	UFUNCTION()
	void OnRep_HasOwner();
};
