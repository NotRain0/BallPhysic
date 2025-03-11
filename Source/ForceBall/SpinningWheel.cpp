// Fill out your copyright notice in the Description page of Project Settings.


#include "SpinningWheel.h"
//
//

// Sets default values
ASpinningWheel::ASpinningWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpinningWheel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpinningWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator Rotation(RotationSpeed * DeltaTime, 0, 0);
	AddActorLocalRotation(Rotation);
}