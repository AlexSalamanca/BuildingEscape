

#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsComponent();
	SetupInputComponent();
}




// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector player_viewpoint_location;
	FRotator player_viewpoint_rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT player_viewpoint_location,
		OUT player_viewpoint_rotation
	);

	FVector line_trace_end = player_viewpoint_location + player_viewpoint_rotation.Vector() * reach;

	if (physic_handle->GetGrabbedComponent()) {
		physic_handle->SetTargetLocation(line_trace_end);
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"))
		
	auto hit_result = ReturnFirstPhysicBody();
	auto component_to_grab = hit_result.GetComponent();
	auto actor_hit = hit_result.GetActor();

	if (actor_hit) {
		physic_handle->GrabComponent(
			component_to_grab,
			NAME_None,
			component_to_grab->GetOwner()->GetActorLocation(),
			true
		);
	}

	
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab released"))

	physic_handle->ReleaseComponent();
}

void UGrabber::FindPhysicsComponent()
{
	physic_handle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (physic_handle) {

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}
}

void UGrabber::SetupInputComponent()
{
	input_component = GetOwner()->FindComponentByClass<UInputComponent>();

	if (input_component) {
		input_component->BindAction("Grabber", IE_Pressed, this, &UGrabber::Grab);
		input_component->BindAction("Grabber", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName())

	}
}

const FHitResult UGrabber::ReturnFirstPhysicBody()
{
	FVector player_viewpoint_location;
	FRotator player_viewpoint_rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT player_viewpoint_location,
		OUT player_viewpoint_rotation
	);

	FVector line_trace_end = player_viewpoint_location + player_viewpoint_rotation.Vector() * reach;
	FHitResult hit;
	FCollisionQueryParams trace_parameters(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		player_viewpoint_location,
		line_trace_end,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		trace_parameters
	);

	AActor* actor_hit = hit.GetActor();

	if (actor_hit) {
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *actor_hit->GetName())
	}

	return hit;
}
