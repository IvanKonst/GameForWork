// Fill out your copyright notice in the Description page of Project Settings.


// ReSharper disable CppJoinDeclarationAndAssignment
#include "FootballCharacter.h"
#include "GoalGate.h"
#include "BattleManager.h"


AFootballCharacter::AFootballCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// Default values
	bCanMove = false;
	bHasBall = false;
	IsInCooldown = false;
	
	KickRange = 700;
	SkillCooldown = 1.0f;
	PlayerProperties.CurrentStaminaPoints = PlayerProperties.CoreStats.Stamina;
	PlayerProperties.StaminaPercent = 100.f;
	
	ChangePlayerState(EPStates::isBallFree);

	// Adding default tags on creation
	Tags.Reserve(2);
	Tags.Push("Team1Tag");
	Tags.Push("RadarTag");
}

void AFootballCharacter::KickBallInGoal_Implementation(const bool CheckIfGoalIsValid)
{
	if(BattleManagerReference)
	{
		const bool SuccessfulGoal = CheckIfKickBallInGoal();
		if(SuccessfulGoal)
		{
			ChangePlayerState(EPStates::isBallInGoal);
		}
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("BattleManager reference is invalid when attempting to score a goal."));
	}
	
	// bool SuccessfulGoal = true;
	//
	// if(CheckIfGoalIsValid)
	// {
	// 	SuccessfulGoal = CheckIfKickBallInGoal();
	// }
	//
	// if(BattleManagerReference)
	// {
	//
	// 	PassBall(true, SuccessfulGoal);
	// 	if(SuccessfulGoal)
	// 	{
	// 		BattleManagerReference->ActorPossessingBall = GoalReference;
	// 		BattleManagerReference->CallBallIsInGoalStates();
	// 	}
	// }
}

void AFootballCharacter::ReceiveBall_Implementation()
{
	bHasBall = true;
	SetVisibilityBall(bHasBall);
	ChangePlayerState(EPStates::possessesBall);
}

void AFootballCharacter::LoseBall_Implementation()
{
	bHasBall = false;
	SetVisibilityBall(bHasBall);
	ChangePlayerState(EPStates::enemyHasBall);
}

void AFootballCharacter::OnConstruction(const FTransform& Transform)
{
	if(PlayerStates == EPStates::possessesBall)
	{
		bHasBall = true;
		SetVisibilityBall(true);
		BattleManagerReference->ActorPossessingBall = this;
	} else {
		bHasBall = false;
		SetVisibilityBall(false);
	}
}

void AFootballCharacter::PassBall_Implementation(bool IsInitiatingBall, bool IsSuccessful)
{
	if(IsInitiatingBall)
	{
		bHasBall = false;
		SetVisibilityBall(bHasBall);
	}
	if(!IsInitiatingBall && IsSuccessful)
	{
		bHasBall = true;
		SetVisibilityBall(bHasBall);
	}
	
	if(IsInitiatingBall == true)
	{
		if(IsSuccessful)
		{
			ChangePlayerState(EPStates::teammateHasBall);
		} else
		{
			ChangePlayerState(EPStates::enemyHasBall);
		}
	}

	if(IsInitiatingBall == false)
	{
		if(IsSuccessful)
		{
			ChangePlayerState(EPStates::possessesBall);
		} else 
		{
			ChangePlayerState(EPStates::enemyHasBall);
		}
	}
}

void AFootballCharacter::Init_Implementation(int32 in_KickRange, ABattleManager* BM)
{
}

bool AFootballCharacter::CheckIfKickBallInGoal_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Use of checkifkickballin goal, returned automatically true"));
	return BattleManagerReference->PassBallFromPwnToGoal(GoalReference);
}

void AFootballCharacter::PassBallToIdealPassMate_Implementation()
{
	if(BattleManagerReference && IdealPassMateReference) 
	{
		BattleManagerReference->PassBallFromPwnToPwn(this, IdealPassMateReference);
		TeammateHasBallBasedOnTag();
	} else {
		// If Battle manager or PassMate reference are invalid print the following
		UE_LOG(LogTemp, Error, TEXT("PassBallToIdealPassMate: BM or IdealPassMate pointer invalid"))
	}
}

