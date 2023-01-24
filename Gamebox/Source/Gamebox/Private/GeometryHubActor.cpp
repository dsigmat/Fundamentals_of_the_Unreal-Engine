// Fill out your copyright notice in the Description page of Project Settings.


#include "GeometryHubActor.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogGeometryHub, All, All)

// Sets default values
AGeometryHubActor::AGeometryHubActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGeometryHubActor::BeginPlay()
{
	Super::BeginPlay();

	/*DoActorSpawn1();
	DoActorSpawn2();*/
	DoActorSpawn3();
	
}

// Called every frame
void AGeometryHubActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AGeometryHubActor::DoActorSpawn1()
{
	UWorld* World = GetWorld();//возвращает указатель на глобальный объект мира игры. 
	if (World)
	{
		for (int32 i = 0; i < 10; i++)
		{
			const FTransform GeometryTransform = FTransform(FRotator::ZeroRotator, FVector(0.0f, 300.0f * i, 300.0f));
			ABaseActor* Geometry = World->SpawnActor<ABaseActor>(GeometryClass, GeometryTransform);
			if (Geometry)
			{
				FGeometryData Data;
				Data.MoveType = FMath::RandBool() ? EMovementType::Static : EMovementType::Sin;
				Geometry->SetGeometryData(Data);
			}
		}
	}
}

void AGeometryHubActor::DoActorSpawn2()
{
	UWorld* World = GetWorld();//возвращает указатель на глобальный объект мира игры. 
	if (World)
	{
		for (int32 i = 0; i < 10; i++)
		{
			const FTransform GeometryTransform = FTransform(FRotator::ZeroRotator, FVector(0.0f, 300.0f * i, 700.0f));
			ABaseActor* Geometry = World->SpawnActorDeferred<ABaseActor>(GeometryClass, GeometryTransform);
			if (Geometry)
			{
				FGeometryData Data;
				Data.Color = FLinearColor::MakeRandomColor();
				Geometry->SetGeometryData(Data);
				Geometry->FinishSpawning(GeometryTransform);
			}
		}
	}
}

void AGeometryHubActor::DoActorSpawn3()
{
	UWorld* World = GetWorld();//возвращает указатель на глобальный объект мира игры. 
	if (World)
	{
		for (const FGeometryPayload Payload : GeometryPayloads)
		{
			ABaseActor* Geometry = World->SpawnActorDeferred<ABaseActor>(Payload.GeometryClass, Payload.InitialTransform);
			if (Geometry)
			{
				Geometry->SetGeometryData(Payload.Data);

				Geometry->OnColorChanged.AddDynamic(this, &AGeometryHubActor::OnColorChaged);
				Geometry->OnTimerFinished.AddUObject(this, &AGeometryHubActor::OnTimerFinished);

				Geometry->FinishSpawning(Payload.InitialTransform);
			}
		}
	}
}

void AGeometryHubActor::OnColorChaged(const FLinearColor& Color, const FString& Name)
{
	UE_LOG(LogGeometryHub, Warning, TEXT("Actor name: %s, Color: %s"), *Name, *Color.ToString());
}

void AGeometryHubActor::OnTimerFinished(AActor* Actor)
{
	if (!Actor) return;
	UE_LOG(LogGeometryHub, Error, TEXT("Timer finished: %s"), *Actor->GetName());

	ABaseActor* Geometry = Cast<ABaseActor>(Actor);

	if (!Geometry) return;
	UE_LOG(LogGeometryHub, Error, TEXT("Cast is success, amplitude %f"), Geometry->GetGeometryData().Amplitude);

	Geometry->Destroy();
	//Geometry->SetLifeSpan(2.0f);
}
