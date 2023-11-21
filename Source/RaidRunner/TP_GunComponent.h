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

public:
	/** 枪支对应的发射物类 */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ADefaultProjectile> ProjectileClass;

	/** 枪口相对于相机位置的偏移 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

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
	/** 持有本枪械的角色 */
	ARunnerCharacter* OwnerCharacter;

public:	
	// Sets default values for this component's properties
	UTP_GunComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// 将武器及其输入上下文绑定到目标角色
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AttachWeapon(ARunnerCharacter* TargetCharacter);

	// 开枪
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Shoot();

	// 装弹
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();
};
