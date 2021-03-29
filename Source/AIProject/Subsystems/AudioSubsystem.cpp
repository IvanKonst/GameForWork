// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"


void UAudioSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void UAudioSubsystem::SetVolume(const float AudioMultiplier)
{
	AudioVolume = AudioMultiplier;
	
	if(CurrentTrack)
	{
		CurrentTrack->SetVolumeMultiplier(AudioVolume);
	}
}

void UAudioSubsystem::SetHomePlaylist(const TArray<USoundWave*> Playlist)
{
	HomeMusicArray = Playlist;
	HomePlaylistLength = HomeMusicArray.Num();
}

void UAudioSubsystem::SetFieldPlaylist(const TArray<USoundWave*> Playlist)
{
	FieldMusicArray = Playlist;
	FieldPlaylistLength = FieldMusicArray.Num();
}

void UAudioSubsystem::NextTrack()
{
	if(CurrentPlaylistLength == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Playlist is empty - audio subsystem"))
		return;
	}

	if(CurrentTrackIndex > CurrentPlaylistLength - 1)
	{
		CurrentTrackIndex = 0;
	} else
	{
		CurrentTrackIndex++;
		UE_LOG(LogTemp, Warning, TEXT("Current track index: %d"), CurrentTrackIndex)
	}
}

UAudioComponent* UAudioSubsystem::PlayLoop(int32 CurrentTrackIndexIn, int32 PlaylistLength,TArray<USoundWave*> Playlist, float AudioVolumeIn)
{
	CurrentPlaylistLength = PlaylistLength;
	
	UE_LOG(LogTemp, Warning, TEXT("TrackIndex: %d"), CurrentTrackIndexIn)
	
	if(Playlist.IsValidIndex(CurrentTrackIndexIn))
	{
		CurrentTrack = UGameplayStatics::SpawnSound2D(GetWorld(), Playlist[CurrentTrackIndexIn], AudioVolumeIn);
		return CurrentTrack;
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid index - playloop: %d"), CurrentTrackIndexIn)
	}
	
	return nullptr;
	
}

void UAudioSubsystem::PlayHomeMenuMusic()
{
	CurrentPlaylistLength = HomePlaylistLength;
	PlayLoop(CurrentTrackIndex, HomePlaylistLength, HomeMusicArray, AudioVolume);
	
	if(CurrentTrack)
	{
		CurrentTrack->OnAudioFinished.AddDynamic(this, &UAudioSubsystem::HomeMenuNextTrack);
	}
}

void UAudioSubsystem::HomeMenuNextTrack()
{
	if(CurrentTrack != nullptr)
	{
		CurrentTrack->DestroyComponent();
	}
	
	NextTrack();
	PlayHomeMenuMusic();
}

void UAudioSubsystem::PlayFieldMusic()
{
	CurrentPlaylistLength = FieldPlaylistLength;
	PlayLoop(CurrentTrackIndex, FieldPlaylistLength, FieldMusicArray, AudioVolume);
	if(CurrentTrack)
	{
		CurrentTrack->OnAudioFinished.AddDynamic(this, &UAudioSubsystem::FieldMusicNextTrack);
	}
}

void UAudioSubsystem::FieldMusicNextTrack()
{
	NextTrack();
	PlayFieldMusic();
}

int32 UAudioSubsystem::GetCurrentTrackIndex() const
{
	return CurrentTrackIndex;
}

void UAudioSubsystem::InitAudioSubsystem(const TArray<USoundWave*> FieldMusicArrayIn, TArray<USoundWave*> HomeMenuMusicArrayIn, const float AudioMultiplier)
{
	CurrentTrackIndex = 0;
	CurrentPlaylistLength = 0;
	CurrentTrack = nullptr;

	SetFieldPlaylist(FieldMusicArrayIn);
	SetHomePlaylist(HomeMenuMusicArrayIn);
	
	AudioVolume = AudioMultiplier;
	if(FieldMusicArrayIn.Num() != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Default array isn't empty..."))
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Please stop, my ears are hurting..."))
	}
}

float UAudioSubsystem::GetAudioVolume() const
{
	return AudioVolume;
}
