#pragma once
#include "String.h"
#include "Model.h"
#include <iostream>
#include <fstream>
#include <thread>

using std::fstream;

class Loader {
public:
	void Load(string, void func(ObjectRenderer*));
private:
	static ObjectRenderer* FromSOFF(ObjectRenderer*, void func(ObjectRenderer*));
	static ObjectRenderer* FromOBJ(ObjectRenderer*, void func(ObjectRenderer*));
	static ObjectRenderer* FromOFF(ObjectRenderer*, void func(ObjectRenderer*));
};