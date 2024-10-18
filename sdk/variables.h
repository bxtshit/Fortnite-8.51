struct Var {
	bool
		aimbot = 1,
		render_menu = 1,
		render_fov = 1,
		no_recoil = 0,
		rapid_fire = 0,
		player_speed = 0,
		fov_changer = 1;
	int
		smooth,
		speed_value,
		selected_option,
		width, height;
	float
		fov_value = 100,
		fov_changer_value = 100;
}; std::unique_ptr<Var> var = std::make_unique<Var>();