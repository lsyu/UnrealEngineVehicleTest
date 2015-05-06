// Fill out your copyright notice in the Description page of Project Settings.

#include "MyVehicle.h"
#include "MyVehicleGameMode.h"
#include "VehiclePawn.h"

AMyVehicleGameMode::AMyVehicleGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = AVehiclePawn::StaticClass();
}

bool AMyVehicleGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	return false;
}

AActor* AMyVehicleGameMode::ChoosePlayerStart(AController* Player)
{
	TArray<APlayerStart*> FallbackSpawns;
	APlayerStart* TestSpawn = nullptr;
	for (int32 i = 0; i < PlayerStarts.Num(); i++)
	{
		if (Cast<APlayerStartPIE>(PlayerStarts[i]) != nullptr)
		{
			// Always prefer the first "Play from Here" PlayerStart, if we find one while in PIE mode
			TestSpawn = PlayerStarts[i];
			break;
		}
		else if (PlayerStarts[i] != nullptr && IsSpawnpointPreferred(PlayerStarts[i], Player))
		{
			TestSpawn = PlayerStarts[i];
			break;
		}
		else
		{
			FallbackSpawns.Add(PlayerStarts[i]);
		}
	}

	if (!TestSpawn)
	{
		TestSpawn = FallbackSpawns[FMath::RandHelper(FallbackSpawns.Num())];
	}

	return TestSpawn ? TestSpawn : Super::ChoosePlayerStart(Player);
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
				const float CombinedHeight = (MyPawn->CapsuleComponent->GetScaledCapsuleHalfHeight() + OtherPawn->CapsuleComponent->GetScaledCapsuleHalfHeight()) * 2.0f;
				const float CombinedRadius = MyPawn->CapsuleComponent->GetScaledCapsuleRadius() + OtherPawn->CapsuleComponent->GetScaledCapsuleRadius();
				const FVector OtherLocation = OtherPawn->GetActorLocation();

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


