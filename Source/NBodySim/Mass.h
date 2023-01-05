// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mass.generated.h"

UCLASS()
class NBODYSIM_API AMass : public AActor
{
	GENERATED_BODY()
	
public:	
	AMass();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* mesh;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		UStaticMeshComponent* GetMesh(); 
};
