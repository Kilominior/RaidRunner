// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerCharacter.h"
#include "../../../../../Extra_Libs_and_Include/UnrealEngine/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../../Extra_Libs_and_Include/UnrealEngine/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

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
}

// Called when the game starts or when spawned
void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// 显示调试消息五秒
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Using Runner Character."));

	// 找到角色控制器，以此获取增强输入上下文并添加到子系统
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
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
		AddMovementInput(GetActorForwardVector(), MovementDir.Y);
		AddMovementInput(GetActorRightVector(), MovementDir.X);
		UE_LOG(LogTemp, Log, TEXT("行动方向：%f, %f"), MovementDir.X, MovementDir.Y);
	}
}