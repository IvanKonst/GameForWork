// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/TargetPoint.h"
#include "Containers/Array.h"
#include "Kismet/KismetMathLibrary.h"
#include "BPFunctionLibrary.generated.h"

/**
 * 
 */
 
class AFootballCharacter;
 
UCLASS()
class AIPROJECT_API UBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Math")
	static FVector2D Translate3DTo2DFField(AActor* Actor, TArray<ATargetPoint*> TargetPoints, FVector2D ScreenDimensions);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool IsTeam1(const FName TeamTag);

	// Will return 1 for team1 and 2 for team 2
	// @return 1 or 2 for team ID
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static int32 GetTeamID(const FName TeamTag);

	// Clamp location according to their role
	// @param PWN			The PWN which is moving to the end location, needed for the goal reference, team and role.
	// @param InLocation	The location which is taken in to be clamped.
	// @param FreeRange		The range between the end location and the goal gate for which to be allowed extended Y clamping.
	// @param ExtendedRange The amount of which the Y range will be extended when in the FreeRange zone.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FVector ClampLoc(AFootballCharacter* PWN, const FVector InLocation,
		const float GKRange = 8500.f,
		const float FWRange = 0.f,
		const float MFRange = 5500.f,
		const float DFRange = 7000.f,
		const float YClamp = 3000.f,
		const float FreeRange = 3000.f,
		const float ExtendedRange = 1000.0f,
		const float BackRange = 1000.f);

	// Returns the a clamped value in a range
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static float TrimRange(const float MaxR, const float MinR, const float Input);
};
