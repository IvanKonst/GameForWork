// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Sound/SoundWave.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class AIPROJECT_API UAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	UPROPERTY(EditAnywhere)
	TArray<USoundWave*> HomeMusicArray;

	UPROPERTY(EditAnywhere)
	TArray<USoundWave*> FieldMusicArray;
	
private:	
	UPROPERTY()
	int32 CurrentTrackIndex = 0;
	UPROPERTY()
	int32 CurrentPlaylistLength = 0;
	UPROPERTY()
	int32 FieldPlaylistLength = 0;
	UPROPERTY()
	int32 HomePlaylistLength = 0;

	UPROPERTY()
	float AudioVolume;

	UPROPERTY()
	UAudioComponent* CurrentTrack;

public:
	/**
	 * @param AudioMultiplier 0.0f - 1.0f audio multiplier to adjust the volume of the music
	 */
	UFUNCTION(BlueprintCallable, Category = "AudioSubsystem")
	void InitAudioSubsystem(const TArray<USoundWave*> FieldMusicArrayIn, const TArray<USoundWave*> HomeMenuMusicArrayIn, const float AudioMultiplier = 1.0f);
	
	UFUNCTION(BlueprintCallable, Category = "AudioSubsystem")
	float GetAudioVolume() const;
	
	UFUNCTION(BlueprintCallable, Category = "AudioSubsystem")
	int32 GetCurrentTrackIndex() const;
	
	/**
	 * @param AudioMultiplier 0.0f - 1.0f audio multiplier to adjust the volume of the music
	 */
	UFUNCTION(BlueprintCallable, Category = "AudioSubsystem")
	void SetVolume(const float AudioMultiplier = 1.0f);

	/**
	 * @param Playlist - the new home playlist
	 */
	UFUNCTION(BlueprintCallable, Category = "AudioSubsystem")
	void SetHomePlaylist(const TArray<USoundWave*> Playlist);

	/**
	 * @param Playlist - the new field(soccer field) playlist
	 */
	UFUNCTION(BlueprintCallable, Category = "AudioSubsystem")
	void SetFieldPlaylist(const TArray<USoundWave*> Playlist);

	UFUNCTION(BlueprintCallable, Category = "AudioSubsystem")
	void NextTrack();

	UFUNCTION(BlueprintCallable, Category = "AudioSubsystem")
	UAudioComponent* PlayLoop(int32 CurrentTrackIndexIn, int32 PlaylistLength, TArray<USoundWave*> Playlist, float AudioVolumeIn = 1.0f);
	
	UFUNCTION(BlueprintCallable, Category = "AudioSubsystem")
	void PlayHomeMenuMusic();
	
	UFUNCTION(BlueprintCallable, Category = "AudioSubsystem")
	void HomeMenuNextTrack();
	
	UFUNCTION(BlueprintCallable, Category = "AudioSubsystem")
	void PlayFieldMusic();
	
	UFUNCTION(BlueprintCallable, Category = "AudioSubsystem")
	void FieldMusicNextTrack();
};
