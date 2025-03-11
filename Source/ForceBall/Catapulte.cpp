// Fill out your copyright notice in the Description page of Project Settings.


#include "Catapulte.h"

#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "BallPawn.h"

ACatapulte::ACatapulte()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACatapulte::BeginPlay()
{
    Super::BeginPlay();

    TArray<USceneComponent*> SceneComponents;
    GetComponents(SceneComponents);

    for (USceneComponent* Comp : SceneComponents)
    {
        if (Comp->ComponentHasTag(FName("SceneComp")))
        {
            SceneComp = Comp;
            //UE_LOG(LogTemp, Warning, TEXT("Catapulte got scene"));
            break;
        }
    }

    TArray<UBoxComponent*> BoxComponents;
    GetComponents(BoxComponents);

    for (UBoxComponent* Comp : BoxComponents)
    {
        if (Comp->ComponentHasTag(FName("Hitbox")))
        {
            Hitbox = Comp;
            Hitbox->OnComponentBeginOverlap.AddDynamic(this, &ACatapulte::OnHitboxOverlap);
            //UE_LOG(LogTemp, Warning, TEXT("Catapulte got hitbox"));
            break;
        }
    }
}

void ACatapulte::Launch()
{
    if (!SceneComp) return;

    canTrigger = false;

    ElapsedTime = 0.0f;

    GetWorldTimerManager().SetTimer(RotationTimerHandle, this, &ACatapulte::UpdateRotation, 0.016f, true);
}

void ACatapulte::UpdateRotation()
{
    if (!SceneComp) return;

    ElapsedTime += 0.016f;
    float Alpha = FMath::Clamp(ElapsedTime / AnimationDuration, 0.0f, 1.0f);

    FRotator NewRotation = FMath::Lerp(StartRotation, TargetRotation, Alpha);
    SceneComp->SetRelativeRotation(NewRotation);

    if (Alpha >= 0.2f)
    {
        GetWorldTimerManager().ClearTimer(RotationTimerHandle);

        FTimerHandle TimerHandleLambda;
        GetWorld()->GetTimerManager().SetTimer(TimerHandleLambda, [this]()
            {
                SceneComp->SetRelativeRotation(StartRotation);
                canTrigger = true;
            }, 2.0f, false);
    }
}

void ACatapulte::OnHitboxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(ABallPawn::StaticClass()) && canTrigger)
    {
        UE_LOG(LogTemp, Warning, TEXT("player in box"));

        FTimerHandle TimeHandler;
        GetWorldTimerManager().SetTimer(TimeHandler, this, &ACatapulte::Launch, 2.0f, false);
    }
}