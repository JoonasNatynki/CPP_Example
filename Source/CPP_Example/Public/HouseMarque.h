//	Joonas Nätynki

#pragma once

//	The procedural mesh component that displays the mesh with the vertex displacement using a heightmap
#include "HouseMarque_PMC.h"
//	Used only to show where the actor is in the editor
#include "Components/BillboardComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HouseMarque.generated.h"

UCLASS()
class CPP_EXAMPLE_API AHouseMarque : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHouseMarque();
	
	UPROPERTY(VisibleAnywhere)
		//	Procedural mesh component
		UHouseMarque_PMC * _pmc;
	UPROPERTY(VisibleAnywhere)
		//	The root sprite. So that we can tell where the actor actually is in the level (only visible in editor)
		UBillboardComponent * _rootSprite;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
