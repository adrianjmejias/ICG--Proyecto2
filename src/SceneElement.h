#pragma once
//#include "../headers/GL/glew.h"
//#define GLFW_INCLUDE_GLU

#include "../headers/glfw3.h"
#include "../headers/glm/glm.hpp"
#include "../headers/glm/gtx/quaternion.hpp"
#include "../headers/glm/gtc/quaternion.hpp"
#include "../headers/glm/glm.hpp"
#include "../headers/glm/gtc/type_ptr.hpp"


#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>

using std::vector;
using std::array;
using std::max;
using std::string;
using glm::vec3;
using glm::quat;
using glm::cross;
using glm::normalize;

typedef	GLushort TType;

#define glColorFunc glColor4fv
#define ColorToFloatArr(color) glm::value_ptr(color) 
typedef glm::vec4 Color;
typedef glm::vec4 ColorM;
enum class MaterialType {
	EMERALD,
	JADE,
	OBSIDIAN,
	PEARL,
	RUBY,
	TURQUOISE,
	BRASS,
	BRONZE,
	CHROME,
	COPPER,
	GOLD,
	SILVER,
	BLACK_PLASTIC,
	CYAN_PLASTIC,
	GREEN_PLASTIC,
	RED_PLASTIC,
	WHITE_PLASTIC,
	YELLOW_PLASTIC,
	BLACK_RUBBER,
	CYAN_RUBBER,
	GREEN_RUBBER,
	RED_RUBBER,
	WHITE_RUBBER,
	YELLOW_RUBBER,
	CUSTOM
};

enum class DisplayType {
	GL,
	VBO,
	LIST,
	VP
};

