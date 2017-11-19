// Fill out your copyright notice in the Description page of Project Settings.

#include "HouseMarque.h"

// Sets default values
AHouseMarque::AHouseMarque()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//	Initialize and construct the _rootSprite component
	_rootSprite = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root Sprite"));
	//	Set sprite with texutre from path
	_rootSprite->SetSprite(LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EditorResources/S_Actor.S_Actor")));
	//	Initialize the procedural mesh component
	_pmc = CreateDefaultSubobject<UHouseMarque_PMC>(TEXT("pmc)"));
}

// Called when the game starts or when spawned
void AHouseMarque::BeginPlay()
{
	Super::BeginPlay();	
	//	Set the heightmap in the pmc
	_pmc->SetHeightMap(_heightMap);
}

// Called every frame
void AHouseMarque::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

