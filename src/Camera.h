#pragma once
#include "Transform.h"

enum class CameraType {
	ORTHO,
	PERSPECTIVE,
};


class Camera
{
public:
	glm::vec2 near_far;
	Transform transform;
};