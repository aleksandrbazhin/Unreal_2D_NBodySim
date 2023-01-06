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
	void SetMass(float NewMass);
	float Mass = 1.0f;
	FVector2D Position;
	FVector2D Velocity;

	static FVector LocationFrom2DPosition(FVector2D planar_position);

	virtual void Tick(float DeltaSecs) override;
	
	UStaticMeshComponent* StaticMesh;
	UStaticMeshComponent* GetMesh(); 


protected:
	virtual void BeginPlay() override;

private:
	void UpdatePosition();

};
