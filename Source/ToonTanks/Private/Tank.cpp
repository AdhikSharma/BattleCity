// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Chaos/DebugDrawQueue.h"

ATank::ATank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    TankCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Tank Camera"));
    TankCamera->SetupAttachment(SpringArm);
}

void ATank :: BeginPlay()
{
    Super::BeginPlay();

    PlayerControllerRef = Cast<APlayerController>(GetController());
}

void ATank :: SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"),this,&ATank::Turn);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FHitResult hitResult;
    if(PlayerControllerRef)
    {
        PlayerControllerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,
                            hitResult);

        DrawDebugSphere(
        GetWorld(),
        hitResult.ImpactPoint,
        10.f,
        12,
        FColor::Red,
        false,
        -1.f);

        RotateTurret(hitResult.ImpactPoint);
        
    }

}

void ATank :: Move(float value)
{
    UE_LOG(LogTemp,Warning,TEXT("Value : %f"),value);
    
    //FVector DeltaLocation(0.f);
    FVector DeltaLocation = FVector::ZeroVector;
    DeltaLocation.X = speed * value * UGameplayStatics::GetWorldDeltaSeconds(this);

    AddActorLocalOffset(DeltaLocation,true);

}

void ATank :: Turn(float value)
{
    UE_LOG(LogTemp,Warning,TEXT("Value : %f"),value);
    FRotator DeltaRotation = FRotator::ZeroRotator;
    DeltaRotation.Yaw = RotationSpeed * value * UGameplayStatics::GetWorldDeltaSeconds(this);

    AddActorLocalRotation(DeltaRotation,true);

}

