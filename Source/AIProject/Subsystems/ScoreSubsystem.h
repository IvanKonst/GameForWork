// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ScoreSubsystem.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoredTeamDelegate, int32, TeamID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreDelegate, int32, NewScore);

DECLARE_LOG_CATEGORY_EXTERN(LogScoreSystem, Log, All);

UCLASS()
class AIPROJECT_API UScoreSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// virtual void Deinitialize() override;

	// Score properties 
	UPROPERTY()
	int32 Team1Score;
	
	UPROPERTY()
	int32 Team2Score;
	
	UPROPERTY()
	int32 LastUpdatedScoreTeamID;

public:
	UFUNCTION(BlueprintCallable)
	void ResetScore();
	
	UFUNCTION(BlueprintCallable)
	int32 GetTeam1Score() const;
	
	UFUNCTION(BlueprintCallable)
	int32 GetTeam2Score() const;

	UFUNCTION(BlueprintCallable)
	int32 GetLastScoreTeamID() const;
	
	UFUNCTION(BlueprintCallable)
	void AddScore(const int32 TeamID);

	UPROPERTY(BlueprintAssignable, Category="Dispatchers")
	FScoredTeamDelegate OnScoreUpdate;
	
	UPROPERTY(BlueprintAssignable, Category="Dispatchers")
	FScoreDelegate OnScoreUpdateT1;
	
	UPROPERTY(BlueprintAssignable, Category="Dispatchers")
	FScoreDelegate OnScoreUpdateT2;
};
