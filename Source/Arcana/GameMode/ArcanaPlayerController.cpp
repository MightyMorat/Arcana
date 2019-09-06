// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaPlayerController.h"

#include "Camera/CameraComponent.h"
#include "Characters/ArcanaPlayerCharacter.h"
#include "Components/InputComponent.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "FunctionLibraries/ArcanaFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameMode/ArcanaGameMode.h"
#include "InteractiveObjects/InteractiveObjectComponent.h"
#include "Kismet/GameplayStatics.h"

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
	bAutoManageActiveCameraTarget = false;
	bAttachToPawn = true;
}

void AArcanaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
	SetInputMode(InputMode);

	InputComponent->BindKey(EKeys::LeftMouseButton, EInputEvent::IE_Pressed, this, &AArcanaPlayerController::OnLeftClickPressed);
	InputComponent->BindKey(EKeys::LeftMouseButton, EInputEvent::IE_Released, this, &AArcanaPlayerController::OnLeftClickReleased);
	InputComponent->BindKey(EKeys::RightMouseButton, EInputEvent::IE_Pressed, this, &AArcanaPlayerController::OnRightClickPressed);
	InputComponent->BindKey(EKeys::RightMouseButton, EInputEvent::IE_Released, this, &AArcanaPlayerController::OnRightClickReleased);

	InputComponent->BindAxisKey(EKeys::MouseX, this, &AArcanaPlayerController::OnMouseX);
	InputComponent->BindAxisKey(EKeys::MouseWheelAxis, this, &AArcanaPlayerController::OnMouseWheel);
}

void AArcanaPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	AttachCameraToPlayer();
}

AArcanaPlayerCharacter* AArcanaPlayerController::GetPlayerCharacter() const
{
	AArcanaGameMode* GameMode = Cast<AArcanaGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		return GameMode->GetPlayerCharacter();
	}

	return nullptr;
}

