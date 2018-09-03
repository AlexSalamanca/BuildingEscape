

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
	
	if (!physic_handle) { return; }

	if (physic_handle->GetGrabbedComponent()) {
		physic_handle->SetTargetLocation(GetReachLineEnd());
	}
}

void UGrabber::Grab() {
	auto hit_result = ReturnFirstPhysicBody();
	auto component_to_grab = hit_result.GetComponent(); //get the mesh in our case
	auto actor_hit = hit_result.GetActor();

	if (!physic_handle) { return; }

	if (actor_hit) {
		physic_handle->GrabComponent(
			component_to_grab,
			NAME_None,//No bones needed
			component_to_grab->GetOwner()->GetActorLocation(),
			true//allow rotation
		);
	}

	
}

void UGrabber::Release() {
	if (!physic_handle) { return; }

	physic_handle->ReleaseComponent();
}

void UGrabber::FindPhysicsComponent()
{
	physic_handle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (physic_handle == nullptr) {
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
	
	FHitResult hit_result;
	FCollisionQueryParams trace_parameters(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT hit_result,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		trace_parameters
	);

	return hit_result;
}

FVector UGrabber::GetReachLineEnd() {
	FVector player_viewpoint_location;
	FRotator player_viewpoint_rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT player_viewpoint_location,
		OUT player_viewpoint_rotation
	);

	return player_viewpoint_location + player_viewpoint_rotation.Vector() * reach;
}

FVector UGrabber::GetReachLineStart()
{
	FVector player_viewpoint_location;
	FRotator player_viewpoint_rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT player_viewpoint_location,
		OUT player_viewpoint_rotation
	);

	return player_viewpoint_location;
}
