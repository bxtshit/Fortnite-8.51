Vector3 get_bone(DWORD_PTR mesh, int bone_index)
{
	DWORD_PTR bonearray = ctx->read<DWORD_PTR>(mesh + Offsets::BoneArray);
	if(bonearray == NULL) bonearray = ctx->read<DWORD_PTR>(mesh + Offsets::BoneArray + 0x);
	FTransform bone = ctx->read<FTransform>(bonearray + (bone_index * 0x));
	FTransform ComponentToWorld = ctx->read<FTransform>(mesh + Offsets::ComponentToWorld);
	D3DMATRIX Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}	

struct CameraInfo
{
	Vector3 Location;
	Vector3 Rotation;
	float FieldOfView;
};

CameraInfo GetCameraInfo() {
	__int64 ViewStates = ctx->read<__int64>(fortnite->LocalPlayer + 0xC8);
	__int64 ViewReference = ctx->read<__int64>(ViewStates + 8);
	CameraInfo ret;
	ret.Location = ctx->read<Vector3>(ctx->read<__int64>(fortnite->UWorld + ));
	ret.Rotation.x = (asin(ctx->read<float>(ViewReference + )) * (180.0 / M_PI));
	ret.Rotation.y = ctx->read<float>(fortnite->RootComponent + 0);
	ret.FieldOfView = ctx->read<float>(fortnite->LocalPlayer + );
	return ret;
}

Vector3 world_to_screen(Vector3 WorldLocation)
{
	Vector3 Screenlocation = Vector3(0, 0, 0);
	CameraInfo camera = GetCameraInfo();

	D3DMATRIX tempMatrix = Matrix(camera.Rotation);

	Vector3 vAxisX, vAxisY, vAxisZ;

	vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	Vector3 vDelta = WorldLocation - camera.Location;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	float FovAngle = camera.FieldOfView;
	float ScreenCenterX = var->width / 2.0f;
	float ScreenCenterY = var->height / 2.0f;

	Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

	return Screenlocation;
}

float powf_(float _X, float _Y) {
	return (_mm_cvtss_f32(_mm_pow_ps(_mm_set_ss(_X), _mm_set_ss(_Y))));
}
float sqrtf_(float _X) {
	return (_mm_cvtss_f32(_mm_sqrt_ps(_mm_set_ss(_X))));
}
double GetDistance(double x1, double y1, double z1, double x2, double y2) {
	return sqrtf(powf((x2 - x1), 2) + powf_((y2 - y1), 2));
}

void mouse(float x, float y)
{
    Vector3 center(var->width / 2, var->height / 2, 0);
    Vector3 target(0, 0, 0);

    if (x != 0)
    {
        if (x > center.x)
        {
            target.x = -(center.x - x);
            target.x /= (var->smooth + 1);
            if (target.x + center.x > center.x * 2)
                target.x = 0;
        }

        if (x < center.x)
        {
            target.x = x - center.x;
            target.x /= (var->smooth + 1);
            if (target.x + center.x < 0)
                target.x = 0;
        }
    }
    if (y != 0)
    {
        if (y > center.y)
        {
            target.y = -(center.y - y);
            target.y /= (var->smooth + 1);
            if (target.y + center.y > center.y * 2)
                target.y = 0;
        }

        if (y < center.y)
        {
            target.y = y - center.y;
            target.y /= (var->smooth + 1);
            if (target.y + center.y < 0)
                target.y = 0;
        }
    }
    mouse_event(MOUSEEVENTF_MOVE, target.x, target.y, NULL, NULL);
}