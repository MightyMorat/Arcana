// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObject.h"

#include "Engine/Classes/Components/PrimitiveComponent.h"

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
}