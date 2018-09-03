

#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	owner = GetOwner();

	if (!pressure_plate) {
		UE_LOG(LogTemp, Error, TEXT("No pressure plate found"))
	}

	if (!owner) {
		UE_LOG(LogTemp, Error, TEXT("No ower plate found"))
	}
	
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorOnPlate() > trigger_mass) {
		OnOpen.Broadcast();
		owner->SetActorEnableCollision(false);
	}
	else {
		OnClose.Broadcast();
		owner->SetActorEnableCollision(true);
	}
	
}

float UOpenDoor::GetTotalMassOfActorOnPlate() {
	float total_mass = 0.f;
	TArray<AActor*> overlapping_actors;

	if (!pressure_plate) { return total_mass; }

	pressure_plate->GetOverlappingActors(OUT overlapping_actors);

	for (const auto& actor : overlapping_actors) {
		total_mass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return total_mass;
}