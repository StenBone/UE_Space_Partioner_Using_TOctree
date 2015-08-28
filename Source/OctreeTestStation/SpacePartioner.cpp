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

void ASpacePartioner::Initialize(const FBox& NewBounds, const bool& inDrawDebugInfo)
{
	bInitialized = true;
	bDrawDebugInfo = inDrawDebugInfo;
	OctreeData = new FSimpleOctree(NewBounds.GetCenter(), NewBounds.GetExtent().GetMax()); // const FVector & InOrigin, float InExtent
}

void ASpacePartioner::Initialize(const float& inExtent, const bool& inDrawDebugInfo)
{
	bInitialized = true;
	bDrawDebugInfo = inDrawDebugInfo;

	// The Extent is very similar to the radius of a circle
	FVector min = FVector(-inExtent, -inExtent, -inExtent);
	FVector max = FVector(inExtent, inExtent, inExtent);
	FBox NewBounds = FBox(min, max);
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
		int count = 0;

		float max;
		FVector maxExtent;
		FVector center;

		// Iterate over entire Octree
		for (FSimpleOctree::TConstElementBoxIterator<> OctreeIt(*OctreeData, OctreeData->GetRootBounds());
			OctreeIt.HasPendingElements();
			OctreeIt.Advance())
		{
			max = OctreeIt.GetCurrentElement().BoxSphereBounds.BoxExtent.GetMax();
			maxExtent = FVector(max, max, max);
			center = OctreeIt.GetCurrentElement().MyActor->GetActorLocation();

			DrawDebugBox(GetWorld(), center, maxExtent, FColor().Blue, false, 0.0f);
			DrawDebugSphere(GetWorld(), center + maxExtent, 4.0f, 12, FColor().White, false, 0.0f);
			DrawDebugSphere(GetWorld(), center - maxExtent, 4.0f, 12, FColor().White, false, 0.0f);
			count++;
		}
		// UE_LOG(LogTemp, Log, TEXT("%d elements in %s"), count, *OctreeData->GetRootBounds().Extent.ToString());


		DrawOctreeBounds();
	}

	
}

void ASpacePartioner::AddOctreeElement(const FOctreeElement& NewOctreeElement)
{
	check(bInitialized);
	OctreeData->AddElement(NewOctreeElement);
	UE_LOG(LogTemp, Log, TEXT("Added element to Octree."));
}

TArray<FOctreeElement> ASpacePartioner::GetElementsWithinBounds(const FBoxCenterAndExtent& inBoundingBoxQuery)
{
	// Iterating over a region in the octree and storing the elements
	int count = 0;
	TArray<FOctreeElement> octreeElements;

	for (FSimpleOctree::TConstElementBoxIterator<> OctreeIt(*OctreeData, inBoundingBoxQuery);
		OctreeIt.HasPendingElements();
		OctreeIt.Advance())
	{
		octreeElements.Add(OctreeIt.GetCurrentElement());
		count++;
	}
	// UE_LOG(LogTemp, Log, TEXT("%d elements in %s"), count, *inBoundingBoxQuery.Extent.ToString());

	return octreeElements;
}

void ASpacePartioner::DrawOctreeBounds()
{
	FVector extent = this->OctreeData->GetRootBounds().Extent;
	
	float max = extent.GetMax();
	FVector maxExtent = FVector(max, max, max);
	FVector center = this->OctreeData->GetRootBounds().Center;

	DrawDebugBox(GetWorld(), center, maxExtent, FColor().Blue, false, 0.0f);
	DrawDebugSphere(GetWorld(), center + maxExtent, 4.0f, 12, FColor().White, false, 0.0f);
	DrawDebugSphere(GetWorld(), center - maxExtent, 4.0f, 12, FColor().White, false, 0.0f);
}