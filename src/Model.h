#pragma once
#include "../headers/glfw3.h"
#include "../headers/glm/glm.hpp"
#include "../headers/glm/gtx/quaternion.hpp"
#include "../headers/glm/gtc/quaternion.hpp"
#include "../headers/glm/glm.hpp"

#include "Transform.h"
#include "SceneElement.h"
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>


class CModel
{
protected:

public:

	string path;
	Transform transform;
	ObjectRenderer* renderer;

	CModel(ObjectRenderer* _renderer) { renderer = _renderer; }
	CModel();
	~CModel();
	

	void TakeCustomization(CModel&);



};