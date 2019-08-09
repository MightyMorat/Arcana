// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArcanaPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARCANA_API AArcanaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AArcanaPlayerController();

	virtual void BeginPlay() override;
	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* CameraComponent;
};
