// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

#include "BaseActor.generated.h"

UCLASS()
class GAMEBOX_API ABaseActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseActor();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Weapon")
		int32 WeaponsNum = 4;
	UPROPERTY(EditDefaultsOnly, Category = "Stat")
		int32 KillsNum = 7;
	UPROPERTY(EditInstanceOnly, Category = "Health")
		float Health = 34.455869f;
	UPROPERTY(EditAnywhere, Category = "Health")
		bool IsDead = false;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		bool HasWeapon = true;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	void PrintTypes();
	void PrintStringType();
	void PrintTransform();


};
