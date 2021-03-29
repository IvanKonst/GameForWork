// Fill out your copyright notice in the Description page of Project Settings.


// ReSharper disable CppJoinDeclarationAndAssignment
// ReSharper disable IdentifierTypo
#include "BattleManager.h"
#include "FootballCharacter.h"
#include "GoalGate.h"
#include "Utilities/BPFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ABattleManager::ABattleManager()
{
	PrimaryActorTick.bCanEverTick = false;

	TagArrayTeam1.Reserve(2);
	TagArrayTeam1.Push(ConfigVars::Team1Tag);
	TagArrayTeam1.Push(ConfigVars::RadarTag);
	
	TagArrayTeam2.Reserve(2);
	TagArrayTeam2.Push(ConfigVars::Team2Tag);
	TagArrayTeam2.Push(ConfigVars::RadarTag);

	const uint8 TeamSize = 11;
	T1PWNArray.Reserve(TeamSize);
	T2PWNArray.Reserve(TeamSize);
}

void ABattleManager::PassBallToClosestTeammate_Implementation(AFootballCharacter* Initiator, const bool PassBehind, const float Range)
{
	// Used to calculate the "forward" x vector values and correct array
	const bool PTeam = Initiator->IsTeam1();
	// New ball possessor reference
	AFootballCharacter* TeammateToPass = nullptr;
	// Determine the player array
	const auto &PWNArr = (PTeam ? T1PWNArray : T2PWNArray);

	// Get the location of the initiator to compare later to
	const FVector PlayerLocation = Initiator->GetActorLocation();
	float MinDistance = 10000.f;
	float CurrentDistance;
	
	for(int i = 0; i < PWNArr.Num(); i++)
	{
		if(!PWNArr.IsValidIndex(i))
		{
			continue;
		}
		
		const float PWNXLoc = PWNArr[i]->GetActorLocation().X;
		CurrentDistance = FMath::Abs(PWNXLoc - PlayerLocation.X);
		
		// Check if the player is behind & if it's allowed to be
		const bool IsToBeConsidered = (PTeam ? (PWNXLoc > PlayerLocation.X || PassBehind) : (PWNXLoc < PlayerLocation.X || PassBehind) );
		
		if (IsToBeConsidered)
		{
			if (CurrentDistance < MinDistance && CurrentDistance <= Range && Initiator != PWNArr[i])
			{
				TeammateToPass = PWNArr[i];
				MinDistance = CurrentDistance;
			}
		}
	} // End of distance checking loop
		

	// Pass the ball to another teammate if there is one found within the criteria, has the ball and is not in cooldown
	if(TeammateToPass && Initiator->bHasBall && !Initiator->IsInCooldown)
	{
		PassBallFromPwnToPwn(Initiator, TeammateToPass);
	} else if(TeammateToPass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find a teammate to pass to."))
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cooldown or doesn't have the ball."))
	}
}

void ABattleManager::UpdateBallPossessor_Implementation(AActor* NewPossessor)
{
	LastActorPossessingBall = ActorPossessingBall;
	ActorPossessingBall = NewPossessor;
}

void ABattleManager::UpdateScore_Implementation(const int32 TeamIDScore)
{
}

void ABattleManager::SetupStartingPawnWithBall_Implementation(AFootballCharacter* NewPossessor)
{
	NewPossessor->ReceiveBall();
	UpdateBallPossessor(NewPossessor);
	
	if (NewPossessor->IsTeam1()) {
		CallT1HasBall();
	} else
	{
		CallT2HasBall();
	}
}

void ABattleManager::SetAttackState_Implementation(const EAttackState NewAttackState)
{
	for (int i = 0; i < T1PWNArray.Num(); i++)
	{
		T1PWNArray[i]->OffenseState = NewAttackState;
	}
}

