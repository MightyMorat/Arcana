// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveObjectComponent.h"

#include "Engine.h"
#include "GameFramework/Actor.h"

void UInteractiveObjectComponent::OnHovered()
{
	bIsHovered = true;

	if(!bIsSelected)
		SetActorOutlineEnabled(true);
}

void UInteractiveObjectComponent::OnUnhovered()
{
	bIsHovered = false;

	if (!bIsSelected)
		SetActorOutlineEnabled(false);
}

void UInteractiveObjectComponent::OnSelected()
{
	bIsSelected = true;

	if (!bIsHovered)
		SetActorOutlineEnabled(true);
}

void UInteractiveObjectComponent::OnDeselected()
{
	bIsSelected = false;

	if (!bIsHovered)
		SetActorOutlineEnabled(false);
}

void UInteractiveObjectComponent::SetActorOutlineEnabled(bool bEnable)
{
	AActor* OwningActor = GetOwner();
	if (!OwningActor)
		return;

	for (UActorComponent* ActorComponent : OwningActor->GetComponents())
	{
		if (UPrimitiveComponent * PrimComp = Cast<UPrimitiveComponent>(ActorComponent))
		{
			PrimComp->SetRenderCustomDepth(bEnable);
			PrimComp->SetCustomDepthStencilValue((int32)bEnable);
		}
	}
}

