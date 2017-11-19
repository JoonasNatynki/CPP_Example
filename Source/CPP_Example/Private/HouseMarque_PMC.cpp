//	Joonas Nätynki

#include "HouseMarque_PMC.h"
#include "ProceduralMeshComponent.h"

// Sets default values for this component's properties
UHouseMarque_PMC::UHouseMarque_PMC()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//	Initialize pmc
	_mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("pmc"));
	//	New in UE 4.17, multi-threaded PhysX cooking.
	_mesh->bUseAsyncCooking = true;

	//_heightMap = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/HeightMap.HeightMap'"));
}


// Called when the game starts
void UHouseMarque_PMC::BeginPlay()
{
	Super::BeginPlay();

	CreateMeshSection();

	// ...
	
}


// Called every frame
void UHouseMarque_PMC::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHouseMarque_PMC::SetHeightMap(UTexture2D * heightmap)
{
	//	If heightmap isn't nullptr, set _heightMap
	if (heightmap != nullptr) { _heightMap = heightmap; }
	return;
}

FColor UHouseMarque_PMC::GetPixelColorFromTexture(UTexture2D * texture, int x, int y)
{
	FColor * FormatedImageData = static_cast<FColor*>(texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_ONLY));
	_textureWidth = texture->PlatformData->Mips[0].SizeX;
	_textureHeight = texture->PlatformData->Mips[0].SizeY;
	texture->PlatformData->Mips[0].BulkData.Unlock();

	FColor PixelColor;

	if (x >= 0 && x < _textureWidth && y >= 0 && y < _textureHeight)
	{
		PixelColor = FormatedImageData[y * _textureWidth + x];
	}

	return PixelColor;
}

void UHouseMarque_PMC::CreateMeshSection()
{
	TArray<FVector> vertices;
	TArray<int32> Triangles;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FColor> vertexColors;

	_heightMap->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_ONLY);
	_textureWidth = _heightMap->PlatformData->Mips[0].SizeX;
	_textureHeight = _heightMap->PlatformData->Mips[0].SizeY;
	_heightMap->PlatformData->Mips[0].BulkData.Unlock();

	for (int y = 0; y < _quadsPerSectionSide + 1; y++)
	{
		for (int x = 0; x < _quadsPerSectionSide + 1; x++)
		{
			float xcoord = x * quadSize;
			float ycoord = y * quadSize;
			int xpixel = ((xcoord) / 2000.0f) * _textureWidth;
			int ypixel = ((ycoord) / 2000.0f) * _textureHeight;
			float zcoord = pow(
				GetPixelColorFromTexture(
										_heightMap,
										xpixel,
										ypixel).R,
										0.4f
										);
			vertices.Add(FVector(xcoord, ycoord, zcoord));
			UV0.Add(FVector2D(xpixel, ypixel));
		}
	}

	UKismetProceduralMeshLibrary::CreateGridMeshTriangles((_quadsPerSectionSide + 1), (_quadsPerSectionSide + 1), true, Triangles);
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(vertices, Triangles, UV0, normals, tangents);

	_mesh->CreateMeshSection(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);

	// Enable collision data
	_mesh->ContainsPhysicsTriMeshData(false);
}

