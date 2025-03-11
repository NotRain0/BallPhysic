// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlateform.h"

// Sets default values
AMovingPlateform::AMovingPlateform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMovingPlateform::BeginPlay()
{
    Super::BeginPlay();

    // Sauvegarde la position de départ
    StartLocation = GetActorLocation();
}

void AMovingPlateform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsPaused || !canPlateformMove) return;

    FVector CurrentLocation = GetActorLocation();

    FVector Destination = bGoingToTarget ? TargetLocation : StartLocation;

    FVector Direction = (Destination - CurrentLocation).GetSafeNormal();
    FVector NewLocation = CurrentLocation + Direction * Speed * DeltaTime;

    SetActorLocation(NewLocation);

    if (FVector::Dist(NewLocation, Destination) < 5.0f)
    {
        bIsPaused = true; // Pause pour le style
        GetWorldTimerManager().SetTimer(PauseTimerHandle, this, &AMovingPlateform::SwitchDirection, PauseTime);
    }
}

void AMovingPlateform::SwitchDirection()
{
    bGoingToTarget = !bGoingToTarget; 
    bIsPaused = false;
}