void AFootballCharacter::SetVisibilityBall_Implementation(bool Visible)
{
	
}

void AFootballCharacter::HasBallInPossession_Implementation(bool HasBall_in)
{
	if(HasBall_in) {
		ChangePlayerState(EPStates::possessesBall);
		BattleManagerReference->ActorPossessingBall = this;
		SetVisibilityBall(true);
	} else {
		SetVisibilityBall(false);
	}
}

void AFootballCharacter::EnemyHasBall_Implementation()
{
	ChangePlayerState(EPStates::enemyHasBall);
	SetVisibilityBall(false);
}

void AFootballCharacter::BallIsInGoalState_Implementation()
{
	ChangePlayerState(EPStates::isBallInGoal);
	SetVisibilityBall(false);
}

void AFootballCharacter::TeammateHasBall_Implementation()
{
	ChangePlayerState(EPStates::teammateHasBall);
	SetVisibilityBall(false);
}

void AFootballCharacter::TeammateHasBallBasedOnTag_Implementation()
{
	if(BattleManagerReference)
	{
		if(IsTeam1())
		{
			BattleManagerReference->CallT1HasBall();
		} else
		{
			BattleManagerReference->CallT2HasBall();
		}
	}
}

void AFootballCharacter::DefaultAbilityFactory_Implementation()
{
	PlayerProperties.PlayerAbilities[0].SetName(ConfigVars::DefaultKickAb);
	PlayerProperties.PlayerAbilities[1].SetName(ConfigVars::DefaultDribble);
	PlayerProperties.PlayerAbilities[2].SetName(ConfigVars::DefaultJumpKick);
	PlayerProperties.PlayerAbilities[3].SetName(ConfigVars::DefaultSpecialKick);
}

void AFootballCharacter::CalculateFieldPositions()
{
	SpawnPosition = GetActorLocation();
	
	AttackPosition = SpawnPosition;
	BalancePosition = SpawnPosition;
	DefencePosition = SpawnPosition;
	
	if(PlayerProperties.PlayerRole != EPlayerRole::goalkeeper && GoalReference)
	{
		const float GoalPosX = GoalReference->GetActorLocation().X;

		if(GoalPosX > SpawnPosition.X)
		{
			AttackPosition.X  += ConfigVars::AttackOffset;
			BalancePosition.X += ConfigVars::BalanceOffset;
		} else
		{
			AttackPosition.X  -= ConfigVars::AttackOffset;
			BalancePosition.X -= ConfigVars::BalanceOffset;
			
		}
	}
}

void AFootballCharacter::ChangePlayerState(const EPStates NewPlayerState)
{
	LastPlayerState = PlayerStates;
	PlayerStates = NewPlayerState;
	bHasBall = (NewPlayerState == EPStates::possessesBall);
	// ChangedState();
	ChangedStateDelegate.Broadcast();
}

void AFootballCharacter::ChangedState_Implementation()
{
}

void AFootballCharacter::CheckPlayerState()
{
	const bool& T1HasBall = BattleManagerReference->bT1HasBall;
	if(T1HasBall)
	{
		if(IsTeam1())
		{
			 ChangePlayerState((bHasBall ? EPStates::possessesBall : EPStates::teammateHasBall));
		} else
		{
			ChangePlayerState(EPStates::enemyHasBall);
		}
	} else
	{
		if (!IsTeam1())
		{
			ChangePlayerState((bHasBall ? EPStates::possessesBall : EPStates::teammateHasBall));
		}
		else
		{
			ChangePlayerState(EPStates::enemyHasBall);
		}
	}
}

void AFootballCharacter::RestoreStateFromBT_Implementation()
{
	CheckPlayerState();
}

void AFootballCharacter::SubtractStamina(const int32 Stamina)
{
	int32 &StamPts = PlayerProperties.CurrentStaminaPoints;
	StamPts -= Stamina;
	if(StamPts < 0)
	{
		StamPts = 0;
	}
	CalculateStaminaPercent();
}

