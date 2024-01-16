// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultProjectile.generated.h"

UCLASS()
class RAIDRUNNER_API ADefaultProjectile : public AActor
{
	GENERATED_BODY()

protected:
	/** Mesh组件 */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class UStaticMeshComponent* ProjectileMesh;
	
	/** 球碰撞体组件 */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComponent;
	
	/** 发射物移动组件 */
	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	/** 子弹命中的粒子特效 */
	UPROPERTY(EditAnywhere, Category = "Effects")
	class UParticleSystem* ExplosionEffect;

	// 对玩家造成的伤害类型的容器
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;

	// 子弹的的伤害值，由枪支在生成子弹时赋值
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float Damage;

public:	
	// Sets default values for this actor's properties
	ADefaultProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override; 

	// 销毁时播放粒子效果并同步
	virtual void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 设置子弹伤害
	UFUNCTION()
	void SetDamage(const float DamageValue) { Damage = DamageValue; }

	// 在指定方向发射子弹
	UFUNCTION()
	void FireInDirection(const FVector& ShootDirection);

	// 发生碰撞时执行
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

};
