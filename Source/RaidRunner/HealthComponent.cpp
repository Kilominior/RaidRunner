// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

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

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 复制当前生命值
	DOREPLIFETIME(UHealthComponent, CurrentHealth);
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
	
	// 广播受伤事件，将当前的生命值告知组件的主人
	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, DamageType, InstigatedBy, DamageCauser);
}

void UHealthComponent::OnHealthUpdate()
{
	// 获取玩家Pawn
	APawn* HealthOwner = Cast<APawn, AActor>(GetOwner());
	if (!HealthOwner)
	{
		return;
	}

	// 客户端特定的功能
	if (HealthOwner->IsLocallyControlled())
	{
		FString healthMessage = FString::Printf(TEXT("[客户端]: 当前生命值为%f"), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		// 若生命值耗尽，则销毁角色
		if (CurrentHealth <= 0.0f)
		{
			FString deathMessage = FString::Printf(TEXT("[客户端]: 您已死亡"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}

	// 服务器特定的功能
	if (HealthOwner->GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("[服务器]: 玩家%s当前生命值为%f"), *GetOwner()->GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		// 若生命值耗尽，则销毁角色
		if (CurrentHealth <= 0.0f)
		{
			FString deathMessage = FString::Printf(TEXT("[服务器]: 该玩家已死亡，尝试销毁"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);

			HealthOwner->Destroy();
		}
	}

	// 在所有机器上都执行的功能
	/*
		因任何因伤害或死亡而产生的特殊功能都应放在这里。
	*/

}

void UHealthComponent::SetCurrentHealth(float healthValue)
{
	AActor* HealthOwner = GetOwner();
	if (HealthOwner)
	{
		// 只有在服务器上才允许更新玩家的生命值
		if (HealthOwner->GetLocalRole() == ROLE_Authority)
		{
			CurrentHealth = FMath::Clamp(healthValue, 0.0f, MaxHealth);
			OnHealthUpdate();
		}
	}
}

void UHealthComponent::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}