void ABattleManager::DetermineStates_Implementation()
{
	if(ActorPossessingBall)
	{
		if(ActorPossessingBall->ActorHasTag(ConfigVars::Team1Tag))
		{
			CallT1HasBall();
		} else if(ActorPossessingBall->ActorHasTag(ConfigVars::Team2Tag))
		{
			CallT2HasBall();
		} else
		{
			CallBallIsInGoalStates();
		}
	}
}

/** If the attack is successful: return true; If it is not: return false
 * Calls for player state change
 */
bool ABattleManager::AttackBallPossessor_Implementation(AFootballCharacter* Initiator,
	AFootballCharacter* BallPossessor)
{
	const bool bSuccessfulAttack = GetOutcomeAttack(Initiator->PlayerProperties.CoreStats.Attack, BallPossessor->PlayerProperties.CoreStats.Defence);

	if(bSuccessfulAttack)
	{
		Initiator->ReceiveBall();
		BallPossessor->LoseBall();
		UpdateBallPossessor(Initiator);
		
		if(Initiator->IsTeam1())
		{
			CallT1HasBall();
		} else
		{
			CallT2HasBall();
		}
		LastPWNPossessor = BallPossessor;
		
	} else
	{
		// Not needed but for redundancy(?)
		BallPossessor->ReceiveBall();
		Initiator->LoseBall();
		UpdateBallPossessor(BallPossessor);
		
		if(BallPossessor->IsTeam1())
		{
			CallT1HasBall();
		} else
		{
			CallT2HasBall();
		}
	}

	return bSuccessfulAttack;
}

void ABattleManager::CallBallIsInGoalStates_Implementation()
{
	for (int i = 0; i < T1PWNArray.Num(); i++)
	{
		if(T1PWNArray.IsValidIndex(i))
			T1PWNArray[i]->BallIsInGoalState();	
	}
	for (int i = 0; i < T2PWNArray.Num(); i++)
	{
		if(T2PWNArray.IsValidIndex(i))
			T2PWNArray[i]->BallIsInGoalState();	
	}
	bBallIsInGoal = true;
}

void ABattleManager::CallT1HasBall_Implementation()
{
	for (int i = 0; i < T1PWNArray.Num(); i++)
	{
		if(!T1PWNArray[i]->bHasBall)
			T1PWNArray[i]->TeammateHasBall();
	}
	for (int i = 0; i < T2PWNArray.Num(); i++)
	{
		T2PWNArray[i]->EnemyHasBall();
	}

	bT1HasBall = true;
}

void ABattleManager::CallT2HasBall_Implementation()
{
	for (int i = 0; i < T2PWNArray.Num(); i++)
	{
		if(!T2PWNArray[i]->bHasBall)
		{
			T2PWNArray[i]->TeammateHasBall();
		}
	}
	for (int i = 0; i < T1PWNArray.Num(); i++)
	{
		T1PWNArray[i]->EnemyHasBall();
	}
	bT1HasBall = false;
}

AFootballCharacter* ABattleManager::GetClosestTeammate(AFootballCharacter* Player)
{
	float MinDistanceBetweenActors = 10000.f;
	float CurrentDistance;
	AFootballCharacter* ClosestPlayer = nullptr;

	const bool IsT1 = Player->IsTeam1();
	auto& PWNArr = T1PWNArray;
	// Quick array check for the team array
	if(!IsT1){
		PWNArr = T2PWNArray;
	}

	for(int i = 0; i < PWNArr.Num(); i++)
	{
		CurrentDistance = Player->GetDistanceTo(PWNArr[i]);
		if(CurrentDistance < MinDistanceBetweenActors && Player != PWNArr[i])
		{
			MinDistanceBetweenActors = CurrentDistance;
			ClosestPlayer = PWNArr[i];
		}
	}
	return ClosestPlayer;
}

int32 ABattleManager::GetOutcomePlayerIDFromStat(const float P1Stat, const float P2Stat, const float BiasMultiplier, const float HardCap) const
{
	const float Difference = P1Stat - P2Stat;

	const float RandNum = FMath::RandRange(0.f, 1000.f);
	
	if(Difference > HardCap)
	{
		return 1;
	}
	if (Difference < -HardCap)
	{
		return 2;
	}

	if(RandNum > Difference * BiasMultiplier + 500)
	{
		return 2;
	} else {
		return 1;
	}
}

