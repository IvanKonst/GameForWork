// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"
#include "GameFramework/Character.h"
#include "Structures.h"
#include "FootballCharacter.generated.h"

class ABattleManager;
class AGoalGate;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDChangedState);

UCLASS()
class AIPROJECT_API AFootballCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category="Dispatchers")
	FDChangedState ChangedStateDelegate;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="NPC bools")
	bool bCanMove;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="NPC bools")
	bool bIsSelected;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="NPC bools")
	bool bHasBall;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="NPC bools")
	bool IsInCooldown;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="NPC states")
	EAttackState OffenseState;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="NPC states")
	EAttackTypes CurrentAttackType;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="NPC states")
	EPStates PlayerStates;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="NPC states")
	EPStates LastPlayerState;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="NPC")
	FPlayerProperties PlayerProperties;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="NPC references")
	AFootballCharacter *IdealPassMateReference;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="NPC references")
	ABattleManager *BattleManagerReference;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="NPC references")
	AGoalGate *GoalReference;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="NPC")
	int32 KickRange;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="NPC")
	float SkillCooldown;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="NPC")
	FVector SpawnPosition;
	
private:
	
	// Defence/Balance/Attack positions to get back to, calculated on construct
	UPROPERTY()
	FVector AttackPosition;
	UPROPERTY()
	FVector DefencePosition;
	UPROPERTY()
	FVector BalancePosition;
	
public:
	// Sets default values for this character's properties
	AFootballCharacter();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Init(int32 in_KickRange, ABattleManager* BM);

	/** State management functions */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PassBall(bool IsInitiatingPass, bool IsSuccessful);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LoseBall();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReceiveBall();

	/** Triggers an event to kick the ball into the goal */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void KickBallInGoal(const bool CheckIfGoalIsValid = true);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CheckIfKickBallInGoal();

	/** Passes the ball if it's in our possession onto the "ideal pass mate" */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PassBallToIdealPassMate();

	/** Manages states and ball visibility based on if we have the ball or not
	 * @param HasBall Used to indicate whether or not the pawn has acquired possession of the ball
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HasBallInPossession(bool HasBall);

	// Not really, just setting default names to the abilities
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DefaultAbilityFactory();
	
	/** Sets the visibility of the ball sphere, intended for debugging */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetVisibilityBall(bool Visible);

	/** Calculates the target positions for the pawns when they're controlled by the balance/attack/defense buttons
	 */
	UFUNCTION(BlueprintCallable)
	void CalculateFieldPositions();

	/** Manages the change of states, stores the last state as well */
	UFUNCTION(BlueprintCallable)
	void ChangePlayerState(const EPStates NewPlayerState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ChangedState();

	/** Verifies the current state of the pawn based on parameters (hasBall, current ball owner, etc.)*/
	UFUNCTION(BlueprintCallable)
	void CheckPlayerState();

	/** Restores the last state after the BT switch */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RestoreStateFromBT();
	
	/** Subtracts the value out of the stamina of the character */
	UFUNCTION(BlueprintCallable)
	void SubtractStamina(const int32 Stamina);

	/** Adds the value out of the stamina of the character */
	UFUNCTION(BlueprintCallable)
	void AddStamina(const int32 Stamina);

	/** Recalculate the stamina percent based on left points and starter stats */
	UFUNCTION(BlueprintCallable)
	void CalculateStaminaPercent();

	UFUNCTION(BlueprintCallable)
	AFootballCharacter* SeekEnemyC(const float DistanceNearThreshold);

	/** Returns the current state of the player pawn */
	UFUNCTION(BlueprintCallable)
	EPStates GetState() const;
	
	/** Returns the previous state of the player pawn */
	UFUNCTION(BlueprintCallable)
	EPStates GetLastState() const;

	UFUNCTION(BlueprintCallable)
	FName GetTeamTag() const;

	UFUNCTION(BlueprintCallable)
	int32 GetTeamID() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsTeam1() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsSameTeamAs(AFootballCharacter* P2) const;
	
	/** Pass in a EPlayerRarity enum to set the card's rarity
	 * @param Rarity	The new rarity of the card.
	 */
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerRarity(EPlayerRarity Rarity);

	/** Static check whether the player has enough stamina to execute the skill */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool HasStaminaForSkill(AFootballCharacter* Character, const FPlayerAbility Ability);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool HasStaminaForSkillSimple(const int32 CurrentStamina, const int32 StaminaCost);
	// Events 

	/** State management function */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnemyHasBall();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnemyGoalieHasBall();

	/** State management function */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BallIsInGoalState();

	/** State management function */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TeammateHasBall();
	
	/** State management function */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TeammateHasBallBasedOnTag();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool ActivateCooldown(const FString &Message = FString(TEXT("")));
	
	UFUNCTION(BlueprintCallable)
	FVector GetAttackPosition() const
	{
		return AttackPosition;
	}

	UFUNCTION(BlueprintCallable)
	FVector GetDefencePosition() const
	{
		return DefencePosition;
	}

	UFUNCTION(BlueprintCallable)
	FVector GetBalancePosition() const
	{
		return BalancePosition;
	}
	
protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
