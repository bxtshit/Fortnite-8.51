struct Fortnite {
	uintptr_t
		UWorld,
		OwningGameInstance,
		RootComponent,
		PersistentLevel,
		AcknowledgedPawn,
		LocalPlayer,
		AFortPlayerController,
		AFortPlayerState,
		CurrentWeapon,
		PlayerCameraManager,
		MeshComponent,
		MovementComponent,
		Actors,
		ActorArray;
	int
		TeamIndex;
	Vector3
		RelativeLocation;
}; std::unique_ptr<Fortnite> fortnite = std::make_unique<Fortnite>();

enum Offsets {
	UWorld = ,
	PersistentLevel = ,
	OwningGameInstance = ,
	RootComponent = ,
	AcknowledgedPawn = ,
	LocalPlayer = ,
	AFortPlayerController = ,
	AFortPlayerState = ,
	CustomTimeDilation = ,
	CurrentWeapon = ,
	ReviveFromDBNOTime = ,
	PlayerCameraManager = ,
	MeshComponent = ,
	BoneArray = ,
	ComponentToWorld = ,
	DefaultFOV = ,
	RelativeLocation = ,
	LastFireTime = ,
	LastFireTimeVerified = ,
	MovementComponent = ,
	TeamIndex = ,
	LastSubmitTime = ,
	LastRenderTimeOnScreen = ,
	WeaponData = 
};