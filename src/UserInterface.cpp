#include "UserInterface.h"

extern bool isPerspectiveOrOrto;
extern int gWidth, gHeight;
extern int picked;
CUserInterface * CUserInterface::mInterface = NULL;
float allScale = 1;
MaterialType materialType = MaterialType::BLACK_PLASTIC;
CUserInterface * CUserInterface::Instance() 
{
	if (!mInterface) {
		mInterface = new CUserInterface();
	}
   return mInterface;
}
//https://stackoverflow.com/questions/18660248/ray-casting-from-mouse-with-opengl-2
CUserInterface::CUserInterface()
{
	//http://anttweakbar.sourceforge.net/doc/tools:anttweakbar:twaddbutton
	//http://anttweakbar.sourceforge.net/doc/tools:anttweakbar:twaddvarcb

	mUserInterface = TwNewBar("Model");
	TwDefine("Model refresh = '0.0001f'");
	TwDefine("Model resizable = true");
	TwDefine("Model fontresizable = false");
	TwDefine("Model movable = true");
	TwDefine("Model position = '20 20'");
	TwDefine("Model size = '250 600'");


	TwAddButton(mUserInterface, "Click to load model", [](void* clientData) {
		AddModel(openfilename());
	}, NULL, NULL);

	TwAddVarCB(mUserInterface, "isPerspectiveOrOrto", TW_TYPE_BOOLCPP, [](const void* floatValue, void* clientData) {
		*((bool*)clientData) = *((bool*)floatValue);
		externalReshape();
	},
		[](void* floatValue, void* clientData) {
		*((bool*)floatValue) = *((bool*)clientData);

	}, &isPerspectiveOrOrto, NULL);
	TwAddVarRW(mUserInterface, "Picked", TW_TYPE_INT32, &picked, NULL);


	TwAddVarRW(mUserInterface, "T_X", TW_TYPE_FLOAT, &model.transform.mTranslation.x, " group='Translation' step=0.5");
	TwAddVarRW(mUserInterface, "T_Y", TW_TYPE_FLOAT, &model.transform.mTranslation.y, " group='Translation' step=0.5");
	TwAddVarRW(mUserInterface, "T_Z", TW_TYPE_FLOAT, &model.transform.mTranslation.z, " group='Translation' step=0.5");

	TwAddVarRW(mUserInterface, "S_X", TW_TYPE_FLOAT, &model.transform.mScale.x, " group='Scale' step=0.1");
	TwAddVarRW(mUserInterface, "S_Y", TW_TYPE_FLOAT, &model.transform.mScale.y, " group='Scale' step=0.1");
	TwAddVarRW(mUserInterface, "S_Z", TW_TYPE_FLOAT, &model.transform.mScale.z, " group='Scale' step=0.1");
	TwAddVarCB(mUserInterface, "S_ALL", TW_TYPE_FLOAT, [](const void* floatValue, void* clientData) {
		CUserInterface *u = CUserInterface::Instance(); // cant capture bc it would break the types jeje
		*((float*)clientData) = *((float*)floatValue);
		
		allScale = *((float*)floatValue);

		u->model.transform.mScale.x = allScale;
		u->model.transform.mScale.y = allScale;
		u->model.transform.mScale.z = allScale;
	}, 
		[](void* floatValue, void* clientData) {
		*((float*)floatValue) = *((float*)clientData);
	
	}, &allScale, " group='Scale' step=0.25");

	TwAddVarRW(mUserInterface, "ObjRotation", TW_TYPE_QUAT4F, &model.transform.mRotation,
		" label='Object rotation' opened=true help='Change the object orientation.' ");
	//
	TwAddVarRW(mUserInterface, "isZCulled", TW_TYPE_BOOLCPP, &model.renderer->isZCulled, " group='RenderOptions'");
	TwAddVarRW(mUserInterface, "isAffectedByLight", TW_TYPE_BOOLCPP, &model.renderer->isAffectedByLight, " group='RenderOptions'");
	TwAddVarRW(mUserInterface, "isFlatOrGouraud", TW_TYPE_BOOLCPP, &model.renderer->isFlatOrGouraud, " group='RenderOptions'");
	TwAddVarRW(mUserInterface, "isBackFaceCulled", TW_TYPE_BOOLCPP, &model.renderer->isBackFaceCulled, " group='RenderOptions'");
	TwAddVarRW(mUserInterface, "showVertexNormals", TW_TYPE_BOOLCPP, &model.renderer->showVertexNormals, " group='RenderOptions'");
	TwAddVarRW(mUserInterface, "showFaceNormals", TW_TYPE_BOOLCPP, &model.renderer->showFaceNormals, " group='RenderOptions'");
	TwAddVarRW(mUserInterface, "showFill", TW_TYPE_BOOLCPP, &model.renderer->showFill, " group='RenderOptions'");
	TwAddVarRW(mUserInterface, "showVertex", TW_TYPE_BOOLCPP, &model.renderer->showVertex, " group='RenderOptions'");
	TwAddVarRW(mUserInterface, "showMesh", TW_TYPE_BOOLCPP, &model.renderer->showMesh, " group='RenderOptions'");

	TwAddVarRW(mUserInterface, "normalScale", TW_TYPE_FLOAT, &model.renderer->normalScale, " group='RenderOptions' step=0.01");

	TwEnumVal enumFigures[] = {
		{ static_cast<int>(DisplayType::GL) , "GL" },
		{ static_cast<int>(DisplayType::VBO) , "VBO" },
		{ static_cast<int>(DisplayType::VP) , "VP" },
		{ static_cast<int>(DisplayType::LIST) , "LIST" },
	};
	TwType twEnumFigures = TwDefineEnum("Figures", enumFigures, 4);

	TwEnumVal enumMaterials[] = {
		{static_cast<int>(MaterialType::EMERALD), "EMERALD"},
		{static_cast<int>(MaterialType::JADE), "JADE"},
		{static_cast<int>(MaterialType::OBSIDIAN), "OBSIDIAN"},
		{static_cast<int>(MaterialType::PEARL), "PEARL"},
		{static_cast<int>(MaterialType::RUBY), "RUBY"},
		{static_cast<int>(MaterialType::TURQUOISE), "TURQUOISE"},
		{static_cast<int>(MaterialType::BRASS), "BRASS"},
		{static_cast<int>(MaterialType::BRONZE), "BRONZE"},
		{static_cast<int>(MaterialType::CHROME), "CHROME"},
		{static_cast<int>(MaterialType::COPPER), "COPPER"},
		{static_cast<int>(MaterialType::GOLD), "GOLD"},
		{static_cast<int>(MaterialType::SILVER), "SILVER"},
		{static_cast<int>(MaterialType::BLACK_PLASTIC), "BLACK_PLASTIC"},
		{static_cast<int>(MaterialType::CYAN_PLASTIC), "CYAN_PLASTIC"},
		{static_cast<int>(MaterialType::GREEN_PLASTIC), "GREEN_PLASTIC"},
		{static_cast<int>(MaterialType::RED_PLASTIC), "RED_PLASTIC"},
		{static_cast<int>(MaterialType::WHITE_PLASTIC), "WHITE_PLASTIC"},
		{static_cast<int>(MaterialType::YELLOW_PLASTIC), "YELLOW_PLASTIC"},
		{static_cast<int>(MaterialType::BLACK_RUBBER), "BLACK_RUBBER"},
		{static_cast<int>(MaterialType::CYAN_RUBBER), "CYAN_RUBBER"},
		{static_cast<int>(MaterialType::GREEN_RUBBER), "GREEN_RUBBER"},
		{static_cast<int>(MaterialType::RED_RUBBER), "RED_RUBBER"},
		{static_cast<int>(MaterialType::WHITE_RUBBER), "WHITE_RUBBER"},
		{static_cast<int>(MaterialType::YELLOW_RUBBER), "YELLOW_RUBBER"},
	};
	TwType twEnumMaterials = TwDefineEnum("Materials", enumMaterials, 24);


	TwType colorType = TW_TYPE_COLOR4F;
	TwAddVarRW(mUserInterface, "colorVertex", colorType, &model.renderer->colorVertex, " group='Color'");
	TwAddVarRW(mUserInterface, "colorEdges", colorType, &model.renderer->colorEdges, " group='Color'");
	TwAddVarRW(mUserInterface, "colorFill", colorType, &model.renderer->colorFill, " group='Color'");
	TwAddVarRW(mUserInterface, "colorNormalsVertex", colorType, &model.renderer->colorNormalsVertex, " group='Color'");
	TwAddVarRW(mUserInterface, "colorNormalsFace", colorType, &model.renderer->colorNormalsFace, " group='Color'");
	TwAddVarRW(mUserInterface, "colorBB", colorType, &model.renderer->colorBB, " group='Color'");

	TwAddVarCB(mUserInterface, "Material Type", twEnumMaterials, [](const void* enumMaterial, void* clientData) {

		*static_cast<MaterialType*>(clientData) = *static_cast<const MaterialType*>(enumMaterial);

		CUserInterface *u = CUserInterface::Instance(); // cant capture bc it would break the types jeje
		u->model.renderer->material = Material::GetMaterial(static_cast<MaterialType>(*(int*)enumMaterial));
	}, [](void* enumMaterial, void* clientData) {
		*static_cast<MaterialType*>(enumMaterial) = *static_cast<MaterialType*>(clientData);

	}, &materialType, " group='Material'");


	TwAddVarRW(mUserInterface, "isEmisor", TW_TYPE_BOOLCPP, &model.renderer->material.isEmisor, " group='Material' ");
	TwAddVarRW(mUserInterface, "ambient", colorType, &model.renderer->material.ambient, " group='Material' ");
	TwAddVarRW(mUserInterface, "diffuse", colorType, &model.renderer->material.diffuse, " group='Material' ");
	TwAddVarRW(mUserInterface, "specular", colorType, &model.renderer->material.specular, " group='Material'");
	TwAddVarRW(mUserInterface, "emisor", colorType, &model.renderer->material.emisor, " group='Material' ");
	TwAddVarRW(mUserInterface, "shininess", TW_TYPE_FLOAT, &model.renderer->material.shininess, " group='Material' step=0.01");
	TwAddVarRW(mUserInterface, "Display Type", twEnumFigures, &model.renderer->displayType, " group='RenderOptions'");

}

CUserInterface::~CUserInterface()
{
}

void CUserInterface::reshape()
{
	TwWindowSize(gWidth, gHeight);
}

void CUserInterface::show()
{
	TwDefine("Figure visible = true");
}

void CUserInterface::hide()
{
	TwDefine("Figure visible = false");
}

void CUserInterface::setModel(CModel* model)
{

	if (model) 
	{
		this->model = *model;
	}
	else 
	{

	}
}



CModel CUserInterface::getModel() {
	return this->model;
}


void CUserInterface::UpdateLights() {
	//for (Light* light : lights) {
	//	light->ApplyLight();
	//}
}