bool ABattleManager::GetOutcomeAttack(const float P1StatAttack, const float P2StatDefend, const bool UseDefaultMultiplierValues, const float BiasMult,
	const float HardCap) const
{
	int32 Result;
	if(UseDefaultMultiplierValues)
	{
		Result = GetOutcomePlayerIDFromStat(P1StatAttack, P2StatDefend);
	} else
	{
		Result = GetOutcomePlayerIDFromStat(P1StatAttack, P2StatDefend, BiasMult, HardCap);
	}
	
	return Result == 1 ? true : false;
}

int32 ABattleManager::GetOutcomeDefend(const float P1StatDefend, const float P2StatAttack, const bool UseDefaultMultiplierValues, const float BiasMult,
	const float HardCap) const
{
	int32 Result;
	if(UseDefaultMultiplierValues)
	{
		Result = GetOutcomePlayerIDFromStat(P1StatDefend, P2StatAttack);
	} else
	{
		Result = GetOutcomePlayerIDFromStat(P1StatDefend, P2StatAttack, BiasMult, HardCap);
	}
	
	return Result;
}

int32 ABattleManager::GetOutcomeScoreGoal(const float P1StatAttack, const float P2StatDefend, const bool UseDefaultMultiplierValues, const float BiasMult, const float HardCap) const
{
	int32 Result;
	if(UseDefaultMultiplierValues)
	{
		Result = 0;
	} else
	{
		Result = 0;
	}
	
	return Result;
}

int32 ABattleManager::GetWinningPlayerIDFromBasicEncounter_Implementation(AFootballCharacter* T1Player,
                                                                          AFootballCharacter* T2Player)
{
	const float Multiplier = 1.f;
	const float Attack1 = T1Player->PlayerProperties.CoreStats.Attack;
	const float Attack2 = T1Player->PlayerProperties.CoreStats.Attack;
	return GetOutcomePlayerIDFromStat(Attack1, Attack2, Multiplier);
}
// Does a check whether the score goal is successful and sets up the score and ownership of the ball, the last player needs to do it in their own function
bool ABattleManager::PassBallFromPwnToGoal_Implementation(AGoalGate *Goal)
{
	const bool bSuccessfullyScoredGoal = true;
	
	if(bSuccessfullyScoredGoal)
	{
		Goal->ReceiveBall();
		if(Goal->ActorHasTag(FName("TargetGateForT1")))
		{
			UpdateScore(1);
		} else
		{
			UpdateScore(2);
		}
		UpdateBallPossessor(Goal);
		bBallIsInGoal = true;
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("PassBallFromPWNToGoal: Failed to successfuly score a goal."));
	}
	
	return bSuccessfullyScoredGoal;
}

// Considering the pass is successful
// @param Initiator : Actor passing the ball
// @param IdealPassMate : Actor getting the ball
bool ABattleManager::PassBallFromPwnToPwn_Implementation(AFootballCharacter* InitiatorActor, AFootballCharacter* ReceiverActor)
{
	// TODO: Implement the method for line trace block check and proximity
	const bool bBallBlockedFromEnemy = false;

	if(!bBallBlockedFromEnemy)
	{
		if(InitiatorActor && ReceiverActor )
		{
			ReceiverActor->PassBall(false, true);
			InitiatorActor->PassBall(true, true);
			ReceiverActor->IdealPassMateReference = NULL;
			InitiatorActor->IdealPassMateReference = NULL;
			
			UpdateBallPossessor(ReceiverActor);
		} else {
			UE_LOG(LogTemp, Error, TEXT("PassBallFromActorToActor: invalid pointer"))
		}
	}
	
	return !bBallBlockedFromEnemy;
}

void ABattleManager::BeginPlay()
{
	Super::BeginPlay();
}

void ABattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}