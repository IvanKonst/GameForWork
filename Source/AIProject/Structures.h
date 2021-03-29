#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"
#include "Structures.generated.h"

class UMaterialInstance;
class UTexture;

UENUM(BlueprintType)
enum class EPlayerCountry : uint8
{
	England,
	Indonesia,
	Nigeria,
	Russia,
	Turkey,
	Argentina,
	India,
	Italy,
	Mexico,
	Uruguay,
	Brazil,
	China,
	France,
	Spain,
	Thailand,
	Bulgaria,
	Germany,
	Japan,
	Portugal,
	Vietnam
};

UENUM(BlueprintType)
enum class EAttackTypes : uint8
{
	Default,
	ScoreGoal,
	PassBall,
	Attack,
	SimpleAttack,
	Defend,
	KeepBall
};

UENUM(BlueprintType)
enum class EAttackState : uint8
{
	defense UMETA(DisplayName = "Defense"),
	balance UMETA(DisplayName = "Balance"),
	attack UMETA(DisplayName = "Attack")
};

UENUM(BlueprintType)
enum class EPStates : uint8 {
	isBallInGoal     UMETA(DisplayName = "IsBallInGoal"),
	possessesBall    UMETA(DisplayName = "PossessesBall"),
	enemyHasBall     UMETA(DisplayName = "EnemyHasBall"),
	teammateHasBall  UMETA(DisplayName = "TeammateHasBall"),
	isBallFree       UMETA(DisplayName = "IsBallFree"),
	playerTurn		 UMETA(DisplayName = "PlayerTurn")
};

UENUM(BlueprintType)
enum class EPlayerRarity : uint8 {
	common        	 UMETA(DisplayName = "Common"),
	super_rare    	 UMETA(DisplayName = "Super Rare"),
	fantastic     	 UMETA(DisplayName = "Fantastic"),
	legendary     	 UMETA(DisplayName = "Legendary")
};

UENUM(BlueprintType)
enum class EPlayerRole : uint8 {
	forward          UMETA(DisplayName = "FW"),
	midfield 		 UMETA(DisplayName = "MF"),
	defender         UMETA(DisplayName = "DF"),
	goalkeeper       UMETA(DisplayName = "GK")
};

USTRUCT(BlueprintType)
struct FPlayerAbility
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	FName AbilityName; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	int32 AbilityLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	EAttackTypes AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	int32 StaminaCost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	int32 IconID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	bool TriggerSequence;

	
	FPlayerAbility(
		const FName Name = FName("DefaultAbName"),
		const int32 Level = 1,
		const int32 Stamina = 1,
		const EAttackTypes Attack = EAttackTypes::Attack,
		const bool ShouldTriggerSequence = false,
		const int32 IconRowID = 0
		)
	{
		AbilityName = Name; 
		AbilityLevel = Level;
		AttackType = Attack;
		StaminaCost = Stamina;
		TriggerSequence = ShouldTriggerSequence;
		IconID = IconRowID;
	}

	void SetName(const FName Name)
	{
		AbilityName = Name;
	}
	void SetLevel(const int32 Level)
	{
		AbilityLevel = Level;
	}
	void SetAttack(const EAttackTypes Attack)
	{
		AttackType = Attack;
	}
	void SetStaminaCost(const int32 Stamina)
	{
		StaminaCost = Stamina;
	}
	void SetTriggerSequence(const bool ShouldTriggerSequence)
	{
		TriggerSequence = ShouldTriggerSequence;
	}
};

USTRUCT(BlueprintType)
struct FPlayerCoreStats
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="Ability")
	float Speed = 100.f;
	
	UPROPERTY(BlueprintReadWrite, Category="Ability")
	float Attack= 100.f;
	
	UPROPERTY(BlueprintReadWrite, Category="Ability")
	float Defence= 100.f;
	
	UPROPERTY(BlueprintReadWrite, Category="Ability")
	float Stamina= 100.f;
};

USTRUCT(BlueprintType)
struct FPlayerProperties 
{
	GENERATED_BODY()

	// Common properties for use in the game
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name = FName("DefaultPlayerName");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerCountry PlayerCountry = EPlayerCountry::Germany;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerRole PlayerRole = EPlayerRole::defender;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPlayerRarity PlayerRarity = EPlayerRarity::common;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerCoreStats CoreStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentStaminaPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaPercent = 100.f;
	
	// Where the player is supposed to be spawned based on ID
	// ID of -1 means he's off the field
	// ID 0-10 corresponds to the position on the field
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerPositionID = -1;

	// IDs for the player picture and frame
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerProfilePictureRow = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerFrameID = -1;
	
	// Player abilities struct, containing all the abilities the character has
	// Should be in an array but UE decided to break those...
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerAbility> PlayerAbilities = {
		FPlayerAbility(),
		FPlayerAbility(),
		FPlayerAbility(),
		FPlayerAbility()
	};
	
	// Puppet values for the sequence montages
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* PlayerMaterialInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* HairMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HairMeshID = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor HairColor = FColor::Black;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor SkinColor = FColor::Black;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkinShade = 0.4f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor ClothesColor = FColor::Black;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor LogoColor = FColor::Black;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* LogoAlpha;
};

USTRUCT(BlueprintType)
struct FGameSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AudioVolume = 0.2f;
	
};

namespace ConfigVars
{
	const float AttackOffset = 1000.f;
	const float BalanceOffset = 300.f;

	const FName RadarTag("RadarTag");
	const FName Team1Tag("Team1Tag");
	const FName Team2Tag("Team2Tag");

	const FName DefaultKickAb("LightPass");
	const FName DefaultDribble("Dribble");
	const FName DefaultSpecialKick("SpecialKick");
	const FName DefaultJumpKick("JumpKick");

	const float MinDistanceGK = 0.f;
	const float MinDistanceFW = 0.f;
	const float MinDistanceMF = 0.f;
	const float MinDistanceDF = 0.f;
}