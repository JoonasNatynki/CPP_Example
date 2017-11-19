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
	//	Load up the heightmap texture
	_heightMap = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/HeightMap.HeightMap'"));

	if (_heightMap != nullptr) { CreateMeshSection(); }
	else { UE_LOG(LogTemp, Warning, TEXT("Couldn't find heightmap data or it was the wrong type.")); };
}


// Called when the game starts
void UHouseMarque_PMC::BeginPlay()
{
	Super::BeginPlay();	
}


// Called every frame
void UHouseMarque_PMC::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//	Read color of pixel at texturecoordinate (x, y), return FColor
FColor UHouseMarque_PMC::GetPixelColorFromTexture(UTexture2D * texture, int x, int y)
{
	//	Raw color date of texture (lock texture so that it can't be accessed by other threads or the GPU(?))
	FColor * FormatedImageData = static_cast<FColor*>(texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_ONLY));
	_textureWidth = texture->PlatformData->Mips[0].SizeX;
	_textureHeight = texture->PlatformData->Mips[0].SizeY;
	//	Release texture data lock
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
	TArray<FColor> vertexColors;	//	Not really used yet

	//	Load the mip0 bulkdata from texture and lock the data so other threads don't access it at the same time
	_heightMap->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_ONLY);
	_textureWidth = _heightMap->PlatformData->Mips[0].SizeX;
	_textureHeight = _heightMap->PlatformData->Mips[0].SizeY;
	_heightMap->PlatformData->Mips[0].BulkData.Unlock();

	//	Create vertices at positions and offset them accrding to the heighmap
	for (int y = 0; y < _quadsPerSide + 1; y++)
	{
		for (int x = 0; x < _quadsPerSide + 1; x++)
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

	//	Create polygons using KISMET function libraries, easier and more convenient for now
	UKismetProceduralMeshLibrary::CreateGridMeshTriangles((_quadsPerSide + 1), (_quadsPerSide + 1), true, Triangles);
	//	Generate tangents and normals for vertices using triangle data and vertex positions (WARNING: EXTREMELY SLOW ON LARGER MESHES. GENERATION TIME GROWS EXPONENTIALLY THE MORE VERTICES YOU HAVE. A WORKAROUND IS TO CUT THE MESH INTO SMALLER SECTIONS)
	//	!!!!!!!!!
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(vertices, Triangles, UV0, normals, tangents);	//	Comment me to get a faster result, but without vertex normals/tangents
	//	!!!!!!!!!

	//	Register the mesh to the engine and put it into the level
	_mesh->CreateMeshSection(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);

	// Enable collision data for fun
	_mesh->ContainsPhysicsTriMeshData(false);
}

