

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

	plate_on = GetWorld()->GetFirstPlayerController()->GetPawn();
	owner = GetOwner();
	
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (pressure_plate->IsOverlappingActor(plate_on)) {
		OpenDoor();
		time_door_open = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - time_door_open > door_close_delay) {
		CloseDoor();
	}
	
}

void UOpenDoor::OpenDoor()
{
	owner->SetActorRotation(FRotator(0.f, open_angle, 0.f));
	owner->SetActorEnableCollision(false);
}

void UOpenDoor::CloseDoor()
{
	owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
	owner->SetActorEnableCollision(true);
}