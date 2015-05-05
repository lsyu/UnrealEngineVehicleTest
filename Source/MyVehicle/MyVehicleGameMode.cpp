// Fill out your copyright notice in the Description page of Project Settings.

#include "MyVehicle.h"
#include "MyVehicleGameMode.h"
#include "VehiclePawn.h"

AMyVehicleGameMode::AMyVehicleGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = AVehiclePawn::StaticClass();
}

AActor* AMyVehicleGameMode::ChoosePlayerStart(AController* Player)
{
	APlayerStart* SpawnExists = nullptr;
	for (int32 i = 0; i < PlayerStarts.Num(); i++)
	{
		APlayerStart* TestSpawn = PlayerStarts[i];
		if (IsSpawnpointPreferred(TestSpawn, Player))
		{
			SpawnExists = TestSpawn;
			break;
		}
	}

	return SpawnExists ? SpawnExists : Super::ChoosePlayerStart(Player);
}

bool AMyVehicleGameMode::IsSpawnpointPreferred(APlayerStart* SpawnPoint, AController* Player) const
{
	ACharacter* MyPawn = Player ? Cast<ACharacter>(Player->GetPawn()) : nullptr;
	if (MyPawn)
	{
		const FVector& SpawnLocation = SpawnPoint->GetActorLocation();
		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			ACharacter* OtherPawn = Cast<ACharacter>(*It);
			if (OtherPawn && OtherPawn != MyPawn)
			{
				const float CombinedHeight = (MyPawn->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + OtherPawn->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) * 2.0f;
				const float CombinedRadius = MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() + OtherPawn->GetCapsuleComponent()->GetScaledCapsuleRadius();
				const FVector& OtherLocation = OtherPawn->GetActorLocation();

				// check if player start overlaps this pawn
				if (FMath::Abs(SpawnLocation.Z - OtherLocation.Z) < CombinedHeight && (SpawnLocation - OtherLocation).Size2D() < CombinedRadius)
				{
					return false;
				}
			}
		}
	}

	return true;
}


