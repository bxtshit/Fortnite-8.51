typedef struct Actors {
	uint64_t Actor;
	uint64_t MeshComponent;
	uint64_t AFortPlayerState;
	int TeamIndex;
}Actors;
std::vector<Actors> ActorList;

auto cache() {
	for (;;) {
		std::vector<Actors> ActorTempList;
		fortnite->UWorld = ctx->read<uint64_t>(ctx->image_base + Offsets::UWorld);
		fortnite->OwningGameInstance = ctx->read<uint64_t>(fortnite->UWorld + Offsets::OwningGameInstance);
		fortnite->PersistentLevel = ctx->read<uint64_t>(fortnite->UWorld + 0x30);
		auto LocalPlayers = ctx->read<uint64_t>(fortnite->OwningGameInstance + Offsets::LocalPlayer); fortnite->LocalPlayer = ctx->read<DWORD_PTR>(LocalPlayers);
		fortnite->AFortPlayerController = ctx->read<uint64_t>(fortnite->LocalPlayer + Offsets::AFortPlayerController);
		fortnite->AcknowledgedPawn = ctx->read<uint64_t>(fortnite->AFortPlayerController + Offsets::AcknowledgedPawn);
		fortnite->RootComponent = ctx->read<uint64_t>(fortnite->AcknowledgedPawn + Offsets::RootComponent);
		fortnite->AFortPlayerState = ctx->read<uint64_t>(fortnite->AcknowledgedPawn + Offsets::AFortPlayerState);
		fortnite->PlayerCameraManager = ctx->read<uint64_t>(fortnite->AFortPlayerController + Offsets::PlayerCameraManager);
		fortnite->CurrentWeapon = ctx->read<uint64_t>(fortnite->AcknowledgedPawn + Offsets::CurrentWeapon);
		fortnite->RelativeLocation = ctx->read<Vector3>(fortnite->RootComponent + Offsets::RelativeLocation);
		fortnite->MeshComponent = ctx->read<uint64_t>(fortnite->AcknowledgedPawn + Offsets::MeshComponent);
		fortnite->MovementComponent = ctx->read<uint64_t>(fortnite->AcknowledgedPawn + Offsets::MovementComponent);
		fortnite->ActorArray = ctx->read<int>(fortnite->PersistentLevel + 0x);
		fortnite->Actors = ctx->read<uintptr_t>(fortnite->PersistentLevel + 0x);
		std::cout << "ActorArray Size -> " << fortnite->ActorArray << std::endl;
		for (int i = 0; i < fortnite->ActorArray; ++i) {
			auto current_actor = ctx->read<uint64_t>(fortnite->Actors + i * 0x);
			if (ctx->read<float>(current_actor + Offsets::ReviveFromDBNOTime) == 10)
			{
				Actors FortniteActor{};
				FortniteActor.Actor = current_actor;
				FortniteActor.MeshComponent = ctx->read<uintptr_t>(current_actor + Offsets::MeshComponent);
				FortniteActor.AFortPlayerState = ctx->read<uint64_t>(current_actor + Offsets::AFortPlayerState);
				FortniteActor.TeamIndex = ctx->read<uintptr_t>(FortniteActor.AFortPlayerState + Offsets::TeamIndex);
				ActorTempList.push_back(FortniteActor);
			}
		}
		ActorList.clear();
		ActorList = ActorTempList;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}