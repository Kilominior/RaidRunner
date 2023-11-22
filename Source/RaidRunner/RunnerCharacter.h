// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "BaseWeapon.h"
#include "RunnerCharacter.generated.h"

UCLASS()
class RAIDRUNNER_API ARunnerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** 第一人称手臂Mesh */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** 相机组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* RunnerCameraComponent;

	/** 默认输入上下文 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** 视角旋转输入 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** 角色移动输入 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** 跳跃输入 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	// 当前使用中的武器槽位
	UPROPERTY(VisibleAnywhere)
	uint8 WeaponSlotNow;

	// 将要切换到的武器槽位，未在切换时为0
	UPROPERTY(VisibleAnywhere)
	uint8 SwitchingWeaponTo;

	// 当前使用中的武器实例
	UPROPERTY(VisibleAnywhere)
	ABaseWeapon* WeaponNow;

	/** 切换到1号武器 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SlotAction1;

	/** 切换到2号武器 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SlotAction2;

	/** 切换到投掷物 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ThrowAction;

public:
	// Sets default values for this character's properties
	ARunnerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	// 视角旋转
	UFUNCTION()
	void Look(const FInputActionValue& Value);

	// 角色移动
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	// 尝试进行武器槽位切换，按下按键触发
	UFUNCTION()
	void TrySlotChange(const int SlotId);

	// 结束武器槽位切换的按键操作，进行能否切换的判定，若能则执行切换
	UFUNCTION()
	void EndSlotChange(const int SlotId);

	// 切换到指定槽位的武器
	UFUNCTION()
	void SlotChangeTo(const int SlotId);

	// 返回第一人称Mesh
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
};
