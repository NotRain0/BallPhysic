// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlateform.generated.h"

UCLASS()
class FORCEBALL_API AMovingPlateform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlateform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // Position initiale
    FVector StartLocation;

    // Position cible
    UPROPERTY(EditAnywhere, Category = "Movement")
    FVector TargetLocation;

    // Vitesse de déplacement
    UPROPERTY(EditAnywhere, Category = "Movement")
    float Speed = 200.0f;

    // Durée d'arrêt aux extrémités
    UPROPERTY(EditAnywhere, Category = "Movement")
    float PauseTime = 1.0f;

    // Timer pour la pause
    FTimerHandle PauseTimerHandle;

    // Booléen pour savoir si la plateforme attend
    bool bIsPaused = false;

    // Direction du déplacement
    bool bGoingToTarget = true;

    // Fonction pour inverser la direction après une pause
    void SwitchDirection();

    UPROPERTY(EditAnywhere, Category = "Movement")
    bool canPlateformMove = true; //Ca mevite davoir a creer une idle plateform, juste je changerai la bool

};
