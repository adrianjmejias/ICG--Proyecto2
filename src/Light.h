#pragma once
#include "../headers/glm/glm.hpp"
#include "SceneElement.h"
#include "Transform.h"
typedef glm::vec4 ColorM;



using std::string;

enum class LightType {
	Ambient,
	Point,
	SpotLight,
	Distant,
};

class Light {
	//http://what-when-how.com/opengl-programming-guide/defining-material-properties-lighting-opengl-programming/
	//http://info.ee.surrey.ac.uk/Teaching/Courses/CGI/lectures_pdf/opengl4.pdf
public:
	string path;
	ObjectRenderer* renderer;
	LightType lightType = LightType::Ambient;
	int  lightID;
	glm::vec4 position;
	bool isOn = true;
	ColorM ambient{ 0.2F, 0.2F, 0.2F, 1.0F };
	ColorM specular{ 1.0, 1.0, 1.0, 1.0 };
	ColorM diffuse{ 1.0, 1.0, 1.0, 1.0 };
	float constantAtenuation=1;
	float linearAtenuation;
	float quadraticAtenuation;

	Light(int _lightID)
	{
		lightID = _lightID;
		path = "Light" + std::to_string(lightID);
	}
	void ApplyLight() {
		glEnable(GL_LIGHTING);
		isOn ? glEnable(lightID) : glDisable(lightID);
		renderer->showFill = isOn;
		
		glLightfv(lightID, GL_POSITION, glm::value_ptr(position));
		glLightfv(lightID, GL_AMBIENT, glm::value_ptr(ambient));
		glLightfv(lightID, GL_DIFFUSE, glm::value_ptr(diffuse));
		glLightfv(lightID, GL_SPECULAR, glm::value_ptr(specular));

		glLightf(lightID, GL_CONSTANT_ATTENUATION, constantAtenuation);
		glLightf(lightID, GL_LINEAR_ATTENUATION, linearAtenuation);
		glLightf(lightID, GL_QUADRATIC_ATTENUATION, quadraticAtenuation);
	}
};