class Material {
public:
	ColorM ambient{ 0.2, 0.2, 0.2, 1.0 };
	ColorM specular{ 0.0, 0.0, 0.0, 1.0 };
	ColorM diffuse{ 0.8, 0.8, 0.8, 1.0 };
	ColorM emisor{ 0.0, 0.0, 0.0, 1.0 };
	float shininess{ 0.0 };
	bool isEmisor = false;
	Material() {}
	//Material(Color, Color, Color, float);
	//static Material GetMaterial(MaterialType);
	void ApplyMaterial() {
		glEnable(GL_COLOR_MATERIAL);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(ambient));
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(specular));
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(diffuse));
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
		if (isEmisor) {
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(emisor));
		}
		else {
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(ColorM()));

		}

	}
	void RemoveMaterial() {
		glDisable(GL_COLOR_MATERIAL);
	}

	Material::Material(ColorM _ambient, ColorM _diffuse, ColorM _specular, float _shiny) {
		ambient = _ambient;
		diffuse = _diffuse;
		specular = _specular;
		shininess = _shiny;
	}

	static Material GetMaterial(MaterialType materialType) {
		switch (materialType) {
		case MaterialType::EMERALD: return Material(ColorM(0.0215f, 0.1745f, 0.0215f, 1.0f), ColorM(0.07568f, 0.61424f, 0.07568f, 1.0f), ColorM(0.633f, 0.727811f, 0.633f, 1.0f), 0.6f);
		case MaterialType::JADE: return Material(ColorM(0.135f, 0.2225f, 0.1575f, 1.0f), ColorM(0.54f, 0.89f, 0.63f, 1.0f), ColorM(0.316228f, 0.316228f, 0.316228f, 1.0f), 0.1f);
		case MaterialType::OBSIDIAN: return Material(ColorM(0.05375f, 0.05f, 0.06625f, 1.0f), ColorM(0.18275f, 0.17f, 0.22525f, 1.0f), ColorM(0.332741f, 0.328634f, 0.346435f, 1.0f), 0.3f);
		case MaterialType::PEARL: return Material(ColorM(0.25f, 0.20725f, 0.20725f, 1.0f), ColorM(1.0f, 0.829f, 0.829f, 1.0f), ColorM(0.296648f, 0.296648f, 0.296648f, 1.0f), 0.088f);
		case MaterialType::RUBY: return Material(ColorM(0.1745f, 0.01175f, 0.01175f, 1.0f), ColorM(0.61424f, 0.04136f, 0.04136f, 1.0f), ColorM(0.727811f, 0.626959f, 0.626959f, 1.0f), 0.6f);
		case MaterialType::TURQUOISE: return Material(ColorM(0.1f, 0.18725f, 0.1745f, 1.0f), ColorM(0.396f, 0.74151f, 0.69102f, 1.0f), ColorM(0.297254f, 0.30829f, 0.306678f, 1.0f), 0.1f);
		case MaterialType::BRASS: return Material(ColorM(0.329412f, 0.223529f, 0.027451f, 1.0f), ColorM(0.780392f, 0.568627f, 0.113725f, 1.0f), ColorM(0.992157f, 0.941176f, 0.807843f, 1.0f), 0.21794872f);
		case MaterialType::BRONZE: return Material(ColorM(0.2125f, 0.1275f, 0.054f, 1.0f), ColorM(0.714f, 0.4284f, 0.18144f, 1.0f), ColorM(0.393548f, 0.271906f, 0.166721f, 1.0f), 0.2f);
		case MaterialType::CHROME: return Material(ColorM(0.25f, 0.25f, 0.25f, 1.0f), ColorM(0.4f, 0.4f, 0.4f, 1.0f), ColorM(0.774597f, 0.774597f, 0.774597f, 1.0f), 0.6f);
		case MaterialType::COPPER: return Material(ColorM(0.19125f, 0.0735f, 0.0225f, 1.0f), ColorM(0.7038f, 0.27048f, 0.0828f, 1.0f), ColorM(0.256777f, 0.137622f, 0.086014f, 1.0f), 0.1f);
		case MaterialType::GOLD: return Material(ColorM(0.24725f, 0.1995f, 0.0745f, 1.0f), ColorM(0.75164f, 0.60648f, 0.22648f, 1.0f), ColorM(0.628281f, 0.555802f, 0.366065f, 1.0f), 0.4f);
		case MaterialType::CUSTOM:
		case MaterialType::SILVER: return Material(ColorM(0.19225f, 0.19225f, 0.19225f, 1.0f), ColorM(0.50754f, 0.50754f, 0.50754f, 1.0f), ColorM(0.508273f, 0.508273f, 0.508273f, 1.0f), 0.4f);
		case MaterialType::BLACK_PLASTIC: return Material(ColorM(0.0f, 0.0f, 0.0f, 1.0f), ColorM(0.01f, 0.01f, 0.01f, 1.0f), ColorM(0.50f, 0.50f, 0.50f, 1.0f), 0.25f);
		case MaterialType::CYAN_PLASTIC: return Material(ColorM(0.0f, 0.1f, 0.06f, 1.0f), ColorM(0.0f, 0.50980392f, 0.50980392f, 1.0f), ColorM(0.50196078f, 0.50196078f, 0.50196078f, 1.0f), 0.25f);
		case MaterialType::GREEN_PLASTIC: return Material(ColorM(0.0f, 0.0f, 0.0f, 1.0f), ColorM(0.1f, 0.35f, 0.1f, 1.0f), ColorM(0.45f, 0.55f, 0.45f, 1.0f), 0.25f);
		case MaterialType::RED_PLASTIC: return Material(ColorM(0.0f, 0.0f, 0.0f, 1.0f), ColorM(0.5f, 0.0f, 0.0f, 1.0f), ColorM(0.7f, 0.6f, 0.6f, 1.0f), 0.25f);
		case MaterialType::WHITE_PLASTIC: return Material(ColorM(0.0f, 0.0f, 0.0f, 1.0f), ColorM(0.55f, 0.55f, 0.55f, 1.0f), ColorM(0.70f, 0.70f, 0.70f, 1.0f), 0.25f);
		case MaterialType::YELLOW_PLASTIC: return Material(ColorM(0.0f, 0.0f, 0.0f, 1.0f), ColorM(0.5f, 0.5f, 0.0f, 1.0f), ColorM(0.60f, 0.60f, 0.50f, 1.0f), 0.25f);
		case MaterialType::BLACK_RUBBER: return Material(ColorM(0.02f, 0.02f, 0.02f, 1.0f), ColorM(0.01f, 0.01f, 0.01f, 1.0f), ColorM(0.4f, 0.4f, 0.4f, 1.0f), 0.078125f);
		case MaterialType::CYAN_RUBBER: return Material(ColorM(0.0f, 0.05f, 0.05f, 1.0f), ColorM(0.4f, 0.5f, 0.5f, 1.0f), ColorM(0.04f, 0.7f, 0.7f, 1.0f), 0.078125f);
		case MaterialType::GREEN_RUBBER: return Material(ColorM(0.0f, 0.05f, 0.0f, 1.0f), ColorM(0.4f, 0.5f, 0.4f, 1.0f), ColorM(0.04f, 0.7f, 0.04f, 1.0f), 0.078125f);
		case MaterialType::RED_RUBBER: return Material(ColorM(0.05f, 0.0f, 0.0f, 1.0f), ColorM(0.5f, 0.4f, 0.4f, 1.0f), ColorM(0.7f, 0.04f, 0.04f, 1.0f), 0.078125f);
		case MaterialType::WHITE_RUBBER: return Material(ColorM(0.05f, 0.05f, 0.05f, 1.0f), ColorM(0.5f, 0.5f, 0.5f, 1.0f), ColorM(0.7f, 0.7f, 0.7f, 1.0f), 0.078125f);
		case MaterialType::YELLOW_RUBBER: return Material(ColorM(0.05f, 0.05f, 0.0f, 1.0f), ColorM(0.5f, 0.5f, 0.4f, 1.0f), ColorM(0.7f, 0.7f, 0.04f, 1.0f), 0.078125f);
		}
	}
};
class ObjectRenderer {
	// Shading 
	//http://info.ee.surrey.ac.uk/Teaching/Courses/CGI/lectures_pdf/opengl4.pdf
	//https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glShadeModel.xml
protected:
	GLuint displayID;

public:
	string path;
	vector<array<TType, 3>> mTriangles;
	vector<glm::vec3> mTrianglesNormals;
	vector<glm::vec3> mTrianglesCentroid;

