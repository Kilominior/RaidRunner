// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_GunComponent.h"
#include "Net/UnrealNetwork.h"
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
	MuzzleOffset.Set(180.0f, 0.0f, 100.0f);

	// 设置默认子弹状态，为满仓
	MagazineCapacity = 25;
	CurrentAmmoNum = MagazineCapacity;
	
	BulletDamage = 10.0f;

	// 默认情况下没有主人
	bHasOwner = false;

	// 默认枪械模型为AR4
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mesh(TEXT("'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	if (Mesh.Succeeded())
	{
		SetSkeletalMesh(Mesh.Object);
	}

	// 绑定枪支对应的发射物
	if (!ProjectileClass)
	{
		ProjectileClass = CreateDefaultSubobject<ADefaultProjectile>(TEXT("DefaultProjectile"))->GetClass();
	}

	// 初始化射速
	FireRate = 0.15f;
	bIsFiringWeapon = false;

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

		// 根据枪械种类对输入操作添加触发器，以模拟不同枪械的开枪频率效果
		//UInputTriggerPulse* ShootTrigger = CreateDefaultSubobject<UInputTriggerPulse>(TEXT("ShootTrigger"));
		//ShootTrigger->Interval = FireRate;
		//ShootAction->Triggers.Add(ShootTrigger);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> RA(TEXT("'/Game/Input/Actions/IA_Reload.IA_Reload'"));
	if (RA.Succeeded())
	{
		ReloadAction = RA.Object;
	}
}

void UTP_GunComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 复制当前枪械数据
	DOREPLIFETIME(UTP_GunComponent, CurrentAmmoNum);
	DOREPLIFETIME(UTP_GunComponent, bHasOwner);
	DOREPLIFETIME(UTP_GunComponent, OwnerCharacter);
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
}

void UTP_GunComponent::UnbindWeapon()
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	bHasOwner = false;

	// switch bHasRifle so the animation blueprint can switch to another animation set
	//OwnerCharacter->SetHasRifle(false);
}

void UTP_GunComponent::BindInput()
{
	// 仅在客户端执行
	if (OwnerCharacter->IsLocallyControlled())
	{
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
				EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &UTP_GunComponent::StartFire);

				// 装弹
				EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &UTP_GunComponent::Reload);
			}
		}
	}
}

void UTP_GunComponent::UnbindInput()
{
	// 仅在客户端执行
	if (OwnerCharacter->IsLocallyControlled())
	{
		// 移除玩家身上的枪械输入上下文
		if (APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(GunMappingContext);
			}
		}
	}
}

void UTP_GunComponent::StartFire()
{
	// 当枪拥有主人时才发射
	if (!bHasOwner || OwnerCharacter->GetController() == nullptr)
	{
		return;
	}

	// 若玩家未处于发射延迟，则启动新延迟并开始发射
	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FiringTimer, this, &UTP_GunComponent::StopFire, FireRate, false);
		HandleFire();
	}
}

void UTP_GunComponent::StopFire()
{
	bIsFiringWeapon = false;
}

void UTP_GunComponent::HandleFire_Implementation()
{
	// 当发射物类存在时才发射
	if (!ProjectileClass)
	{
		return;
	}

	// 当枪械仍有子弹时才发射
	if (CurrentAmmoNum <= 0)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("开枪，当前子弹数%d"), CurrentAmmoNum);

	// 当前子弹数减少
	CurrentAmmoNum--;

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
		// 子弹的驱使者是玩家，主人是枪
		SpawnParams.Instigator = OwnerCharacter->GetInstigator();
		SpawnParams.Owner = GetOwner();

		// 若生成位置为碰撞物内，尝试寻找最近可生成位置
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// 在枪口位置生成子弹
		ADefaultProjectile* Projectile = World->SpawnActor<ADefaultProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (Projectile)
		{
			// 设置子弹的初始轨迹
			FVector LaunchDirection = SpawnRotation.Vector();
			Projectile->FireInDirection(LaunchDirection);

			// 设置子弹的属性
			Projectile->SetDamage(BulletDamage);
		}
	}
}

void UTP_GunComponent::Reload()
{
	// 枪内子弹数量为满时不装弹
	if (CurrentAmmoNum == MagazineCapacity)
	{
		return;
	}

	// TODO: 角色持有子弹数量为0时不装弹

	// 当发射物类存在时才装弹
	if (!ProjectileClass)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("开始上弹，弹容量%d"), MagazineCapacity);
	SetCurrentAmmoNum(MagazineCapacity);
}

void UTP_GunComponent::SetCurrentAmmoNum_Implementation(const int& Num)
{
	CurrentAmmoNum = Num;
}

void UTP_GunComponent::OnRep_HasOwner()
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	// 根据主人状态绑定或解绑输入
	if (bHasOwner)
	{
		BindInput();
	}
	else
	{
		UnbindInput();
	}
}
