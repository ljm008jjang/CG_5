#pragma once
#include <glm/glm.hpp>

using namespace glm;

class Light {
public:
	Light(vec3 position) : pos(position), illumination(1) {};

	vec3 pos;

	float illumination;
};