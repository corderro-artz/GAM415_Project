// ------------------------------------------------------------------------------------------------------
// ------ MODULE 4 CODE ---------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------


#include "PerlinProcTerrain.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

// Sets default values
APerlinProcTerrain::APerlinProcTerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>("Procedural Mesh");
	ProcMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void APerlinProcTerrain::BeginPlay()
{
	Super::BeginPlay();

	CreateVertices();
	CreateTriangles();
	ProcMesh->CreateMeshSection(sectionID, Vertices, Triangles, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>(), true);
	ProcMesh->SetMaterial(0, Mat);
}

// Called every frame
void APerlinProcTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APerlinProcTerrain::AlterMesh(FVector impactPoint)
{
	for (int i = 0; i < Vertices.Num(); i++)
	{
		FVector tempVector = impactPoint - this->GetActorLocation();
		if (FVector(Vertices[i] - tempVector).Size() < Radius)
		{
			Vertices[i] = Vertices[i] - Depth;
			ProcMesh->UpdateMeshSection(sectionID, Vertices, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>());
		}
	}
}

/*void APerlinProcTerrain::AlterMesh(FVector impactPoint)
{
	bool bVertexChanged = false;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("AlterMesh called!"));

	for (int i = 0; i < Vertices.Num(); i++)
	{
		FVector vertexWorld = GetActorLocation() + Vertices[i];
		float dist = FVector(vertexWorld - impactPoint).Size();
		if (dist < Radius)
		{
			FString msg = FString::Printf(TEXT("Vertex %d before: Z=%f"), i, Vertices[i].Z);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, msg);

			Vertices[i].Z -= Depth.Z; // Only change Z
			bVertexChanged = true;

			msg = FString::Printf(TEXT("Vertex %d after: Z=%f"), i, Vertices[i].Z);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, msg);
		}
	}

	if (bVertexChanged)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Vertices changed, updating mesh section!"));
		ProcMesh->UpdateMeshSection(sectionID, Vertices, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, TEXT("No vertices changed!"));
	}
}*/


void APerlinProcTerrain::CreateVertices()
{
	for (int x = 0; x <= XSize; x++)
	{
		for (int y = 0; y <= YSize; y++)
		{
			float z = FMath::PerlinNoise2D(FVector2D(x * NoiseScale + 0.1, y * NoiseScale + 0.1)) * ZMultiplier;
			GEngine->AddOnScreenDebugMessage(-1, 999.0f, FColor::Yellow, FString::Printf(TEXT("Z %f"), z));
			Vertices.Add(FVector(x * Scale, y * Scale, z));
			UV0.Add(FVector2D(x * UVScale, y * UVScale));
		}
	}
}

void APerlinProcTerrain::CreateTriangles()
{

	int Vertex = 0;

	for (int X = 0; X < XSize; X++)
	{
		for (int Y = 0; Y < YSize; Y++)
		{
			Triangles.Add(Vertex);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 1);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 2);
			Triangles.Add(Vertex + YSize + 1);
			Vertex++;
		}
		Vertex++;
	}
}

// ------------------------------------------------------------------------------------------------------

