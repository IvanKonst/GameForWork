// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSequence/Public/LevelSequence.h"
#include "Structures.h"
#include "BattleManager.generated.h"


class AGoalGate;
class AFootballCharacter;

UCLASS()
class AIPROJECT_API ABattleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Methods
	ABattleManager();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateBallPossessor(AActor* NewPossessor);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool PassBallFromPwnToGoal(AGoalGate *Goal);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PassBallToClosestTeammate(AFootballCharacter* Initiator, const bool PassBehind = true, const float Range = 10000.f);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool PassBallFromPwnToPwn(AFootballCharacter *Initiator, AFootballCharacter *IdealPassMate);

	// @return Returns whether the attack was successful or not
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool AttackBallPossessor(AFootballCharacter *Initiator, AFootballCharacter *BallPossessor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetupStartingPawnWithBall(AFootballCharacter* NewPossessor);

	// Call this to increment the score, 1 for team 1 and 2 for team 2
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateScore(const int32 TeamIDScore);
	
	// Battle events
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetAttackState(const EAttackState NewAttackState);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DetermineStates();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CallBallIsInGoalStates();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CallT1HasBall();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CallT2HasBall();

	UFUNCTION(BlueprintCallable)
	AFootballCharacter* GetClosestTeammate(AFootballCharacter* Player);
	
	
	// Return 1 or 2 (int32) regarding to the player stat
	UFUNCTION(BlueprintCallable)
	int32 GetOutcomePlayerIDFromStat(const float P1Stat, const float P2Stat, const float BiasMultiplier = 1.0f, const float HardCap = 300.0f) const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetWinningPlayerIDFromBasicEncounter(AFootballCharacter* T1Player,AFootballCharacter* T2Player);

	/** 
	 * @return Returns true if the stat of player 1 wins, false if player two stat wins
	 */
	UFUNCTION(BlueprintCallable)
	bool GetOutcomeAttack(const float P1StatAttack, const float P2StatDefend, const bool UseDefaultMultiplierValues = true, const float BiasMult = 1.0f, const float HardCap = 300.0f) const;

	// Return 1 or 2 (int32) regarding to the player stat [const]
	UFUNCTION(BlueprintCallable)
	int32 GetOutcomeDefend(const float P1StatDefend, const float P2StatAttack, const bool UseDefaultMultiplierValues = true, const float BiasMult = 1.0f, const float HardCap = 300.0f) const;

	UFUNCTION(BlueprintCallable)
	int32 GetOutcomeScoreGoal(const float P1StatAttack, const float P2StatDefend, const bool UseDefaultMultiplierValues = true, const float BiasMult = 1.0f, const float HardCap = 300.0f) const;
	
	// Variables
public:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Properties")
	TArray<FName> TagArrayTeam1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player Properties")
	TArray<FName> TagArrayTeam2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Players")
	TArray<AFootballCharacter*> T1PWNArray;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Players")
	TArray<AFootballCharacter*> T2PWNArray;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Players")
	AActor* ActorPossessingBall;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Players")
	AActor* LastActorPossessingBall;

	// Intended to store references from BTT_AttackPossessor to be handed off to TriggerSequence
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Players")
	AFootballCharacter* PWNAttacking;

	// Intended to store references from BTT_AttackPossessor to be handed off to TriggerSequence
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Players")
	AFootballCharacter* PWNBeingAttacked;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Players")
	AFootballCharacter* LastPWNPossessor;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Properties")
	float DefaultBiasMultiplier = 1.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Properties")
	float DefaultHardCap = 300.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game")
	bool bBallIsInGoal;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game")
	bool bT1HasBall;
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
