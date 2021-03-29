// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalGate.h"

// Sets default values
AGoalGate::AGoalGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AGoalGate::ReceiveBall_Implementation()
{
}

// Called when the game starts or when spawned
void AGoalGate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoalGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

