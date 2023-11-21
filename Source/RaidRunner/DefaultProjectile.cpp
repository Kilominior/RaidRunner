// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultProjectile.h"

// Sets default values
ADefaultProjectile::ADefaultProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    if (!RootComponent)
    {
        // 创建根组件
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
    }
    if (!CollisionComponent)
    {
        // 创建球形碰撞组件
        CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

        // 设置碰撞半径
        CollisionComponent->InitSphereRadius(5.0f);

        // 设置碰撞预设为子弹
        CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");

        // 禁止角色在碰撞体上行走
        CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
        CollisionComponent->CanCharacterStepUpOn = ECB_No;

        // 将根组件设置为碰撞组件
        RootComponent = CollisionComponent;
    }

    // 创建Mesh组件
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));

    if (!ProjectileMovementComponent)
    {
        // 创建发射物移动组件
        ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
        
        // 发射物控制碰撞组件
        ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);

        // 初始化发射物参数
        ProjectileMovementComponent->InitialSpeed = 2500.0f;
        ProjectileMovementComponent->MaxSpeed = 3000.0f;

        ProjectileMovementComponent->Bounciness = 0.3f;
        ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

        ProjectileMovementComponent->bRotationFollowsVelocity = true;
        ProjectileMovementComponent->bShouldBounce = true;
    }

    // 设置发射物的生命周期
    InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void ADefaultProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADefaultProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADefaultProjectile::FireInDirection(const FVector& ShootDirection)
{
    // 将子弹的速度设为给定方向 * 初始速度
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}