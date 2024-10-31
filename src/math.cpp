#include "math.hpp"

void Bbox::draw(Color color) const
{
	DrawBoundingBox(BoundingBox{ min,max }, color);
}

Bbox Domain::get_bbox() const
{
	// get the bounding box of the domain
	return Bbox{
		{ -width / 2.0f, -height / 2.0f, -depth / 2.0f },
		{ width / 2.0f, height / 2.0f, depth / 2.0f }
	};
}

vec3 Domain::get_extents() const
{
	// get the extents of the domain
	return { width/2.0f , height/2.0f , depth/2.0f };
}

float Domain::get_volume() const
{
	return width * height * depth;
}

void Domain::draw(Color color) const
{
	// draw the domain
	get_bbox().draw(color);
}

float length(const vec3& v)
{
	return v.length();
}

vec3 norm(const vec3& v)
{
	return v.norm();
}

