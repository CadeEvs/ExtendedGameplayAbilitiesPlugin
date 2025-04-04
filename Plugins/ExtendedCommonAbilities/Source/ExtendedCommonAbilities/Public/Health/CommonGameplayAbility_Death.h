﻿// Copyright Bohdon Sayre, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExtendedGameplayAbility.h"
#include "NativeGameplayTags.h"
#include "CommonGameplayAbility_Death.generated.h"

class UCommonHealthComponent;


/**
 * An ability for handling death. Triggered by 'Event.Death', this ability calls
 * StartDeath, then after optional animation or fx, FinishDeath.
 * The CommonHealthComponent will then broadcast appropriate events for other game systems
 * to respond to, such as respawning the player.
 */
UCLASS()
class EXTENDEDCOMMONABILITIES_API UCommonGameplayAbility_Death : public UExtendedGameplayAbility
{
	GENERATED_BODY()

public:
	UCommonGameplayAbility_Death(const FObjectInitializer& ObjectInitializer);

	/** Automatically call StartDeath when the ability is activated. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	bool bStartDeathOnActivate = true;

	/** Automatically cancel abilities on death. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	bool bCancelAbilities = true;

	/** Don't cancel abilities that have any of these tags. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death", meta = (EditCondition = "bCancelAbilities"))
	FGameplayTagContainer CancelAbilityIgnoreTags;

	/** Automatically remove gameplay effects on death. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	bool bRemoveGameplayEffects = true;

	/** Don't remove gameplay effects that have any of these tags. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death", meta = (EditCondition = "bRemoveGameplayEffects"))
	FGameplayTagContainer RemoveEffectIgnoreTags;

	/** Return the health component of the owning avatar. */
	UFUNCTION(BlueprintPure, Category = "CommonAbilities|Death")
	UCommonHealthComponent* GetHealthComponent() const;

	/** Start death on the owner, called automatically on activate if bAutoStartDeath is true. */
	UFUNCTION(BlueprintCallable, Category = "CommonAbilities|Death")
	void StartDeath();

	/** Finish death on the owner, called automatically on EndAbility if not called sooner. */
	UFUNCTION(BlueprintCallable, Category = "CommonAbilities|Death")
	void FinishDeath();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	/**
	 * The health component on which death was started. It's possible that the avatar may change
	 * before end ability is called, in which case we want to call FinishDeath on the same health component that was started.
	 */
	UPROPERTY()
	TWeakObjectPtr<UCommonHealthComponent> HealthComponent;
};
