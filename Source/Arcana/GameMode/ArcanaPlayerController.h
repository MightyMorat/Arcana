// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArcanaPlayerController.generated.h"

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

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void GetIsRotating(bool& bOutIsRotating, float& OutMouseLocationX, float& OutMouseLocationY) const;

	UFUNCTION(BlueprintCallable)
	void DeselectSelectedObject();

protected:
	void OnLeftClickPressed();
	void OnLeftClickReleased();
	void OnRightClickPressed();
	void OnRightClickReleased();

	void OnMouseX(float AxisValue);
	void OnMouseY(float AxisValue);
	void OnMouseWheel(float AxisValue);

	bool bRMBPressed = false;
	bool bLMBPressed = false;
	float MouseLocationX = 0.0f;
	float MouseLocationY = 0.0f;

	UPROPERTY()
	class UInteractiveObjectComponent* HoveredInteractiveObjectComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	class UInteractiveObjectComponent* SelectedInteractiveObjectComponent = nullptr;

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

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* DragSpeedCurve = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* CameraComponent;
};
