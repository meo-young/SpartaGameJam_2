#include "DdakjiCharacter.h"

#include "YutPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ADdakjiCharacter::ADdakjiCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-30.0f, -90.0f, 0.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	Camera->SetupAttachment(SpringArm);
}

void ADdakjiCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 점프 중인 경우에만 로직이 실행됩니다.
	if (bIsJumping && JumpSquareCurve)
	{
		ElapsedTime += DeltaTime;
		float Alpha = ElapsedTime / JumpDuration;

		// 점프 최대 시간이 경과한 경우에 이동을 중단합니다.
		if (Alpha >= 1.f)
		{
			SetActorLocation(TargetPos);
			bIsJumping = false;
			return;
		}

		// Curve 시간 입력 (Curve 키 범위가 0~2라면 Alpha * 2)
		float CurveValue = JumpSquareCurve->GetFloatValue(Alpha * 2.f);

		// 수평 이동하는 로직입니다.
		FVector CurrentPos = FMath::Lerp(StartPos, TargetPos, Alpha);

		// 수직 오프셋 (CurveValue는 0~1이므로 MaxHeight 곱)
		CurrentPos.Z = FMath::Lerp(StartPos.Z, TargetPos.Z, Alpha) + CurveValue * MaxHeight;

		SetActorLocation(CurrentPos);
	}
}

void ADdakjiCharacter::BlendToPlayerCamera()
{
	if (AYutPlayerController* PC = Cast<AYutPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (Camera)
		{
			// 블렌드 파라미터를 선언합니다.
			FViewTargetTransitionParams TransitionParams;
			TransitionParams.BlendTime = 1.0f;
			TransitionParams.BlendFunction = VTBlend_Cubic;

			// PlayerController의 시점을 DdakjiCharacter의 Camera로 변경합니다.
			PC->SetViewTargetWithBlend(this, TransitionParams.BlendTime, TransitionParams.BlendFunction);
		}
	}
}

void ADdakjiCharacter::JumpToLocation(FVector TargetLocation)
{
	StartPos = GetActorLocation();
	TargetPos = TargetLocation;
	TargetPos.Z = GetActorLocation().Z;
	ElapsedTime = 0.f;
	bIsJumping = true;
}
