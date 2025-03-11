// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Catapulte.generated.h"

UCLASS()
class FORCEBALL_API ACatapulte : public AActor
{
	GENERATED_BODY()
	
public:	
	ACatapulte();

protected:
	virtual void BeginPlay() override;

public:	

	UPROPERTY()
	USceneComponent* SceneComp;

	UPROPERTY()
	class UBoxComponent* Hitbox;

	bool canTrigger = true;

	UPROPERTY(EditAnywhere, Category = "Movement")
	FRotator StartRotation;
	UPROPERTY(EditAnywhere, Category = "Movement")
	FRotator TargetRotation;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AnimationDuration = 1.0f; 
	float ElapsedTime = 0.0f;
	FTimerHandle RotationTimerHandle;

	void UpdateRotation();
	void Launch();

protected:
	UFUNCTION()
	void OnHitboxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
