#include "raylib.h"
#include <cstdio>

#include "imgui.h"
#include "implot.h"
#include "rlImGui.h"

#include <raymath.h>
#include <rcamera.h>

#include "math.hpp"
#include "utility.hpp"

namespace config
{
	// window settings
	constexpr int window_width = 1280;
	constexpr int window_height = 800;
	// camera settings
	constexpr float camera_fov = 45.0f;
	constexpr float camera_near = 0.1f;
	constexpr float camera_far = 100.0f;
	// Colors 
	constexpr Color background_color = BLACK;
	constexpr Color text_color = WHITE;
	constexpr Color debug_drawing_color = RED;

	constexpr float CAMERA_PAN_SENSITIVITY = 0.1f;
	constexpr float CAMERA_ZOOM_SENSITIVITY = 100.0f;
	constexpr float GRID_SPACING = 1.0f;
	constexpr int GRID_SLICES = 100;
	constexpr float GRID_OPACITY = 0.5f;
}

// data used by the application
namespace {
	RenderTexture2D target;
	ImVec2 render_texture_size = { 1280, 800 };
	ImVec2 render_offset = { 0, 0 };
	bool bHoveringViewport = false;

	Camera camera = { 0 };
	vec3 grid_position = { 0, 0, 0 };
	float camera_theta = 3.14f / 4.0f;
	float camera_phi = 3.14f / 6.0f;
	float camera_distance = 10.0f;
	int selected_particle_id = -1;

	bool bRun = true;
	bool bOpenImGuiDemo = true;
	bool bDrawGrid = true;
	bool bDrawAxes = true;

	bool bDrawCube = true;
	float cube_size = 2.0f;
	ImColor cube_color = ImColor(255, 0, 0, 255);
}

void resize_render_texture()
{
	UnloadRenderTexture(target);
	printf("Old Render Texture Size %dx%d\n", target.texture.width, target.texture.height);
	target = LoadRenderTexture((int)(render_texture_size.x), (int)(render_texture_size.y));
	printf("New Render Texture Size %dx%d\n", target.texture.width, target.texture.height);
}

void draw_debug_panel()
{
	ImGui::SeparatorText("Control");
	ImGui::Checkbox("Draw Grid", &bDrawGrid);
	ImGui::Checkbox("Draw Axes", &bDrawAxes);
	ImGui::Separator();
	ImGui::Checkbox("Draw Cube", &bDrawCube);
	if (bDrawCube)
	{
		ImGui::DragFloat("Cube size", &cube_size, 0.1f);
		ImGui::ColorEdit4("Cube Color", &cube_color.Value.x);
	}
	ImGui::Separator();
	ImGui::SeparatorText("Info");
	ImGui::Text("Render size %f %f", render_texture_size.x, render_texture_size.y);
	ImVec2 mouse_pos = ImGui::GetMousePos();
	ImGui::Text("Mouse Pos %f %f", mouse_pos.x, mouse_pos.y);
	ImGui::Text("Mouse Hovering Viewport %d", bHoveringViewport);
	ImGui::Text("Camera Position %f %f %f", camera.position.x, camera.position.y, camera.position.z);
}

void update_camera()
{
	// camera orbit controls, only when the right mouse button is pressed
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) and bHoveringViewport)
	{
		const Vector2 mouseDelta = GetMouseDelta();
		camera_theta += mouseDelta.x * config::CAMERA_PAN_SENSITIVITY * GetFrameTime();
		camera_phi -= mouseDelta.y * config::CAMERA_PAN_SENSITIVITY * GetFrameTime();
	}
	// camera zoom controls
	if (bHoveringViewport)
	{
		camera_distance -= GetMouseWheelMove() * GetFrameTime() * config::CAMERA_ZOOM_SENSITIVITY;
	}

	vec3 pos = { camera_distance * sinf(camera_theta) * cosf(camera_phi),
					camera_distance * sinf(camera_phi),
					camera_distance * cosf(camera_theta) * cosf(camera_phi) };
	camera.position = pos;
	camera.target = { 0, 0, 0 };
}

void update()
{
	update_camera();
	// if you need the mouse position you need to adjust for the render panel position
	auto mouse_pos = GetMousePosition();
	mouse_pos.x -= render_offset.x;
	mouse_pos.y -= render_offset.y;
}

void render()
{
	BeginTextureMode(target);
		// Setup the backbuffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);
		BeginMode3D(camera);
			if (bDrawGrid)
			{
				DrawGrid(grid_position, config::GRID_SLICES, config::GRID_SPACING, config::GRID_OPACITY, GRAY);
			}
			if (bDrawAxes)
			{
				Draw3DAxes(grid_position);
			}
			if (bDrawCube)
			{
				DrawCube({ 0, 0, 0 }, cube_size, cube_size, cube_size, from_ImColor(cube_color));
			}
		EndMode3D();
	EndTextureMode();

	BeginDrawing();
		ClearBackground(BLACK);     // Clear screen background
		// start ImGui Conent
		rlImGuiBegin();

		Rectangle sourceRec = { 0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height };
		Rectangle destRec = { 0, 0, (float)target.texture.width, -(float)target.texture.height };

#ifdef IMGUI_HAS_DOCK
		ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_PassthruCentralNode); // set ImGuiDockNodeFlags_PassthruCentralNode so that we can see the raylib contents behind the dockspace
#endif
		ImGui::ShowDemoWindow(&bOpenImGuiDemo);
		
		// The 3D viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		bHoveringViewport = ImGui::IsWindowHovered();
		// check if the viewport size has changed
		ImVec2 size = ImGui::GetContentRegionAvail();
		if ((size.x != render_texture_size.x) or (size.y != render_texture_size.y))
		{
			render_texture_size = size;
			resize_render_texture();
		}
		ImGui::Image(reinterpret_cast<ImTextureID>(&target.texture), render_texture_size, ImVec2{ 0, 0 }, ImVec2{ 1, -1 });
		render_offset = ImGui::GetWindowPos();
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Debug");
		draw_debug_panel();
		ImGui::End();
		// show a simple menu bar
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Quit"))
					bRun = false;

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// end ImGui Content
		rlImGuiEnd();
	// end the frame and get ready for the next one  (display frame, poll input, etc...)
	EndDrawing();
}

int main()
{
	// Tell the window to use vysnc and work on high DPI displays
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(config::window_width, config::window_height, "Hello Raylib and ImGui");
	// Render texture initialization
	target = LoadRenderTexture((int)render_texture_size.x, (int)render_texture_size.y);

	// Define the camera to look into our 3d world
	camera.position = { 10.0f, 10.0f, 10.0f };			// Camera position
	camera.target = { 0.0f, 0.0f, 0.0f };				// Camera looking at point
	camera.up = { 0.0f, 1.0f, 0.0f };					// Camera up vector (rotation towards target)
	camera.fovy = 50.0f;                                // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

	SetTargetFPS(60);									// Set our game to run at 60 frames-per-second
	rlImGuiSetup(true);
	ImPlot::CreateContext();

#ifdef IMGUI_HAS_DOCK
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif
	// main update loop
	while (!WindowShouldClose() && bRun)				// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		update();
		render();
	}
	// cleanup
	ImPlot::DestroyContext();
	rlImGuiShutdown();
	// destory the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}