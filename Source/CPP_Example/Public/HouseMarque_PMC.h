//	Joonas Nätynki

#pragma once

//	Useful functions for procedural mesh generation
#include "KismetProceduralMeshLibrary.h"

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "HouseMarque_PMC.generated.h"

//	Class forward declarations
class UProceduralMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPP_EXAMPLE_API UHouseMarque_PMC : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHouseMarque_PMC();

private:
	//	The actual mesh of the pmc
	UProceduralMeshComponent * _mesh;
	//	The heighmap texture object
	UTexture2D * _heightMap;
	//	Width of the heightmap texture
	int _textureWidth;
	//	Height of the heightmap texture
	int _textureHeight;
	//	How many vertices per side
	int _quadsPerSectionSide = 200;
	//	Quad size in world units
	int quadSize = 10.0f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//	Setter for heightmap
	void SetHeightMap(UTexture2D * heightmap);
	void CreateMeshSection();
	FColor GetPixelColorFromTexture(UTexture2D * texture, int x, int y);
};
