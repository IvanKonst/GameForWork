// Fill out your copyright notice in the Description page of Project Settings.


// ReSharper disable CppUE4CodingStandardNamingViolationWarning
// ReSharper disable IdentifierTypo
#include "BPFunctionLibrary.h"
#include "FootballCharacter.h"
#include "GoalGate.h"
#include "Structures.h"

FVector2D UBPFunctionLibrary::Translate3DTo2DFField(AActor* Actor, TArray<ATargetPoint*> TargetPoints, FVector2D ScreenDimensions)
{
	const bool ValidPoints = (TargetPoints.IsValidIndex(0) && TargetPoints.IsValidIndex(1));

	const FVector ActorLocation = Actor->GetActorLocation();
	const FVector Point1Loc = TargetPoints[0]->GetActorLocation();
	const FVector Point2Loc = TargetPoints[1]->GetActorLocation();

	if (ValidPoints) {
		const float X = FMath::GetMappedRangeValueClamped(FVector2D(Point2Loc.X, Point1Loc.X), FVector2D(0.f, ScreenDimensions.X), ActorLocation.X);
		const float Y = FMath::GetMappedRangeValueClamped(FVector2D(Point1Loc.Y, Point2Loc.Y), FVector2D(0.f, ScreenDimensions.Y), ActorLocation.Y);

		return FVector2D(X, Y);
	}
	else {
		return FVector2D(0.f, 0.f);
	}
}

bool UBPFunctionLibrary::IsTeam1(const FName TeamTag) 
{
	if(TeamTag == ConfigVars::Team1Tag)
	{
		return true;
	} else
	{
		return false;
	}
}

int32 UBPFunctionLibrary::GetTeamID(const FName TeamTag)
{
	if(TeamTag == ConfigVars::Team1Tag)
	{
		return 1;
	} else
	{
		return 2;
	}
}

FVector UBPFunctionLibrary::ClampLoc(AFootballCharacter* PWN, const FVector InLocation, const float GKRange,
	const float FWRange, const float MFRange, const float DFRange, const float YClamp, const float FreeRange, const float ExtendedRange, const float BackRange)
{
	FVector LocOut = InLocation;
	
	float MinDistance = 0.f;
	float NewX = LocOut.X;
	float NewY = LocOut.Y;
	
	if(PWN)
	{
		EPlayerRole &PlayerRole = PWN->PlayerProperties.PlayerRole;
		switch (PlayerRole)
		{
		case EPlayerRole::forward:
			MinDistance = FWRange;
			// MinDistance = ConfigVars::MinDistanceFW;
			break;
		case EPlayerRole::midfield:
			MinDistance = MFRange;
			// MinDistance = ConfigVars::MinDistanceMF;
			break;
		case EPlayerRole::defender:
			MinDistance = DFRange;
			// MinDistance = ConfigVars::MinDistanceDF;
			break;
		case EPlayerRole::goalkeeper:
			MinDistance = GKRange;
			// MinDistance = ConfigVars::MinDistanceGK;
			break;
		}
	}

	if(PWN->GoalReference && PWN)
	{
		const auto &EnemyGoal = PWN->GoalReference;
		const EPlayerRole &PlayerRole = PWN->PlayerProperties.PlayerRole;
		
		const FVector PWNLoc = PWN->GetActorLocation();
		const FVector EnemyGoalLoc = EnemyGoal->GetActorLocation();

		// Distance from the target loc to the goal
		const float DistXInLocToGoal = FMath::Abs(InLocation.X - EnemyGoalLoc.X);

		// X clamping -------------
		// Limit the target location to the allowed range on the X axis.

		// X clamping forward
		if(DistXInLocToGoal < MinDistance)
		{
			if(EnemyGoalLoc.X < PWNLoc.X)
			{
				 NewX = EnemyGoalLoc.X + MinDistance;
			} else
			{
				 NewX = EnemyGoalLoc.X - MinDistance;
			}
		}
		
		// X clamping backwards
		//

		float SpawnPosX;
		switch(PWN->OffenseState)
		{
			case EAttackState::attack:
				SpawnPosX = PWN->GetAttackPosition().X;
				break;
			case EAttackState::balance:
				SpawnPosX = PWN->GetBalancePosition().X;
				break;
			case EAttackState::defense:
				SpawnPosX = PWN->GetDefencePosition().X;
				break;
			default:
				SpawnPosX = PWN->SpawnPosition.X;
				break;
		}
		const float DistPWNtoGoal = FMath::Abs(SpawnPosX - EnemyGoalLoc.X);
		
		const bool IsTooFarBackwards = (DistXInLocToGoal > DistPWNtoGoal + BackRange);
		
		if(IsTooFarBackwards)
		{
			if(EnemyGoalLoc.X < PWNLoc.X)
			{
				NewX = SpawnPosX + BackRange;
			} else
			{
				NewX = SpawnPosX - BackRange;
			}
		}
		

		// Y clamping -------------

		// Clamp the end Y location to their designated corridor defined by their spawn position and YClamp offset
		// @param FreeRange : Distance between the player and the goal that is considered to be a free range
		// ( a range where the player can move more on the Y axis )
		if (DistXInLocToGoal < FreeRange)
		{
			NewY = TrimRange(PWN->SpawnPosition.Y + (YClamp + ExtendedRange),
			                 PWN->SpawnPosition.Y - (YClamp + ExtendedRange), NewY);
			// UE_LOG(LogTemp, Warning, TEXT("Clamping Y to extended range."))
		}
		else 
		{
			NewY = TrimRange(PWN->SpawnPosition.Y + YClamp, PWN->SpawnPosition.Y - YClamp, NewY);
			// UE_LOG(LogTemp, Warning, TEXT("Clamping Y to limited range. (not close enough to the goal)"))
		}
	}
		
	LocOut.X = NewX;
	LocOut.Y = NewY;
	
	return LocOut;
}

float UBPFunctionLibrary::TrimRange(const float MaxR, const float MinR, const float Input)
{
	if(Input >= MaxR)
	{
		return MaxR;
	}
	if(Input <= MinR)
	{
		return MinR;
	}

	return Input;
}

