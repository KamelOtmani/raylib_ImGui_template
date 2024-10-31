#include "math.hpp"
#include "raylib.h"
#include "rlgl.h"           // OpenGL abstraction layer to OpenGL 1.1, 2.1, 3.3+ or ES2

// Draw a grid centered at positions
void DrawGrid(vec3 position, int slices, float spacing, float opacity = 0.5f, Color color = GRAY)
{
	int halfSlices = slices / 2;

	rlBegin(RL_LINES);
	for (int i = -halfSlices; i <= halfSlices; i++)
	{
		if (i == 0)
		{
			rlColor4f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, opacity);
		}
		else
		{
			rlColor4f(color.r / 510.0f, color.g / 510.0f, color.b / 510.0f, opacity);
		}
		vec3 offset = position;

		rlVertex3f(position.x + (float)i * spacing, position.y, position.z + (float)-halfSlices * spacing);
		rlVertex3f(position.x + (float)i * spacing, position.y, position.z + (float)halfSlices * spacing);

		rlVertex3f(position.x + (float)-halfSlices * spacing, position.x + 0.0f, position.z + (float)i * spacing);
		rlVertex3f(position.x + (float)halfSlices * spacing, position.x + 0.0f, position.z + (float)i * spacing);
	}
	rlEnd();
}

void Draw3DAxes(vec3 position, float scale = 1.0f)
{
	constexpr int nSides = 6;
	const float r = scale * 0.05f;

	DrawCylinderEx({ position.x, position.y, position.z }, { position.x + scale, position.y, position.z }, r, r, nSides, RED);
	DrawCylinderEx({ position.x, position.y, position.z }, { position.x , position.y + scale, position.z }, r, r, nSides, GREEN);
	DrawCylinderEx({ position.x, position.y, position.z }, { position.x , position.y, position.z + scale }, r, r, nSides, BLUE);
}

inline Color from_ImColor(const ImColor& color)
{
	return Color{
		unsigned char(color.Value.x * 255.0f),
		unsigned char(color.Value.y * 255.0f),
		unsigned char(color.Value.z * 255.0f),
		unsigned char(color.Value.w * 255.0f) };
}