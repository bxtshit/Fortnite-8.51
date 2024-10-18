int mx = 20, my = 20;
int item_offset = 50;

auto render(int width, int height) {
	if (GetAsyncKeyState(VK_RSHIFT) & 1)
		var->render_menu = !var->render_menu;
	
	if (var->render_fov)
		d2d::EmptyCircle(var->width / 2, var->height / 2, var->fov_value, 2, 255, 0, 0, 1);

	actorloop();
}