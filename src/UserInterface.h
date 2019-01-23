#pragma once

#include "../headers/AntTweakBar.h"
#include "../headers/glm/glm.hpp"
#include "../headers/glm/gtc/quaternion.hpp"
#include "../headers/glm/gtx/quaternion.hpp"
#include "Main.h"
#include <iostream>
#include <string>

using std::string;



//Singleton user interface class

class CUserInterface
{
private:
	///Private constructor
	static CUserInterface * mInterface; //Holds the instance of the class
	TwBar *mUserInterface;
	CUserInterface(); 

public:
	CModel model;
	///Method to obtain the only instance of the calls

	static CUserInterface * Instance();
	~CUserInterface();
	void reshape();
	void show();
	void hide();
	void setModel(CModel* = nullptr);
	CModel getModel();
	void UpdateLights();
	void CreateLightUI(Light* light) {
		ObjectRenderer* renderer = light->renderer;

		renderer->showFill = true;
		renderer->showMesh = false;
		renderer->showVertex = false;


		string prefix = light->path;
		string group = " group='" + light->path+"' ";

		TwAddSeparator(mUserInterface, (prefix+"Light").data(), NULL);
		TwAddVarRW(mUserInterface, (prefix + "isOn").data() , TW_TYPE_BOOLCPP, &light->isOn, group.data());

		TwAddVarRW(mUserInterface, (prefix+ "T_X ").data(), TW_TYPE_FLOAT, &light->position.x, (group +" step=0.5").data());
		TwAddVarRW(mUserInterface, (prefix + "T_Y ").data(), TW_TYPE_FLOAT, &light->position.y, (group + " step=0.5").data());
		TwAddVarRW(mUserInterface, (prefix + "T_Z ").data(), TW_TYPE_FLOAT, &light->position.z, (group + " step=0.5").data());
		
		TwAddVarRW(mUserInterface, (prefix + "ambient").data(), TW_TYPE_COLOR4F, &light->ambient, group.data());
		TwAddVarRW(mUserInterface, (prefix + "specular").data(), TW_TYPE_COLOR4F, &light->specular, group.data());
		TwAddVarRW(mUserInterface, (prefix + "diffuse").data(), TW_TYPE_COLOR4F, &light->diffuse, group.data());

		TwAddVarRW(mUserInterface, (prefix + "constantAtenuation ").data(), TW_TYPE_FLOAT, &light->constantAtenuation, (group + " step=0.1").data());
		TwAddVarRW(mUserInterface, (prefix + "linearAtenuation ").data(), TW_TYPE_FLOAT, &light->linearAtenuation, (group + " step=0.1").data());
		TwAddVarRW(mUserInterface, (prefix + "quadraticAtenuation ").data(), TW_TYPE_FLOAT, &light->quadraticAtenuation, (group + " step=0.1").data());

		//&light->isOn


	}
	void SetMax(int i) {
		TwDefine(("Model/Picked min =0 max="+std::to_string(i)).data());
	}
};