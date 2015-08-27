// Fill out your copyright notice in the Description page of Project Settings.

#include "OctreeTestStation.h"
#include "SpacePartioner.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASpacePartioner::ASpacePartioner(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, OctreeData(NULL)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OctreeData = new FSimpleOctree(FVector(0.0f, 0.0f, 0.0f), 100.0f); // const FVector & InOrigin, float InExtent
}

void ASpacePartioner::Initialize(const FBox& NewBounds, bool drawDebugInfo)
{
	bInitialized = true;
	bDrawDebugInfo = drawDebugInfo;
	Bounds = NewBounds;
	OctreeData = new FSimpleOctree(NewBounds.GetCenter(), NewBounds.GetExtent().GetMax()); // const FVector & InOrigin, float InExtent
}

// Called when the game starts or when spawned
void ASpacePartioner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpacePartioner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//check(bInitialized);
	//check(bDrawDebugInfo);

	if (bInitialized && bDrawDebugInfo)
	{
		float max = this->Bounds.GetExtent().GetMax();
		FVector extent = FVector(max, max, max);
		DrawDebugBox(GetWorld(), this->Bounds.GetCenter(), extent, FColor().Blue, false, 0.0f);
		DrawDebugSphere(GetWorld(), this->Bounds.GetCenter() + extent, 4.0f, 12, FColor().White, false, 0.0f);
		DrawDebugSphere(GetWorld(), this->Bounds.GetCenter() - extent, 4.0f, 12, FColor().White, false, 0.0f);
	}

	
}

void ASpacePartioner::AddOctreeElement(const FOctreeElement& NewOctreeElement)
{
	check(bInitialized);
	OctreeData->AddElement(NewOctreeElement);
}

void ASpacePartioner::ApplyWorldOffset(const FVector& InOffset)
{
	Bounds.Min += InOffset;
	Bounds.Max += InOffset;
	OctreeData->ApplyOffset(InOffset);
}