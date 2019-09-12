// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveObjectComponent.h"

#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Conditions/ArcanaCondition.h"
#include "Engine.h"
#include "GameFramework/Actor.h"

UInteractiveObjectComponent::UInteractiveObjectComponent()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> InteractOptionsWidgetClassFinder(TEXT("/Game/UI/Widgets/HUD/UI_BP_InteractOptions"));
	if (InteractOptionsWidgetClassFinder.Succeeded())
	{
		InteractOptionsWidgetClass = InteractOptionsWidgetClassFinder.Class;

		InteractOptionsWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractOptionsWidgetComponent"));
		InteractOptionsWidgetComponent->SetDrawAtDesiredSize(true);
		InteractOptionsWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		InteractOptionsWidgetComponent->SetupAttachment(this);
	}
}

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
	{
		SetActorOutlineEnabled(true);
	}

	if (InteractOptionsWidgetComponent)
	{
		InteractOptionsWidgetComponent->SetWidgetClass(InteractOptionsWidgetClass);
	}
}

void UInteractiveObjectComponent::OnDeselected()
{
	bIsSelected = false;

	if (!bIsHovered)
	{
		SetActorOutlineEnabled(false);
	}

	if (InteractOptionsWidgetComponent)
	{
		InteractOptionsWidgetComponent->SetWidgetClass(nullptr);
	}
}

void UInteractiveObjectComponent::SetActorOutlineEnabled(bool bEnable)
{
	for (UActorComponent* ActorComponent : GetOwner()->GetComponents())
	{
		if (UPrimitiveComponent * PrimComp = Cast<UPrimitiveComponent>(ActorComponent))
		{
			PrimComp->SetRenderCustomDepth(bEnable);
			PrimComp->SetCustomDepthStencilValue((int32)bEnable);
		}
	}
}

TArray<FArcanaAction> UInteractiveObjectComponent::GetAvailableActions() const
{
	TArray<FArcanaAction> AvailableActions;
	if (!ActionGroup)
		return AvailableActions;

	for (const FArcanaAction& Action : ActionGroup->Actions)
	{
		bool bConditionSuccess = true;
		for (const FArcanaActionCondition& ActionCondition : Action.Conditions)
		{
			if (ActionCondition.Condition && !ActionCondition.Condition->IsConditionMet(this))
			{
				bConditionSuccess = false;
				if (!ActionCondition.bHideOnFailure)
				{
					FArcanaAction& AddedAction = AvailableActions.Add_GetRef(Action);
					AddedAction.bIsEnabled = false;
					if (!ActionCondition.FailureNameOverride.IsEmpty())
					{
						AddedAction.ActionName = ActionCondition.FailureNameOverride;
					}
				}
				break;
			}
		}

		if (bConditionSuccess)
		{
			FArcanaAction& AddedAction = AvailableActions.Add_GetRef(Action);
			AddedAction.bIsEnabled = true;
		}
	}

	return AvailableActions;
}

FVector UInteractiveObjectComponent::GetLocatorPosition(FName Tag) const
{
	TArray<USceneComponent*> Locators;
	GetOwner()->GetComponents<USceneComponent>(Locators);

	for (const USceneComponent* Locator : Locators)
	{
		if (Locator && Locator->ComponentTags.Contains(Tag))
			return Locator->GetComponentLocation();
	}

	ensureMsgf(false, TEXT("Locator not found. Actor: %s, Tag: %s"), *GetOwner()->GetName(), *Tag.ToString());
	return GetComponentLocation();
}