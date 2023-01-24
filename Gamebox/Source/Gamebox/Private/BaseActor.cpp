// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseActor.h"
#include "Engine/Engine.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseActor, All, All)

// Sets default values
ABaseActor::ABaseActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);


}

// Called when the game starts or when spawned
void ABaseActor::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation(); //�������� ������ � �������

	SetColor(GeometryData.Color);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABaseActor::OnTimerFired, GeometryData.TimerRate, true);

	//PrintTransform();
	//PrintStringType();
	//PrintTypes();		
}

void ABaseActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UE_LOG(LogBaseActor, Error, TEXT("Actor is dead %s"), *GetName());
}

// Called every frame
void ABaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMovement();

}

void ABaseActor::HandleMovement()//����������� ������� �������� �� ������
{
	//z = z0 + amplitude * sin(freq * time)

	switch (GeometryData.MoveType)
	{
	case EMovementType::Sin:
	{
		FVector CurrentLocation = GetActorLocation();
		if (GetWorld())
		{
			float Time = GetWorld()->GetTimeSeconds();
			CurrentLocation.Z = InitialLocation.Z + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);

			SetActorLocation(CurrentLocation); // ������ ������� ������ ��������� � ����
		}
	}
	break;
	case EMovementType::Static:
		break;
	default:
		break;
	}
}

void ABaseActor::PrintTypes()
{
	UE_LOG(LogBaseActor, Display, TEXT("Actor name: %s"), *GetName());

	UE_LOG(LogBaseActor, Display, TEXT("Weapons Num: %d, Kills Num: %i"), WeaponsNum, KillsNum);
	UE_LOG(LogBaseActor, Error, TEXT("Health: %f"), Health);
	UE_LOG(LogBaseActor, Error, TEXT("IsDead: %d"), IsDead);
	UE_LOG(LogBaseActor, Error, TEXT("HasWeapon: %d"), static_cast<int>(HasWeapon));
}

void ABaseActor::PrintStringType()
{
	FString Name = "John";

	FString WeaponsNumStr = "Weapons Num = " + FString::FromInt(WeaponsNum);
	FString HealthStr = "Health = " + FString::SanitizeFloat(Health);
	FString HasWeaponStr = "Has Weapon = " + FString(HasWeapon ? "true" : "false");

	FString Stat = FString::Printf(TEXT("\n\n ===All Stat=== \n %s \n %s \n %s"), *WeaponsNumStr, *HealthStr, *HasWeaponStr);

	UE_LOG(LogBaseActor, Warning, TEXT("%s"), *Stat);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Name);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Stat, true, FVector2D(2.0f, 2.0f));
	}	
}

void ABaseActor::PrintTransform()
{
	FTransform Transform = GetActorTransform();
	FVector Location = Transform.GetLocation();
	FRotator Rotation = Transform.Rotator();
	FVector Scale = Transform.GetScale3D();

	UE_LOG(LogBaseActor, Warning, TEXT("Actor name: %s"), *GetName());
	UE_LOG(LogBaseActor, Warning, TEXT("Transform: %s"), *Transform.ToString());
	UE_LOG(LogBaseActor, Warning, TEXT("Location: %s"), *Location.ToString());
	UE_LOG(LogBaseActor, Warning, TEXT("Rotation: %s"), *Rotation.ToString());
	UE_LOG(LogBaseActor, Warning, TEXT("Scale: %s"), *Scale.ToString());


	UE_LOG(LogBaseActor, Error, TEXT("Scale: %s"), *Transform.ToHumanReadableString());
}

void ABaseActor::SetColor(const FLinearColor& Color)
{
	if (!BaseMesh)
	{
		return;
	}
	UMaterialInstanceDynamic* DynMaterial = BaseMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMaterial)
	{
		DynMaterial->SetVectorParameterValue("Color", Color);
	}
}

void ABaseActor::OnTimerFired()
{
	if (++TimerCount <= MaxTimerCount)
	{
		FLinearColor NewColor = FLinearColor::MakeRandomColor();
		UE_LOG(LogBaseActor, Warning, TEXT("Timer Count: %i, Color: %s"), TimerCount, *NewColor.ToString());
		SetColor(NewColor);
		OnColorChanged.Broadcast(NewColor, GetName());
	}
	else
	{
		UE_LOG(LogBaseActor, Warning, TEXT("Timer has been stopped!"));
		GetWorldTimerManager().ClearTimer(TimerHandle);
		OnTimerFinished.Broadcast(this);
	}
}