	vector<glm::vec3> mVertices;
	vector<glm::vec3> mVerticesNormal;

	vec3 minsBB{ 999999,999999,9999999 };
	vec3 maxsBB{ -999999,-999999,-9999999 };


	bool dirty = true;
	DisplayType displayType = DisplayType::GL;
	
	bool isZCulled = false;
	bool isAffectedByLight = true;
	bool isFlatOrGouraud = true;
	bool isBackFaceCulled = false;
	bool showBB = false;
	bool showVertexNormals = false;
	bool showFaceNormals = false;
	bool showFill = true;
	bool showVertex = false;
	bool showMesh = false;
	float normalScale = 0.1f;

	Color colorVertex{ 0,1,1 , 1 };
	Color colorEdges{ 0,0,1 , 1 };
	Color colorFill{ 1, 1, 1 , 1 };
	Color colorNormalsVertex{ 1,1,0 , 1 };
	Color colorNormalsFace{ 0.2f,0.9f,0.4f, 1 };
	Color colorBB{ 0,0.9f,0, 1 };

	Material material;

	ObjectRenderer& operator=(ObjectRenderer other)
	{
		dirty = other.dirty;
		displayType = other.displayType;
		isZCulled = other.isZCulled;
		isAffectedByLight = other.isAffectedByLight;
		isFlatOrGouraud = other.isFlatOrGouraud;
		isBackFaceCulled = other.isBackFaceCulled;
		showVertexNormals = other.showVertexNormals;
		showFaceNormals = other.showFaceNormals;
		showFill = other.showFill;
		showVertex = other.showVertex;
		showMesh = other.showMesh;
		normalScale = other.normalScale;
		colorVertex = other.colorVertex;
		colorEdges = other.colorEdges;
		colorFill = other.colorFill;
		colorNormalsVertex = other.colorNormalsVertex;
		colorNormalsFace = other.colorNormalsFace;
		colorBB = other.colorBB;
		material = other.material;
		return *this;
	}
	void CalculateNormals()
{
	// const auto normalizeFunction = [] (float& x, float minv, float maxv) { x =  2.0*(x - minv) / (maxv - minv) - 1.0; }; // marico me odio por qu� soy as�
	// Asegurando que todo est� limpio
	mVerticesNormal.clear();

	mTrianglesCentroid.clear();
	mTrianglesNormals.clear();


	// Reservando memoria
	mVerticesNormal.reserve(mVertices.size());

	mTrianglesCentroid.reserve(mTriangles.size());
	mTrianglesNormals.reserve(mTriangles.size());

	vec3 center = maxsBB + minsBB;

	float ma = maxsBB.x;
	ma = std::max(ma, maxsBB.y);
	ma = std::max(ma, maxsBB.z);



	//vec3 center = (maxsBB + minsBB)/2.0f;
	for (auto& vertex : mVertices) {
		vertex = (vertex - center) / ma;
	}

	maxsBB = (maxsBB - center) / ma;
	minsBB = (minsBB - center) / ma;

	vector< vector<int> > trianglesRelated(mVertices.size(), vector<int>());

	for (int indexTriangle = 0, iiEnd = mTriangles.size(); indexTriangle < iiEnd; indexTriangle++) {
		auto triangle = mTriangles[indexTriangle];

		vec3 v0 = mVertices[triangle[0]], v1 = mVertices[triangle[1]], v2 = mVertices[triangle[2]];

		// Normal de Triangulo
		mTrianglesNormals.push_back(glm::normalize(glm::cross(v1 - v0, v2 - v0)));

		// Centroides de Triangulo
		mTrianglesCentroid.push_back(vec3(
			(v0.x + v1.x + v2.x) / 3.0f
			, (v0.y + v1.y + v2.y) / 3.0f
			, (v0.z + v1.z + v2.z) / 3.0f
		));

		//agregando a indice de vertices que necesitaremos despu�s
		for (auto indexVertex : triangle) {
			trianglesRelated[indexVertex].push_back(indexTriangle);
		}
	}

	for (auto triangleIndexes : trianglesRelated) {
		vec3 norm;
		for (auto tIndex : triangleIndexes) {
			norm +=mTrianglesNormals[tIndex];
		}
		norm /= (float)triangleIndexes.size();
		mVerticesNormal.push_back(glm::normalize(norm));
	}
}
	void DisplayGizmos() 
{
		glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	if (showFaceNormals) {
		glColorFunc(ColorToFloatArr(colorNormalsFace));

		for (size_t ii = 0, iiEnd = mTriangles.size(); ii < iiEnd; ii++)
		{
			glVertex3fv(glm::value_ptr(mTrianglesCentroid[ii]));
			glVertex3fv(glm::value_ptr(mTrianglesCentroid[ii] + mTrianglesNormals[ii] * normalScale));
		}
	}

	if (showVertexNormals) {
		glColorFunc(ColorToFloatArr(colorNormalsVertex));
		for (size_t ii = 0, iiEnd = mVertices.size(); ii < iiEnd; ii++)
		{
			glVertex3fv(glm::value_ptr(mVertices[ii]));
			glVertex3fv(glm::value_ptr(mVertices[ii] + mVerticesNormal[ii] * normalScale));
		}
	}

	glColorFunc(ColorToFloatArr(colorBB));

	//abajo
	if (showBB) {

		glVertex3f(minsBB.x, minsBB.y, minsBB.z);
		glVertex3f(minsBB.x, minsBB.y, maxsBB.z);

		glVertex3f(minsBB.x, minsBB.y, maxsBB.z);
		glVertex3f(maxsBB.x, minsBB.y, maxsBB.z);

		glVertex3f(maxsBB.x, minsBB.y, maxsBB.z);
		glVertex3f(maxsBB.x, minsBB.y, minsBB.z);

		glVertex3f(maxsBB.x, minsBB.y, minsBB.z);
		glVertex3f(minsBB.x, minsBB.y, minsBB.z);

		//arriba

		glVertex3f(minsBB.x, maxsBB.y, minsBB.z);
		glVertex3f(minsBB.x, maxsBB.y, maxsBB.z);

		glVertex3f(minsBB.x, maxsBB.y, maxsBB.z);
		glVertex3f(maxsBB.x, maxsBB.y, maxsBB.z);

		glVertex3f(maxsBB.x, maxsBB.y, maxsBB.z);
		glVertex3f(maxsBB.x, maxsBB.y, minsBB.z);

		glVertex3f(maxsBB.x, maxsBB.y, minsBB.z);
		glVertex3f(minsBB.x, maxsBB.y, minsBB.z);

		//lados

		glVertex3f(minsBB.x, maxsBB.y, maxsBB.z);
		glVertex3f(minsBB.x, minsBB.y, maxsBB.z);

		glVertex3f(minsBB.x, maxsBB.y, minsBB.z);
		glVertex3f(minsBB.x, minsBB.y, minsBB.z);

		glVertex3f(maxsBB.x, maxsBB.y, minsBB.z);
		glVertex3f(maxsBB.x, minsBB.y, minsBB.z);

		glVertex3f(maxsBB.x, maxsBB.y, maxsBB.z);
		glVertex3f(maxsBB.x, minsBB.y, maxsBB.z);

	}
	glEnd();
	glEnable(GL_LIGHTING);

}
	void Display()
	{
		isZCulled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
		isBackFaceCulled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
		material.ApplyMaterial();

		switch (displayType)
		{
		case DisplayType::GL:
			DisplayGL();
			break;
		case DisplayType::VBO:
			DisplayVBO();
			break;
		case DisplayType::LIST:
			DisplayList();
			break;
		case DisplayType::VP:
			DisplayVP();
			break;
		}
		//Material::RemoveMaterial();
	}

protected:
	void DisplayGL() 
	{
		isAffectedByLight ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
	if (showFill) {
		glColorFunc(ColorToFloatArr(colorFill));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (isFlatOrGouraud) {
			int ii = 0;
			glShadeModel(GL_FLAT);	
			for (auto triangle : mTriangles)
			{
				glNormal3fv(glm::value_ptr(this->mTrianglesNormals[ii]));
				glVertex3fv(glm::value_ptr(this->mTrianglesCentroid[ii]));
				glBegin(GL_TRIANGLES);
				{
					glVertex3fv(glm::value_ptr(mVertices[triangle[0]]));
					glVertex3fv(glm::value_ptr(mVertices[triangle[1]]));
					glVertex3fv(glm::value_ptr(mVertices[triangle[2]]));
				}
				glEnd();

				ii++;
			}
		} 
		else {
			glShadeModel(GL_SMOOTH);
			glBegin(GL_TRIANGLES);

			for (auto triangle : mTriangles) 
			{
				
				glNormal3fv(glm::value_ptr(this->mVerticesNormal[triangle[0]]));
				glVertex3fv(glm::value_ptr(mVertices[triangle[0]]));
				glNormal3fv(glm::value_ptr(this->mVerticesNormal[triangle[1]]));
				glVertex3fv(glm::value_ptr(mVertices[triangle[1]]));
				glNormal3fv(glm::value_ptr(this->mVerticesNormal[triangle[2]]));
				glVertex3fv(glm::value_ptr(mVertices[triangle[2]]));
			}
		glEnd();
		}

	}

	glDisable(GL_LIGHTING);
	glPolygonOffset(2, 2);

	if (showVertex) {
		glColorFunc(ColorToFloatArr(colorVertex));
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		DrawTriangles();
	}

	if (showMesh) {
		glColorFunc(ColorToFloatArr(colorEdges));
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		DrawTriangles();
	}
	glPolygonOffset(0, 0);
}
	void DisplayVBO() 
{
	//https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Buffer_Object
	//http://www.songho.ca/opengl/gl_vbo.html

	//GUIAS CLASE
	//void model::createVBO() {
	//	glGenBuffers(1, &vbo);
	//	glGenBuffers(1, &vindex);
	//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vindex);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numOfVertex * 3, vertex, GL_STATIC_DRAW);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* numOfIndex, index, GL_STATIC_DRAW);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//}

		//glGenBuffers(1, &displayID);

	//void model::displayVBO() {
	//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vindex);
	//	glDrawElements(GL_TRIANGLES, mNumOfIndex, GL_UNSIGNED_INT, 0);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//}

	if (dirty) {
		///* Allocate and assign two Vertex Buffer Objects to our handle */
		//glGenBuffers(1, &displayID);
		///* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
		//glBindBuffer(GL_ARRAY_BUFFER, displayID);

		///* Enable attribute index 0 as being used */
		//glEnableVertexAttribArray(0);
		/////* Bind our second VBO as being the active buffer and storing vertex attributes (colors) */
		////glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

		////cleanup
		//glDeleteBuffers(1, &displayID);
	}
}
	void DisplayList() 
{
	displayID = glGenLists(1);

	if (dirty) {

		glNewList(displayID, GL_COMPILE_AND_EXECUTE);
		{
			DisplayGL();
		}
		glEndList();
	}

	glCallList(displayID);
}
	void DisplayVP()
{
	//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
	//http://www.songho.ca/opengl/gl_vertexarray.html#
	//https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDrawElements.xml
	//https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glVertexPointer.xml

	if (dirty)
	{
		{

			if (showFill)
			{
				isAffectedByLight ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
				glColorFunc(ColorToFloatArr(colorFill));
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				if (isFlatOrGouraud) {
					glShadeModel(GL_FLAT);
					
					glEnableClientState(GL_NORMAL_ARRAY);
					{
						glNormalPointer(GL_FLOAT, 0, mTrianglesNormals.data());
						glVertexPointer(3, GL_FLOAT, 0, mTrianglesCentroid.data());

					}
					glDisableClientState(GL_NORMAL_ARRAY);	


					glEnableClientState(GL_VERTEX_ARRAY);
					{
						glVertexPointer(3,GL_FLOAT, 0, mVertices.data());
						glDrawElements(GL_TRIANGLES, mTriangles.size() *3, GL_UNSIGNED_SHORT, mTriangles.data());
					}
					glDisableClientState(GL_VERTEX_ARRAY);
		   			 		  
				}
				else 
				{
					glShadeModel(GL_SMOOTH);

					glEnableClientState(GL_VERTEX_ARRAY);
					glEnableClientState(GL_NORMAL_ARRAY);
					{
						glNormalPointer(GL_FLOAT, 0, mVerticesNormal.data());
						glVertexPointer(3, GL_FLOAT, 0, mVertices.data());

						glDrawElements(GL_TRIANGLES, mTriangles.size() * 3, GL_UNSIGNED_SHORT, mTriangles.data());
					}
					glDisableClientState(GL_NORMAL_ARRAY);
					glDisableClientState(GL_VERTEX_ARRAY);
				}
				//glDrawArrays(GL_TRIANGLES, 0,mVertices.size());
				glDisable(GL_LIGHTING);
			}



			glDisable(GL_LIGHTING);
			glPolygonOffset(2, 2);

			if (showVertex) {
			glEnableClientState(GL_VERTEX_ARRAY);
			{
				glColorFunc(ColorToFloatArr(colorVertex));

				glVertexPointer(3, GL_FLOAT, 0, mVertices.data());

				glDrawElements(GL_POINTS, mVertices.size()*3, GL_UNSIGNED_SHORT, mTriangles.data());
			}
			glDisableClientState(GL_VERTEX_ARRAY);
			}


			if (showMesh)
			{
				glEnableClientState(GL_VERTEX_ARRAY); 
				{
				glColorFunc(ColorToFloatArr(colorEdges));

				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				glVertexPointer(3, GL_FLOAT, 0, mVertices.data());

				glDrawElements(GL_TRIANGLES, mTriangles.size()*3, GL_UNSIGNED_SHORT, mTriangles.data());
				}
			}

			glPolygonOffset(0, 0);

			glDisableClientState(GL_VERTEX_ARRAY);

		}
	}

}
	

	inline void DrawTriangles() {
		for (auto triangle : mTriangles) {
			glBegin(GL_TRIANGLES);
			{
				glVertex3fv(glm::value_ptr(mVertices[triangle[0]]));
				glVertex3fv(glm::value_ptr(mVertices[triangle[1]]));
				glVertex3fv(glm::value_ptr(mVertices[triangle[2]]));
			}
			glEnd();
		}
	}
};