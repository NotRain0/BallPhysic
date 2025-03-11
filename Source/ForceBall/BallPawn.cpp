// Fill out your copyright notice in the Description page of Project Settings.


#include "BallPawn.h"
//

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"

// Inputs
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

ABallPawn::ABallPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
    RootComponent = SceneComp;

    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    BallMesh->SetupAttachment(RootComponent);
}

void ABallPawn::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(PlayerMappingContext, 0);
        }
    }

    BallMesh->OnComponentHit.AddDynamic(this, &ABallPawn::OnHit);
}

// Tick
void ABallPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector GravityForce = CurrentGravityVec * GravityStrength * BallMesh->GetMass();
    BallMesh->AddForce(GravityForce, NAME_None, true);
}

void ABallPawn::Move(const FInputActionValue& Value)
{

    MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr && BallMesh)
    {
        // Récupération du contrôleur
        APlayerController* PC = Cast<APlayerController>(Controller);
        if (PC)
        {
            FRotator CameraRotation = PC->GetControlRotation(); // Rotation de la caméra

            // On annule la rotation sur l'axe Z pour éviter que le mouvement soit influencé par l'inclinaison de la caméra
            FRotator YawRotation(0, CameraRotation.Yaw, 0);

            // Calcul des directions
            FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
            FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

            // Calcul de la force
            FVector Force = (Forward * MovementVector.Y + Right * MovementVector.X) * MoveForce;
            Force.Z = 0; // On garde uniquement les axes X et Y

            // Appliquer la force
            BallMesh->AddForce(Force);
        }
    }
}

void ABallPawn::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

// Saut avec double saut
void ABallPawn::JumpCustom()
{

    FVector JumpDirection = CurrentGravityVec;

    if (JumpCount == 0)
    {
        FVector JumpForce = -JumpDirection * JumpImpulse;
        JumpCount++;
        BallMesh->AddImpulse(JumpForce);

    }

    else if (JumpCount == 1) //Double jump
    {
        FVector JumpForce = -JumpDirection * JumpImpulse * 1.5f;
        JumpCount++;
        BallMesh->AddImpulse(JumpForce);
    }
}

void ABallPawn::Landed()
{
    JumpCount = 0;
}

void ABallPawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComponent, FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (Hit.ImpactNormal.Z > 0.9f)
    {
        Landed();
    }
}

void ABallPawn::IncreaseGravityFct()
{
    CurrentGravityVec += FVector((PressingFront ? 1 : 0) , (PressingRight ? 1 : 0), (PressingUp ? 1 : 0));
    UE_LOG(LogTemp, Warning, TEXT("Vecteur : %s"), *CurrentGravityVec.ToString());
}

void ABallPawn::LowerGravityFct()
{
    CurrentGravityVec -= FVector((PressingFront ? 1 : 0), (PressingRight ? 1 : 0), (PressingUp ? 1 : 0));
    UE_LOG(LogTemp, Warning, TEXT("Vecteur : %s"), *CurrentGravityVec.ToString());
}
void ABallPawn::RightGravityPressFct()
{
    PressingRight = !PressingRight;
}

void ABallPawn::UpGravityPressFct()
{
    PressingUp = !PressingUp;
}

void ABallPawn::FrontGravityPressFct()
{
    PressingFront = !PressingFront;
}

// Called to bind functionality to input
void ABallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Jumping
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABallPawn::JumpCustom);

        // Moving
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABallPawn::Move);

        // Looking, mais je l'utilise pas en fait
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABallPawn::Look);

        EnhancedInputComponent->BindAction(RightGravity, ETriggerEvent::Started, this, &ABallPawn::RightGravityPressFct);
        EnhancedInputComponent->BindAction(RightGravity, ETriggerEvent::Completed, this, &ABallPawn::RightGravityPressFct);

        EnhancedInputComponent->BindAction(UpGravity, ETriggerEvent::Started, this, &ABallPawn::UpGravityPressFct);
        EnhancedInputComponent->BindAction(UpGravity, ETriggerEvent::Completed, this, &ABallPawn::UpGravityPressFct);

        EnhancedInputComponent->BindAction(FrontGravity, ETriggerEvent::Started, this, &ABallPawn::FrontGravityPressFct);
        EnhancedInputComponent->BindAction(FrontGravity, ETriggerEvent::Completed, this, &ABallPawn::FrontGravityPressFct);

        EnhancedInputComponent->BindAction(IncreaseGravity, ETriggerEvent::Started, this, &ABallPawn::IncreaseGravityFct);

        EnhancedInputComponent->BindAction(DecreaseGravity, ETriggerEvent::Started, this, &ABallPawn::LowerGravityFct);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No EIC found"));
    }
}