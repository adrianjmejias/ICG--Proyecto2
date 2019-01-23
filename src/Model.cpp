#include "Model.h"

CModel::CModel()
{
	renderer = new ObjectRenderer();
}

CModel::~CModel()
{
}

void CModel::TakeCustomization(CModel& other) {
	path = other.path;
	*renderer = *other.renderer;
	transform = other.transform;
}