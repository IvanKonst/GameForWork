// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreSubsystem.h"


DEFINE_LOG_CATEGORY(LogScoreSystem);

void UScoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	ResetScore();
}

void UScoreSubsystem::ResetScore()
{
	Team1Score = 0;
	Team2Score = 0;
}

int32 UScoreSubsystem::GetTeam1Score() const
{
	return Team1Score;
}

int32 UScoreSubsystem::GetTeam2Score() const
{
	return Team2Score;
}

int32 UScoreSubsystem::GetLastScoreTeamID() const
{
	return LastUpdatedScoreTeamID;
}

void UScoreSubsystem::AddScore(const int32 TeamID)
{
	if(TeamID == 1)
	{
		Team1Score++;
		LastUpdatedScoreTeamID = TeamID;
		
		OnScoreUpdateT1.Broadcast(Team1Score);
		OnScoreUpdate.Broadcast(TeamID);
		
		UE_LOG(LogScoreSystem, Warning, TEXT("T1Score: %d"), Team1Score)
	} else if(TeamID == 2)
	{
		Team2Score++;
		LastUpdatedScoreTeamID = TeamID;
		
		OnScoreUpdateT2.Broadcast(Team2Score);
		OnScoreUpdate.Broadcast(TeamID);
		
		UE_LOG(LogScoreSystem, Warning, TEXT("T2Score: %d"), Team2Score)
	}
}
