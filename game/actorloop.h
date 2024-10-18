auto actorloop() {
    float closest_dist = FLT_MAX;
    uintptr_t closest_pawn = NULL;

    if(GetAsyncKeyState(VK_RBUTTON) && fortnite->CurrentWeapon)
        ctx->write<float>(fortnite->CurrentWeapon + Offsets::CustomTimeDilation, 100);
    else
        ctx->write<float>(fortnite->CurrentWeapon + Offsets::CustomTimeDilation, 1);

    for (Actors Actor : ActorList) {
        Vector3 Base = world_to_screen(get_bone(Actor.MeshComponent, 0));
        Vector3 Head = world_to_screen(get_bone(Actor.MeshComponent, 66));
        Vector3 Neck = world_to_screen(get_bone(Actor.MeshComponent, 64));
        Vector3 Pelvis = world_to_screen(get_bone(Actor.MeshComponent, 2));
        Vector3 LShoulder = world_to_screen(get_bone(Actor.MeshComponent, 9));
        Vector3 LElbow = world_to_screen(get_bone(Actor.MeshComponent, 10));
        Vector3 LHand = world_to_screen(get_bone(Actor.MeshComponent, 11));
        Vector3 RShoulder = world_to_screen(get_bone(Actor.MeshComponent, 37));
        Vector3 RElbow = world_to_screen(get_bone(Actor.MeshComponent, 38));
        Vector3 RHand = world_to_screen(get_bone(Actor.MeshComponent, 39));
        Vector3 RThigh = world_to_screen(get_bone(Actor.MeshComponent, 74));
        Vector3 RKnee = world_to_screen(get_bone(Actor.MeshComponent, 75));
        Vector3 RFoot = world_to_screen(get_bone(Actor.MeshComponent, 76));
        Vector3 LThight = world_to_screen(get_bone(Actor.MeshComponent, 67));
        Vector3 LKnee = world_to_screen(get_bone(Actor.MeshComponent, 68));
        Vector3 LFoot = world_to_screen(get_bone(Actor.MeshComponent, 69));

        d2d::Line(Head.x, Head.y, Neck.x, Neck.y, 1, 255, 0, 0, 1);
        d2d::Line(Neck.x, Neck.y, Pelvis.x, Pelvis.y, 1, 255, 0, 0, 1);
        d2d::Line(Neck.x, Neck.y, LShoulder.x, LShoulder.y, 1, 255, 0, 0, 1);
        d2d::Line(LShoulder.x, LShoulder.y, LElbow.x, LElbow.y, 1, 255, 0, 0, 1);
        d2d::Line(LElbow.x, LElbow.y, LHand.x, LHand.y, 1, 255, 0, 0, 1);
        d2d::Line(Neck.x, Neck.y, RShoulder.x, RShoulder.y, 1, 255, 0, 0, 1);
        d2d::Line(RShoulder.x, RShoulder.y, RElbow.x, RElbow.y, 1, 255, 0, 0, 1);
        d2d::Line(RElbow.x, RElbow.y, RHand.x, RHand.y, 1, 255, 0, 0, 1);
        d2d::Line(Pelvis.x, Pelvis.y, RThigh.x, RThigh.y, 1, 255, 0, 0, 1);
        d2d::Line(RThigh.x, RThigh.y, RKnee.x, RKnee.y, 1, 255, 0, 0, 1);
        d2d::Line(RKnee.x, RKnee.y, RFoot.x, RFoot.y, 1, 255, 0, 0, 1);
        d2d::Line(Pelvis.x, Pelvis.y, LThight.x, LThight.y, 1, 255, 0, 0, 1);
        d2d::Line(LThight.x, LThight.y, LKnee.x, LKnee.y, 1, 255, 0, 0, 1);
        d2d::Line(LKnee.x, LKnee.y, LFoot.x, LFoot.y, 1, 255, 0, 0, 1);

        d2d::Line(var->width / 2, var->height - 30, Base.x, Base.y, 1, 255, 0, 0, 1);

        auto dx = Head.x - (var->width / 2);
        auto dy = Head.y - (var->height / 2);
        auto dist = sqrtf(dx * dx + dy * dy);

        if (dist < var->fov_value && dist < closest_dist) {
            closest_dist = dist;
            closest_pawn = Actor.Actor;
        }
    }
    if (closest_pawn)
    {
        if (var->aimbot)
        {
            auto AimbotMesh = ctx->read<uint64_t>(closest_pawn + Offsets::MeshComponent);

            Vector3 CurrentHitbox = world_to_screen(get_bone(AimbotMesh, 66));

            if (CurrentHitbox.x != 0 || CurrentHitbox.y != 0 || CurrentHitbox.z != 0)
            {
                if ((GetDistance(CurrentHitbox.x, CurrentHitbox.y, CurrentHitbox.z, var->width / 2, var->height / 2) <= var->fov_value))
                {
                    if (GetAsyncKeyState(VK_RBUTTON)) mouse(CurrentHitbox.x, CurrentHitbox.y);
                }
            }
        }
    }
    else
    {
        closest_dist = FLT_MAX;
        closest_pawn = NULL;
    }
}