// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!GunComponent)
	{
		// 初始化枪械组件
		GunComponent = CreateDefaultSubobject<UTP_GunComponent>(TEXT("GunComponent"));

		RootComponent = GunComponent;
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USkeletalMeshComponent* AGun::GetWeaponComponent()
{
	return GunComponent;
}

void AGun::AttachWeapon(ARunnerCharacter* TargetCharacter)
{
	GunComponent->AttachWeapon(TargetCharacter);
}

void AGun::UnbindWeapon()
{
	GunComponent->UnbindWeapon();
}
