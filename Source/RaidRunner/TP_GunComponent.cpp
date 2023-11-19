// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_GunComponent.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

// Sets default values for this component's properties
UTP_GunComponent::UTP_GunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTP_GunComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTP_GunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTP_GunComponent::AttachWeapon(ARunnerCharacter* TargetCharacter)
{
	OwnerCharacter = TargetCharacter;
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	// Attach the weapon to the First Person Character
	//FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	//AttachToComponent(OwnerCharacter->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// switch bHasRifle so the animation blueprint can switch to another animation set
	//OwnerCharacter->SetHasRifle(true);

	if (APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// 将枪械输入上下文绑定到角色
			Subsystem->AddMappingContext(GunMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// 绑定开枪输入
			EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &UTP_GunComponent::Shoot);
		}
	}
}

void UTP_GunComponent::Shoot()
{
	// 当枪拥有主人时才发射
	if (OwnerCharacter == nullptr || OwnerCharacter->GetController() == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("开枪"));

	// 当发射物类存在时才发射
	if (ProjectileClass)
	{
		// 设置子弹生成位置
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// 获取枪械主人的控制器
		APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());

		// 获取相机朝向
		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		// 发射方向略微向上倾斜
		//SpawnRotation.Pitch += 10.0f;

		// 将枪口偏移从相机空间变换到世界空间
		const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
		

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			//   SpawnParams.Owner = this;
			//   SpawnParams.Instigator = GetInstigator();
			//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// 在枪口位置生成子弹
			ADefaultProjectile* Projectile = World->SpawnActor<ADefaultProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
			if (Projectile)
			{
				// 设置子弹的初始轨迹
				FVector LaunchDirection = SpawnRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}

void UTP_GunComponent::Reload()
{

}