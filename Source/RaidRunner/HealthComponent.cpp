// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// 默认生命值为100
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// 获取本组件的主人
	AActor* HealthOwner = GetOwner();
	if (HealthOwner)
	{
		// 当主人受到伤害时，将会调用组件提供的伤害处理方法
		HealthOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeDamage);
	}

	// 初始化当前生命值
	CurrentHealth = MaxHealth;
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::HandleTakeDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	// 若伤害不成立，则不处理
	if (Damage <= 0.0f)
	{
		return;
	}

	// 在实际生命值中扣除伤害数值
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
	
	// 广播受伤事件，将当前的生命值告知组件的主人
	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, DamageType, InstigatedBy, DamageCauser);
}

