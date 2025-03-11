// Fill out your copyright notice in the Description page of Project Settings.


#include "Bumper.h"

#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "BallPawn.h"


// Sets default values
ABumper::ABumper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABumper::BeginPlay()
{
	Super::BeginPlay();
	
    TArray<UBoxComponent*> BoxComponents;
    GetComponents(BoxComponents);

    for (UBoxComponent* Comp : BoxComponents)
    {
        if (Comp->ComponentHasTag(FName("Hitbox")))
        {
            Hitbox = Comp;
            Hitbox->OnComponentBeginOverlap.AddDynamic(this, &ABumper::OnHitboxOverlap);
            //UE_LOG(LogTemp, Warning, TEXT("Catapulte got hitbox"));
            break;
        }
    }
}

// Called every frame
void ABumper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABumper::OnHitboxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(ABallPawn::StaticClass()) && canTrigger)
    {
        UE_LOG(LogTemp, Warning, TEXT("player in box"));
        canTrigger = false;
        Cast<ABallPawn>(OtherActor)->BallMesh->AddImpulse(FVector(0, 0, BumpImpulse));

        FTimerHandle TimerHandleLambda;
        GetWorld()->GetTimerManager().SetTimer(TimerHandleLambda, [this]()
            {
                canTrigger = true;
            }, 2.0f, false);
    }
}
