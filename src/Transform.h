#pragma once
#include "../headers/glm/glm.hpp"
#include "../headers/glm/gtx/quaternion.hpp"
#include "../headers/glm/gtc/quaternion.hpp"
#include "../headers/glm/glm.hpp"
#include "../headers/glm/gtc/type_ptr.hpp"
#include "SceneElement.h"
#include <algorithm>
#include <vector>
#include <array>


class Transform{
public:
		glm::quat mRotation;
		glm::vec3 mTranslation;
		glm::vec3 mScale;

		Transform(const Transform& other){
			mRotation = other.mRotation;
			mTranslation = other.mTranslation;
			mScale = other.mScale;
		}
		Transform() { 
				mScale.x = 4;
				mScale.y = 4;
				mScale.z = 4;
		}
		Transform& operator=(Transform other)
		{
			mRotation = other.mRotation;
			mTranslation = other.mTranslation;
			mScale = other.mScale;
			return *this;
		}
};


//abandoné esta idea porque la memoria ya dejaría de ser contigua en cuanto a arreglos de indices y vertices
//y eso complica las cosas
//class Triangle {
//	array<TType, 3> indexes;
//	glm::vec3 normal;
//	glm::vec3 centroid;
//};
//
//class Vertex {
//	glm::vec3 val;
//};
//
//class VertexMesh : public Vertex {
//	glm::vec3 normal;
//};
//
//class Geometry{
//public:
//	vector<Triangle> faces;
//	vector<VertexMesh> vertexes;
//
//	vec3 minsBB{ 999999,999999,9999999 };
//	vec3 maxsBB{ -999999,-999999,-9999999 };
//};