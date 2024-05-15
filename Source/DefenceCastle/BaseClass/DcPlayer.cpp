// Fill out your copyright notice in the Description page of Project Settings.


#include "DcPlayer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ADcPlayer::ADcPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	RootComponent = SphereMesh;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(SphereMesh);
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovement");
}

// Called when the game starts or when spawned
void ADcPlayer::BeginPlay()
{
	Super::BeginPlay();
	SpringArmComp->TargetArmLength = SpringArmLengthDefault;
	XScrollSpeed = XScrollSpeedDefault;
	YScrollSpeed = YScrollSpeedDefault;
}

void ADcPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADcPlayer::Zoom(const float Amount)
{
	if(!GetWorld())
		return;
	
	if(!SpringArmComp)
		return;
	
	const float NewLength =SpringArmComp->TargetArmLength + (Amount * ZoomSpeed * GetWorld()->GetDeltaSeconds());
	
	float FinalLength = FMath::Lerp(SpringArmComp->TargetArmLength, NewLength, 0.7f);
	FinalLength = FMath::Clamp(FinalLength, SpringArmLengthMin, SpringArmLengthMax);

	
	SpringArmComp->TargetArmLength = FinalLength;
}

void ADcPlayer::MoveHorizontal(const float Amount)
{
	const float NewValue = Amount * YScrollSpeed;
	FloatingPawnMovement->AddInputVector(FVector(0.f,NewValue ,0.f));
}

void ADcPlayer::MoveVertical(const float Amount)
{
	const float NewValue = Amount * YScrollSpeed;
	FloatingPawnMovement->AddInputVector(FVector(NewValue, 0.f,0.f));
}

void ADcPlayer::ResetCamera(const FVector& Location)
{
	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.CallbackTarget = this;
	LatentActionInfo.UUID = GetUniqueID();
	LatentActionInfo.Linkage = 0;
	UKismetSystemLibrary::MoveComponentTo(RootComponent,Location, GetActorRotation(),false, false,0.2f, false, EMoveComponentAction::Move, LatentActionInfo);
	SetActorLocation(Location);
}

// Called every frame
void ADcPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