void AArcanaPlayerController::AttachCameraToPlayer()
{
	if (GetPawn())
	{
		GetPawn()->AttachToActor(GetPlayerCharacter(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void AArcanaPlayerController::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	AActor::CalcCamera(DeltaTime, OutResult);
}

void AArcanaPlayerController::GetIsRotating(bool& bOutIsRotating, float& OutMouseLocationX, float& OutMouseLocationY) const
{
	bOutIsRotating = bRMBPressed;
	OutMouseLocationX = MouseLocationX;
	OutMouseLocationY = MouseLocationY;
}

void AArcanaPlayerController::OnLeftClickPressed()
{
	if (bRMBPressed)
		return;

	bLMBPressed = true;
	bIsDragging = false;

	if (bHasValidHoveredLocation)
	{
		bHasValidDragLocation = true;
		DragLocation = HoveredLocation;
	}

	if (HoveredInteractiveObjectComponent != SelectedInteractiveObjectComponent)
	{
		DeselectSelectedObject();
	}
}

void AArcanaPlayerController::OnLeftClickReleased()
{
	if (!bLMBPressed)
		return;

	bLMBPressed = false;
	bHasValidDragLocation = false;

	if (!bIsDragging)
	{
		if (HoveredInteractiveObjectComponent != SelectedInteractiveObjectComponent)
		{
			if (SelectedInteractiveObjectComponent)
			{
				SelectedInteractiveObjectComponent->OnDeselected();
			}

			SelectedInteractiveObjectComponent = HoveredInteractiveObjectComponent;

			if (HoveredInteractiveObjectComponent)
			{
				HoveredInteractiveObjectComponent->OnSelected();
			}
		}

		if (!HoveredInteractiveObjectComponent && bHasValidHoveredLocation)
		{
			AArcanaPlayerCharacter* PlayerCharacter = GetPlayerCharacter();
			if (PlayerCharacter)
			{
				PlayerCharacter->QueueMoveAction(HoveredLocation);
			}
		}
	}

	bIsDragging = false;
}

void AArcanaPlayerController::OnRightClickPressed()
{
	if (bLMBPressed)
		return;

	bRMBPressed = true;
	bShowMouseCursor = false;
	GetMousePosition(MouseLocationX, MouseLocationY);

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	DeselectSelectedObject();
}

void AArcanaPlayerController::OnRightClickReleased()
{
	if (!bRMBPressed)
		return;

	bRMBPressed = false;
	SetMouseLocation(FMath::TruncToInt(MouseLocationX), FMath::TruncToInt(MouseLocationY));
	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
	SetInputMode(InputMode);
}

void AArcanaPlayerController::OnMouseX(float AxisValue)
{
	if (FMath::Abs(AxisValue) < 0.05) // todo[hale] - set up deadzone properly
		return;

	AActor* PawnActor = GetPawn();
	if (!PawnActor)
		return;

	const float DeltaSeconds = UArcanaFunctionLibrary::GetRealDeltaSeconds(this);

	if (bRMBPressed)
	{
		RotationInput = FRotator(0.0f, AxisValue * DeltaSeconds * RotationSpeed, 0.0f);
	}
}

void AArcanaPlayerController::OnMouseWheel(float AxisValue)
{
	const float DeltaSeconds = UArcanaFunctionLibrary::GetRealDeltaSeconds(this);

	ZoomAlpha = FMath::Clamp(ZoomAlpha + AxisValue*DeltaSeconds*ZoomAlphaRate, 0.0f, 1.0f);

	if (CameraDistanceCurve)
	{
		SpringArmComponent->TargetArmLength = CameraDistanceCurve->GetFloatValue(ZoomAlpha);
	}

	if (CameraPitchCurve)
	{
		const float CameraPitch = -CameraPitchCurve->GetFloatValue(ZoomAlpha);
		SpringArmComponent->SetRelativeRotation(FRotator(CameraPitch, 0.0f, 0.0f));
	}
}

void AArcanaPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);

	if (bGamePaused)
		return;

	if (ULocalPlayer * LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		FVector2D MousePosition;
		FHitResult HitResult;
		bool bHit = false;
		UInteractiveObjectComponent* NewHoveredInteractiveObjectComponent = nullptr;
		bHasValidHoveredLocation = false;

		UGameViewportClient* ViewportClient = LocalPlayer->ViewportClient;

		// Ignore hits if we're not over the viewport
		if (bShowMouseCursor && IsInViewportClient(ViewportClient))
		{
			if (ViewportClient->GetMousePosition(MousePosition))
			{
				bHit = GetHitResultAtScreenPosition(MousePosition, CurrentClickTraceChannel, true, /*out*/ HitResult);
				bHasValidHoveredLocation = bHit;
				HoveredLocation = HitResult.Location;
			}
		}

		if (bHit && HitResult.Actor.IsValid())
		{
			NewHoveredInteractiveObjectComponent = Cast<UInteractiveObjectComponent>(HitResult.Actor.Get()->GetComponentByClass(UInteractiveObjectComponent::StaticClass()));
		}

		// Hovered object has updated. Send events.
		if (HoveredInteractiveObjectComponent != NewHoveredInteractiveObjectComponent)
		{
			if (HoveredInteractiveObjectComponent)
			{
				HoveredInteractiveObjectComponent->OnUnhovered();
			}

			HoveredInteractiveObjectComponent = NewHoveredInteractiveObjectComponent;

			if (NewHoveredInteractiveObjectComponent)
			{
				NewHoveredInteractiveObjectComponent->OnHovered();
			}
		}
	}
}

void AArcanaPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PostProcessInput(DeltaTime, bGamePaused);

	if (ULocalPlayer * LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		UGameViewportClient* ViewportClient = LocalPlayer->ViewportClient;
		if (bShowMouseCursor && bLMBPressed && bHasValidDragLocation && IsInViewportClient(ViewportClient))
		{
			FVector2D MousePosition;
			if (ViewportClient->GetMousePosition(MousePosition))
			{
				FVector WorldOrigin;
				FVector WorldDirection;
				if (UGameplayStatics::DeprojectScreenToWorld(this, MousePosition, WorldOrigin, WorldDirection) == true)
				{
					// Find the intersection between the line under the mouse and a horizontal plane through the DragLocation
					if (FMath::Abs(WorldDirection.Z) > SMALL_NUMBER)
					{
						float T = (DragLocation.Z - WorldOrigin.Z) / WorldDirection.Z;
						FVector IntersectionPoint = WorldOrigin + T * WorldDirection;

						FVector TargetDisplacement = DragLocation - IntersectionPoint;
						TargetDisplacement.Z = 0.0f; // Just to avoid any drift from precision errors

						// Consider the player to be dragging only after pulling a certain distance
						if (!bIsDragging && TargetDisplacement.SizeSquared2D() >= DragDistanceThreshold * DragDistanceThreshold)
						{
							bIsDragging = true;
						}

						if (bIsDragging)
						{
							AActor* PawnActor = GetPawn();
							if (PawnActor)
							{
								PawnActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
								PawnActor->AddActorWorldOffset(TargetDisplacement);
							}
						}
					}
				}
			}
		}
	}
}

void AArcanaPlayerController::DeselectSelectedObject()
{
	if (SelectedInteractiveObjectComponent)
	{
		SelectedInteractiveObjectComponent->OnDeselected();
		SelectedInteractiveObjectComponent = nullptr;
	}
}