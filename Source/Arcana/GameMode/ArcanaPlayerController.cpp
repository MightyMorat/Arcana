// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaPlayerController.h"

#include "Classes/Camera/CameraComponent.h"
#include "Classes/GameFramework/SpringArmComponent.h"

AArcanaPlayerController::AArcanaPlayerController()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->TargetArmLength = 1800.0f;
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	SpringArmComponent->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	bAutoManageActiveCameraTarget = false;
}

void AArcanaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
	InputMode.SetWidgetToFocus(nullptr);
	SetInputMode(InputMode);
}

void AArcanaPlayerController::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	AActor::CalcCamera(DeltaTime, OutResult);
}