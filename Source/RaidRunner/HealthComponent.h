// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

// 声明受到伤害委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UHealthComponent*, HealthComp, float, Health, float, DamageAmount, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RAIDRUNNER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// 当前生命值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CurrentHealth;

	// 最大生命值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth;

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 生命值改变时触发事件
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnHealthChangedSignature OnHealthChanged;

protected:
	// 处理受到的伤害
    UFUNCTION(BlueprintCallable)
    void HandleTakeDamage(AActor* DamageActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
