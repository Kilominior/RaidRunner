// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerCharacter.h"
#include "BrawlGameMode.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include "Net/UnrealNetwork.h"
#include "Gun.h"

// Sets default values
ARunnerCharacter::ARunnerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* 相机 */
	// 创建相机组件
	RunnerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(RunnerCameraComponent != nullptr);

	// 绑定到胶囊组件
	RunnerCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	// 将相机绑定到第一人称Mesh上
	//FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	//RunnerCameraComponent->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("CameraPoint")));

	// 移动到高于模型的位置
	RunnerCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

	// 允许Pawn控制相机
	RunnerCameraComponent->bUsePawnControlRotation = true;


	/* 第一人称Mesh */
	// 创建第一人称Mesh组件
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(Mesh1P != nullptr);

	// 只允许拥有者看见
	Mesh1P->SetOnlyOwnerSee(true);

	// 绑定到相机组件
	Mesh1P->SetupAttachment(RunnerCameraComponent);

	// 关闭阴影投射
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;

	// 不允许玩家看见第三人称Mesh
	GetMesh()->SetOwnerNoSee(true);

	/* 生命值组件 */
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	check(HealthComponent != nullptr);
	HealthComponent->SetIsReplicated(true);

	/* 武器槽位状态 */
	CurrentWeaponSlot = 0;
	SwitchingWeaponTo = 0;
}

void ARunnerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 复制当前玩家数据
	DOREPLIFETIME(ARunnerCharacter, CurrentWeaponSlot);
	DOREPLIFETIME(ARunnerCharacter, CurrentWeapon);
}

// Called when the game starts or when spawned
void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		// 显示调试消息五秒
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Hello, this is RunnerCharacter!"));
	}

	// 注册生命值变化事件，以调用角色自身的生命值变化方法
	HealthComponent->OnHealthChanged.AddDynamic(this, &ARunnerCharacter::OnHealthChanged);

	// 默认持有武器1
	//OnWeaponChange(1);
}

void ARunnerCharacter::Destroyed()
{
	// 在销毁前存下自己控制器的引用
	AController* PlayerController = GetController();

	Super::Destroyed();

	// 作为服务器执行操作
	if (GetLocalRole() == ROLE_Authority)
	{
		// 若持有武器，则先将其销毁
		if (CurrentWeaponSlot != 0)
		{
			CurrentWeapon->Destroy();
		}

		// 向目前的GameMode广播自己的死亡
		if (UWorld* World = GetWorld())
		{
			if (ABrawlGameMode* GameMode = Cast<ABrawlGameMode>(World->GetAuthGameMode()))
			{
				GameMode->GetOnPlayerDied().Broadcast(this, PlayerController);
			}
		}
	}
}

// Called every frame
void ARunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 绑定输入
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 视角转动
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARunnerCharacter::Look);

		// 角色移动
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARunnerCharacter::Move);

		// 跳跃与停止
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// 尝试切换武器
		EnhancedInputComponent->BindAction(SlotAction1, ETriggerEvent::Triggered, this, &ARunnerCharacter::TrySlotChange, 1);
		EnhancedInputComponent->BindAction(SlotAction2, ETriggerEvent::Triggered, this, &ARunnerCharacter::TrySlotChange, 2);
	
		// 完成切换武器
		EnhancedInputComponent->BindAction(SlotAction1, ETriggerEvent::Completed, this, &ARunnerCharacter::EndSlotChange, 1);
		EnhancedInputComponent->BindAction(SlotAction2, ETriggerEvent::Completed, this, &ARunnerCharacter::EndSlotChange, 2);

	}
}

void ARunnerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	float YawValue = LookAxisVector.X;
	float PitchValue = -LookAxisVector.Y;

	// TODO: 最终转向的方向受灵敏度影响
	if (Controller != nullptr)
	{
		AddControllerYawInput(YawValue);
		AddControllerPitchInput(PitchValue);
	}
}

void ARunnerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementDir = Value.Get<FVector2D>();

	// TODO: 最终施加的力为移动方向 * 速度
	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementDir.Y * 0.6f);
		AddMovementInput(GetActorRightVector(), MovementDir.X * 0.6f);
		//UE_LOG(LogTemp, Log, TEXT("行动方向：%f, %f"), MovementDir.X, MovementDir.Y);
	}
}

void ARunnerCharacter::TrySlotChange(const int SlotId)
{
	// 当目前已有其他尝试切换武器的按键按下，则无视该次操作
	// 当目前所持有武器已经为目标武器，则无视该次操作
	if (SwitchingWeaponTo != 0 || CurrentWeaponSlot == SlotId)
	{
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("尝试切换到槽位%d"), SlotId);
	SwitchingWeaponTo = SlotId;
}

void ARunnerCharacter::EndSlotChange(const int SlotId)
{
	// 当目前还有其他尝试切换武器的按键按下，则无视该次操作
	// 当目前所持有武器已经为目标武器，则无视该次操作
	if (SwitchingWeaponTo != SlotId || CurrentWeaponSlot == SlotId)
	{
		return;
	}

	SlotChangeTo(SlotId);
}

void ARunnerCharacter::SlotChangeTo(const int SlotId)
{
	UE_LOG(LogTemp, Log, TEXT("切换到槽位%d的武器"), SlotId);

	// 服务器进行武器的创建和销毁
	OnWeaponChange(SlotId);

	// 完成切换武器后更新切换武器状态记录
	SwitchingWeaponTo = 0;
}

void ARunnerCharacter::OnWeaponChange_Implementation(const int SlotId)
{
	UWorld* World = GetWorld();
	if (World)
	{
		// 销毁原来的武器
		if (CurrentWeaponSlot != 0) {
			UE_LOG(LogTemp, Log, TEXT("销毁原有武器"));
			if (CurrentWeapon != nullptr)
			{
				// 解绑
				CurrentWeapon->UnbindWeapon();

				// 销毁
				CurrentWeapon->Destroy();
			}
		}

		// 初始化生成参数
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		if (SlotId == 1)
		{

			// 获取相机朝向
			FRotator SpawnRotation = RunnerCameraComponent->GetComponentRotation();
			// 枪支方向略微偏斜
			//SpawnRotation.Yaw += 10.0f;

			// 将枪械位置从相机空间变换到世界空间
			FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(FVector(0.0, 0.0, 0.0));

			// 生成武器
			CurrentWeapon = World->SpawnActor<AGun>(SpawnLocation, SpawnRotation, SpawnParams);
			CurrentWeapon->AttachWeapon(this);
		}
		else if (SlotId == 2)
		{
			
		}

		/*switch (SlotId)
		{
		case 1:
				break;
		case 2:
			break;

		default:
			break;
		}*/

		// 完成切换武器后更新切换武器状态记录
		CurrentWeaponSlot = SlotId;
	}
}

void ARunnerCharacter::OnHealthChanged(UHealthComponent* HealthComp, float Health, float DamageAmount, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	// 在实际生命值中扣除伤害数值
	HealthComp->SetCurrentHealth(Health - DamageAmount);
	// TODO：复活
}
