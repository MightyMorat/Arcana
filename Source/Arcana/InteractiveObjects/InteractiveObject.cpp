// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveObject.h"

#include "Engine/Classes/Components/PrimitiveComponent.h"
#include "Engine.h"

void AInteractiveObject::NotifyActorBeginCursorOver()
{
	Super::NotifyActorBeginCursorOver();

	for (UActorComponent* ActorComponent : GetComponents())
	{
		UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(ActorComponent);
		if (PrimComp && PrimComp->bRenderCustomDepth)
		{
			PrimComp->SetCustomDepthStencilValue(1);
		}
	}
}

void AInteractiveObject::NotifyActorEndCursorOver()
{
	Super::NotifyActorEndCursorOver();

	for (UActorComponent* ActorComponent : GetComponents())
	{
		UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(ActorComponent);
		if (PrimComp && PrimComp->bRenderCustomDepth)
		{
			PrimComp->SetCustomDepthStencilValue(0);
		}
	}
}

void AInteractiveObject::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Object Clicked: %s"), *GetName()));
}