// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		// 创建根组件
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponSceneComponent"));
	}

	// 初始化武器属性
	WeaponName = TEXT("Weapon");
	CurrentAmmoNum = 0;
	MagazineCapacity = 0;

	// 要求复制
	bReplicates = true;
}

void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseWeapon, CurrentAmmoNum);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USkeletalMeshComponent* ABaseWeapon::GetWeaponComponent()
{
	return nullptr;
}

void ABaseWeapon::AttachWeapon(ARunnerCharacter* TargetCharacter)
{
}

void ABaseWeapon::UnbindWeapon()
{
}

void ABaseWeapon::SetCurrentAmmoNum_Implementation(const int& Num)
{
	CurrentAmmoNum = Num;
}
