// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "BallPawn.generated.h"

class UInputComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class FORCEBALL_API ABallPawn : public APawn
{
    GENERATED_BODY()

public:
    ABallPawn();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void JumpCustom();
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Landed();

    void RightGravityPressFct();
    bool PressingRight = false;
    void UpGravityPressFct();
    bool PressingUp = false;
    void FrontGravityPressFct();
    bool PressingFront = false;

    void IncreaseGravityFct();
    void LowerGravityFct();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* PlayerMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* UpGravity;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* FrontGravity;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* RightGravity;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* IncreaseGravity;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* DecreaseGravity;


    UPROPERTY(VisibleAnywhere, Category = "Comp")
    class UStaticMeshComponent* BallMesh;

    UPROPERTY(VisibleAnywhere, Category = "Comp")
    class USceneComponent* SceneComp;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveForce = 100000.0f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float JumpImpulse =500000.0f;

    int JumpCount = 0;

    FVector2D MovementVector;

    UPROPERTY(EditAnywhere, Category = "Movement")
    FVector CurrentGravityVec = FVector(0, 0, -1);

    UPROPERTY(EditAnywhere, Category = "Movement")
    float GravityStrength = 9.8f;

protected:
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComponent, FVector NormalImpulse,
        const FHitResult& Hit);
};