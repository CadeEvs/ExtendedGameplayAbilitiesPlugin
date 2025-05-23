﻿// Copyright Bohdon Sayre, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectSet.h"
#include "ExtendedAbilitySystemComponent.generated.h"

class UExtendedAbilitySet;
class UExtendedAbilityTagRelationshipMapping;


/**
 * Extends the AbilitySystemComponent with support for gameplay effect sets and more.
 */
UCLASS(Meta = (BlueprintSpawnableComponent))
class EXTENDEDGAMEPLAYABILITIES_API UExtendedAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UExtendedAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Loose gameplay tags to add to this ability system, usually character type, object type, or other traits. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	FGameplayTagContainer DefaultTags;

	/** Abilities, effects, and attribute sets to grant at startup. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	TArray<TObjectPtr<UExtendedAbilitySet>> StartupAbilitySets;

	/** Mapping that defines additional relationships for how abilities block or cancel other abilities. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TObjectPtr<UExtendedAbilityTagRelationshipMapping> AbilityTagRelationshipMapping;

	/**
	 * Create and return an effect spec set.
	 * The spec set can then be applied using ApplyEffectContainerToSelf on this or another ability system.
	 */
	UFUNCTION(BlueprintCallable, Category = "GameplayEffects")
	FGameplayEffectSpecSet MakeEffectSpecSet(const FGameplayEffectSet& EffectSet, float Level);

	/**
	 * Apply all effects from an effect spec set to this ability system.
	 * @return All active gameplay effect handles for any applied effects.
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "ApplyGameplayEffectSpecSetToSelf", Category = "GameplayEffects")
	TArray<FActiveGameplayEffectHandle> ApplyGameplayEffectSpecSetToSelf(const FGameplayEffectSpecSet& EffectSpecSet);

	/** Cancel all abilities with the given state tags. */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void CancelAbilitiesWithState(FGameplayTagContainer WithStateTags, UGameplayAbility* IgnoreAbility);

	virtual void InitializeComponent() override;
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	virtual void ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility,
	                                            bool bEnableBlockTags, const FGameplayTagContainer& BlockTags,
	                                            bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags) override;

	/** Get any additional required and blocked tags needed for ability activation. */
	virtual void GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags,
														FGameplayTagContainer& OutRequiredTags,
														FGameplayTagContainer& OutBlockedTags) const;

	/** Called when ability input has been pressed by tag. */
	void AbilityTagInputPressed(const FGameplayTag& InputTag);

	/** Called when ability input has been released by tag. */
	void AbilityTagInputReleased(const FGameplayTag& InputTag);

	/** Sends a local player Input Pressed event by input tag, notifying any bound abilities. */
	UFUNCTION(BlueprintCallable, Meta = (AutoCreateRefTerm = "InputTag"), Category = "Gameplay Abilities")
	void PressInputTag(const FGameplayTag& InputTag);

	/** Sends a local player Input Released event by input tag, notifying any bound abilities. */
	UFUNCTION(BlueprintCallable, Meta = (AutoCreateRefTerm = "InputTag"), Category = "Gameplay Abilities")
	void ReleaseInputTag(const FGameplayTag& InputTag);

	DECLARE_MULTICAST_DELEGATE_OneParam(FAbilityAddOrRemoveDelegate, FGameplayAbilitySpec& /*AbilitySpec*/);

	/** Called when a new ability is added. */
	FAbilityAddOrRemoveDelegate OnGiveAbilityEvent;

	/** Called when an ability is removed. */
	FAbilityAddOrRemoveDelegate OnRemoveAbilityEvent;
};