void AFootballCharacter::AddStamina(const int32 Stamina)
{
	PlayerProperties.CurrentStaminaPoints += Stamina;
	CalculateStaminaPercent();
}

void AFootballCharacter::CalculateStaminaPercent()
{
	auto &Props = PlayerProperties;
	auto &Stats = PlayerProperties.CoreStats;
	Props.StaminaPercent = ( static_cast<float>(Props.CurrentStaminaPoints) / static_cast<float>(Stats.Stamina) ) * 100.f;
}

AFootballCharacter* AFootballCharacter::SeekEnemyC(const float DistanceNearThreshold)
{
	FVector ActorPossessingLoc;
	AFootballCharacter* Possessor = Cast<AFootballCharacter>(BattleManagerReference->ActorPossessingBall);
	AFootballCharacter* NearestEnemy = nullptr;
	
	if(Possessor != nullptr)
	{
		ActorPossessingLoc = Possessor->GetActorLocation();

		TArray<AFootballCharacter*> PWNArray;
		if(IsTeam1())
		{
			PWNArray = BattleManagerReference->T2PWNArray;
		} else
		{
			PWNArray = BattleManagerReference->T1PWNArray;
		}

		float Distance;
		float NearestDistance = 100000.f;
		
		for(auto &PWN: PWNArray)
		{
			 Distance = FVector::Distance(ActorPossessingLoc, PWN->GetActorLocation());
			 if(NearestDistance < Distance && Distance <= DistanceNearThreshold)
			 {
				 NearestEnemy = PWN;
				NearestDistance = Distance;
			 }
		}
	}

	if(NearestEnemy != nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't find an enemy: SeekEnemyC"))
	}
	
	return NearestEnemy;
}

EPStates AFootballCharacter::GetState() const
{
	return PlayerStates;
}

EPStates AFootballCharacter::GetLastState() const
{
	return LastPlayerState;
} 

FName AFootballCharacter::GetTeamTag() const
{
	return (IsTeam1() ? ConfigVars::Team1Tag : ConfigVars::Team2Tag);
}

int32 AFootballCharacter::GetTeamID() const
{
	if(IsTeam1())
	{
		return 1;
	} else
	{
		return 2;
	}
}

/** Returns true if the character is from team 1 */
bool AFootballCharacter::IsTeam1() const
{
	return ActorHasTag(ConfigVars::Team1Tag);
}
/** P1 - Self, Player Two - P2 */
bool AFootballCharacter::IsSameTeamAs(AFootballCharacter* P2) const
{
	return GetTeamTag() == P2->GetTeamTag();
}

void AFootballCharacter::UpdatePlayerRarity(EPlayerRarity Rarity)
{
	PlayerProperties.PlayerRarity = Rarity;
}

bool AFootballCharacter::HasStaminaForSkill(AFootballCharacter* Character, const FPlayerAbility Ability)
{
	const int32 &StaminaCost = Ability.StaminaCost;
	const int32 &CurrentStamina = Character->PlayerProperties.CurrentStaminaPoints;

	return ((CurrentStamina - StaminaCost) < 0);
}

bool AFootballCharacter::HasStaminaForSkillSimple(const int32 CurrentStamina, const int32 StaminaCost)
{
	return ((CurrentStamina - StaminaCost) < 0);
}

void AFootballCharacter::EnemyGoalieHasBall_Implementation()
{
	EnemyHasBall();
}

bool AFootballCharacter::ActivateCooldown_Implementation(const FString &Message)
{
	bool activateTimer = false;
	if(IsInCooldown == false)
	{
		IsInCooldown = true;
		activateTimer = true;
	}
	
 // Do not print in Shipping or Test
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	// Print out a notification in the console with the PWNs name
	if(Message == FString(TEXT("")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Activated cooldown."))
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Activated cooldown: %s."), *Message)
	}
#endif

	return activateTimer;
}

void AFootballCharacter::BeginPlay()
{
	Super::BeginPlay();

	CalculateFieldPositions();
}
//Unused
// void AFootballCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);
// }
