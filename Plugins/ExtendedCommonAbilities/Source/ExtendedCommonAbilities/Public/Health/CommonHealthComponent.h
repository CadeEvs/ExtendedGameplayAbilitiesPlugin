﻿// Copyright Bohdon Sayre, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "NativeGameplayTags.h"
#include "Components/ActorComponent.h"
#include "CommonHealthComponent.generated.h"

class UAbilitySystemComponent;


UENUM(BlueprintType)
enum class ECommonHealthState : uint8
{
	Alive,
	DeathStarted,
	DeathFinished,
};


/**
 * Handles events related to life and death.
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class EXTENDEDCOMMONABILITIES_API UCommonHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCommonHealthComponent(const FObjectInitializer& ObjectInitializer);

	/**
	 * The attribute that represents the character's main health.
	 * The gameplay event 'Event.Death' is sent when this reaches 0.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	FGameplayAttribute HealthAttribute;

	/** Automatically set the ability system by retrieving it from the owning actor. */
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	bool bAutoRegisterAbilitySystem;

	/** Send a message through the UGameplayMessageSubsystem on death. */
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	bool bSendGameplayMessage;

	/** The channel on which to send the death message via the UGameplayMessageSubsystem. */
	UPROPERTY(EditDefaultsOnly, Meta = (EditCondition = "bSendGameplayMessage"), Category = "Health")
	FGameplayTag GameplayMessageChannel;

	/** The current state of health. */
	UPROPERTY(VisibleAnywhere, Replicated, ReplicatedUsing=OnRep_HealthState, Category = "Health")
	ECommonHealthState HealthState;

	UFUNCTION()
	void OnRep_HealthState(ECommonHealthState OldHealthState);

	UFUNCTION(BlueprintPure, Category = "CommonAbilities|Health")
	bool IsAlive() const { return HealthState <= ECommonHealthState::Alive; }

	/** Trigger death from self destruction. */
	UFUNCTION(BlueprintCallable, Category = "CommonAbilities|Health")
	void TriggerDeathFromSelfDestruct();

	/** Trigger death for custom reasons. */
	UFUNCTION(BlueprintCallable, Meta = (GameplayTagFilter = "Event.Death"), Category = "CommonAbilities|Health")
	void TriggerDeath(AActor* Instigator, FGameplayEffectContextHandle Context, FGameplayTag DeathEventTag);

	/**
	 * Begin dying, called automatically when HP is depleted.
	 * Can be called when HP is not 0 to prematurely kill a character or object.
	 */
	UFUNCTION(BlueprintCallable, Category = "CommonAbilities|Health")
	void StartDeath();

	/**
	 * Finish dying, intended to be called from death abilities after animation or other fx.
	 * StartDeath must be called before this.
	 */
	UFUNCTION(BlueprintCallable, Category = "CommonAbilities|Health")
	void FinishDeath();

	/** Set the ability system to use for tracking health. */
	UFUNCTION(BlueprintCallable, Category = "CommonAbilities|Health")
	void SetAbilitySystem(UAbilitySystemComponent* InAbilitySystem);

	/** Clear the ability system from this component and clear its death tags. */
	UFUNCTION(BlueprintCallable, Category = "CommonAbilities|Health")
	void ClearAbilitySystem();

	DECLARE_MULTICAST_DELEGATE_OneParam(FHealthStateChangedDelegate, AActor* /*OwningActor*/);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthStateChangedDynDelegate, AActor*, OwningActor);

	/** Called the moment the character has died. */
	FHealthStateChangedDelegate OnDeathStartedEvent;

	/** Called the moment the character has died. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnDeathStartedEvent")
	FHealthStateChangedDynDelegate OnDeathStartedEvent_BP;

	/** Called after playing any death animation or fx. */
	FHealthStateChangedDelegate OnDeathFinishedEvent;

	/** Called after playing any death animation or fx. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnDeathFinishedEvent")
	FHealthStateChangedDynDelegate OnDeathFinishedEvent_BP;

protected:
	/** Ability system being monitored. */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystem;

	virtual void InitializeComponent() override;
	virtual void OnUnregister() override;

	void ClearGameplayTags();

	void OnHPChanged(const FOnAttributeChangeData& ChangeData);

	/** Called when health state has changed to DeathStarted. */
	void OnDeathStarted();

	/** Called when health state has changed to DeathFinished. */
	void OnDeathFinished();

public:
	/** Return the CommonHealthComponent of an actor, if one exists. */
	UFUNCTION(BlueprintPure, Category = "CommonAbilities|Health")
	static UCommonHealthComponent* GetHealthComponent(const AActor* Actor)
	{
		return Actor ? Actor->FindComponentByClass<UCommonHealthComponent>() : nullptr;
	}
};
