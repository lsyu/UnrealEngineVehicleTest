// Fill out your copyright notice in the Description page of Project Settings.

#include "MyVehicle.h"
#include "VehiclePawn.h"

#include "WheelFront.h"
#include "WheelRear.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

#include "Vehicles/WheeledVehicleMovementComponent4W.h"


// Sets default values
AVehiclePawn::AVehiclePawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/Vehicles/VH_Buggy/Mesh/SK_Buggy_Vehicle.SK_Buggy_Vehicle"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);

	static ConstructorHelpers::FObjectFinder<UMaterial> CarMaterial0(TEXT("/Game/Vehicles/VH_Buggy/Materials/M_Buggy_A"));
	static ConstructorHelpers::FObjectFinder<UMaterial> CarMaterial1(TEXT("/Game/Vehicles/VH_Buggy/Materials/M_Buggy_B"));
	GetMesh()->SetMaterial(0, CarMaterial0.Object);
	GetMesh()->SetMaterial(1, CarMaterial1.Object);

	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Vehicles/VH_Buggy/Blueprints/VehicleAnimate"));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);

	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());
	check(Vehicle4W->WheelSetups.Num() == 4);

	// Setup the wheels
	Vehicle4W->WheelSetups[0].WheelClass = UWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("F_L_wheelJNT");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -8.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = UWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("F_R_wheelJNT");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 8.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = UWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("B_L_wheelJNT");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -8.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = UWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("B_R_wheelJNT");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 8.f, 0.f);

	Vehicle4W->Mass = 2500.0f;
	Vehicle4W->DragCoefficient = 0.0f;

	Vehicle4W->ChassisWidth = 175;
	Vehicle4W->ChassisHeight = 75;

	// Adjust the tire loading
	//Vehicle4W->MinNormalizedTireLoad = 0.0f;
	//Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
	//Vehicle4W->MaxNormalizedTireLoad = 1.0f;
	//Vehicle4W->MaxNormalizedTireLoadFiltered = 1.0f;

	// Engine 
	// Torque setup
	Vehicle4W->MaxEngineRPM = 100000.0f;
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 60000.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(18900.0f, 153600.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(57300.0f, 60000.0f);

	// Adjust the steering 
	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.7f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.4f);

	// Transmission	
	// We want 4wd
	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;
	// Drive the rear wheels a little more than the front
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.75;

	// Automatic gearbox
	Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.5f;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 0.1f;
	//for (FVehicleGearData& gear : Vehicle4W->TransmissionSetup.ForwardGears)
	//{
	//	gear.Ratio = 5;
	//	gear.DownRatio = 0.1;
	//	gear.UpRatio = 0.1;
	//}

	// Physics settings
	// Adjust the center of mass - the buggy is quite low
	//UPrimitiveComponent* UpdatedPrimitive = Cast<UPrimitiveComponent>(Vehicle4W->UpdatedComponent);
	//if (UpdatedPrimitive)
	//{
	//	UpdatedPrimitive->BodyInstance.COMNudge = FVector(8.0f, 0.0f, 0.0f);
	//}


	// Create a spring arm component for our chase camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	SpringArm->SetWorldRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArm->AttachTo(RootComponent);
	SpringArm->TargetArmLength = 750.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 2.5f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 10.0f;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;

	// Create the chase camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeRotation(FRotator(10.0f, 0.0f, 0.0f));
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;
}

// Called when the game starts or when spawned
void AVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVehiclePawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AVehiclePawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AVehiclePawn::OnMoveForward);
	InputComponent->BindAxis("MoveRight", this, &AVehiclePawn::OnMoveRight);
}

void AVehiclePawn::OnMoveForward(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void AVehiclePawn::OnMoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

