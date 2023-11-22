// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_GunComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

// Sets default values for this component's properties
UTP_GunComponent::UTP_GunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	UE_LOG(LogTemp, Log, TEXT("枪械组件：正在初始化"));

	// 设置子弹生成位置
	MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

	bHasOwner = false;

	// 默认枪械模型为AR4
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mesh(TEXT("'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	if (Mesh.Succeeded())
	{
		SetSkeletalMesh(Mesh.Object);
	}

	// 读取枪械输入上下文
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC(TEXT("'/Game/Input/IMC_Gun.IMC_Gun'"));
	if (IMC.Succeeded())
	{
		GunMappingContext = IMC.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SA(TEXT("'/Game/Input/Actions/IA_Shoot.IA_Shoot'"));
	if (SA.Succeeded())
	{
		ShootAction = SA.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> RA(TEXT("'/Game/Input/Actions/IA_Reload.IA_Reload'"));
	if (RA.Succeeded())
	{
		ReloadAction = RA.Object;
	}

	// 绑定枪支对应的发射物
	if (!ProjectileClass)
	{
		ProjectileClass = CreateDefaultSubobject<ADefaultProjectile>(TEXT("DefaultProjectile"))->GetClass();
	}
}


// Called when the game starts
void UTP_GunComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTP_GunComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindWeapon();
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
	bHasOwner = true;

	// 将武器绑定到第一人称Mesh上
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(OwnerCharacter->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

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
			// 开枪
			EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &UTP_GunComponent::Shoot);

			// 装弹
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &UTP_GunComponent::Reload);
		}
	}
}

void UTP_GunComponent::UnbindWeapon()
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	bHasOwner = false;

	// 移除玩家身上的枪械输入上下文
	if (APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(GunMappingContext);
		}
	}
}

void UTP_GunComponent::Shoot()
{
	// 当枪拥有主人时才发射
	if (!bHasOwner || OwnerCharacter->GetController() == nullptr)
	{
		return;
	}

	// 当发射物类存在时才发射
	if (ProjectileClass)
	{
		UE_LOG(LogTemp, Log, TEXT("开枪"));

		// 获取枪械主人的控制器
		APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());

		// 获取相机朝向
		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		// 发射方向略微向上倾斜
		//SpawnRotation.Pitch += 10.0f;

		// 将枪口偏移从相机空间变换到世界空间
		const FVector SpawnLocation = OwnerCharacter->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = OwnerCharacter;

			// 若生成位置为碰撞物内，则放弃生成
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

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