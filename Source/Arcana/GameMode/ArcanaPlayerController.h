// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArcanaPlayerController.generated.h"

class AArcanaPlayerCharacter;
class UCameraComponent;
class UInteractiveObjectComponent;
class USpringArmComponent;

/**
 * 
 */
UCLASS(BlueprintType)
class ARCANA_API AArcanaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AArcanaPlayerController();

	virtual void BeginPlay() override;
	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void OnPossess(APawn* aPawn) override;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void GetIsRotating(bool& bOutIsRotating, float& OutMouseLocationX, float& OutMouseLocationY) const;

	UFUNCTION(BlueprintCallable)
	void DeselectSelectedObject();

	UFUNCTION(BlueprintCallable)
	void AttachCameraToPlayer();

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	AArcanaPlayerCharacter* GetPlayerCharacter() const;

protected:
	void OnLeftClickPressed();
	void OnLeftClickReleased();
	void OnRightClickPressed();
	void OnRightClickReleased();

	void OnPause();

	void OnMouseX(float AxisValue);
	void OnMouseWheel(float AxisValue);

	bool bRMBPressed = false;
	bool bLMBPressed = false;
	float MouseLocationX = 0.0f;
	float MouseLocationY = 0.0f;

	bool bHasValidDragLocation = false;
	FVector DragLocation = FVector::ZeroVector;
	bool bIsDragging = false;

	UPROPERTY(EditDefaultsOnly)
	float DragDistanceThreshold = 40.0f;

	UPROPERTY()
	UInteractiveObjectComponent* HoveredInteractiveObjectComponent = nullptr;

	bool bHasValidHoveredLocation = false;
	FVector HoveredLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	UInteractiveObjectComponent* SelectedInteractiveObjectComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float ZoomAlpha = 0.4f;

	UPROPERTY(EditDefaultsOnly)
	float ZoomAlphaRate = 3.5f;

	UPROPERTY(EditDefaultsOnly)
	float RotationSpeed = 360.0f;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* CameraDistanceCurve = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* CameraPitchCurve = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;

	bool bHasLevelBounds = false;
	FBox LevelBounds;
};
