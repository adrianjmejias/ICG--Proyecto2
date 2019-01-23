#include "Windows.h"
#include "Main.h"
#include "UserInterface.h"

#include <mutex>
using std::vector;
bool isPerspectiveOrOrto = true;
Camera* camera;
vector<Light*> lights;
Loader loader;
GLFWwindow *gWindow;
int gWidth, gHeight;
CUserInterface * userInterface;
vector <CModel *> models;
int picked;
array<glm::vec4, 2> pos{ glm::vec4{5.0f, 3.5f, 3.5f,1.0f},glm::vec4{-8,0,0,1.0f}};
std::mutex vectorLock;

// para después
//template <typename T>
//class syncVector : vector<T> {
//public:
//	void push_back(T e) {
//		vector::
//	}
//};

int lightCreationIndex = GL_LIGHT0;
int lightIntenalIndex = 0;
void pickObject(int obj) {
	if (picked > -1) {
		models[picked]->renderer->showBB = false;
	}
	picked = obj;
	models[picked]->renderer->showBB = true;
	userInterface->model.TakeCustomization(*models[picked]);
	userInterface->SetMax(picked);
	updateUserInterface();
}

string openfilename()
{
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "OBJ Files(.OBJ)\0*.OBJ\0OFF Files(.OFF)\0*.OFF\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";
	string fileNameStr;
	if (GetOpenFileName(&ofn))
		fileNameStr = fileName;
	return fileNameStr;
}

void AddModel(string path) {
	loader.Load(path, [](ObjectRenderer* _re) {
		CModel* _model = new CModel(_re);
		vectorLock.lock();
		{
			models.push_back(_model);
			pickObject(models.size()-1);
		}
		vectorLock.unlock();
	});
}

void AddLight(string path) {
	loader.Load(path, [](ObjectRenderer* _re) {
		Light* _model = new Light(lightCreationIndex++);
		_model->position = pos[lightIntenalIndex++];
		_model->renderer = _re;
		vectorLock.lock();
		{
			lights.push_back(_model);
			userInterface->CreateLightUI(_model);
		}
		vectorLock.unlock();
	});
}

void updateUserInterface()
{
	if (picked > -1) {
		models[picked]->TakeCustomization(userInterface->model);
	}
}


//float f(float n) { return sin(n / 2); }
void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for(auto model: models)
	{	
		const Transform t = model->transform;
		glm::vec3 translation = t.mTranslation;
		glm::vec3 scale = t.mScale;
		glm::quat rotation = t.mRotation;

		glPushMatrix();
		{
			glTranslatef(translation.x, translation.y, translation.z);
			glMultMatrixf(glm::value_ptr(glm::toMat4(rotation)));
			glScalef(scale.x, scale.y, scale.z);

			model->renderer->DisplayGizmos();
			model->renderer->Display();
		}
		glPopMatrix();
	}
	for (auto model : lights)
	{
		glm::vec4 translation = model->position;
		glPushMatrix();

		glTranslatef(translation.x, translation.y, translation.z);

		model->ApplyLight();
		model->renderer->Display();
		glPopMatrix();
	}

}

void externalReshape() {
	reshape(gWindow, gWidth, gHeight);
}

void reshape(GLFWwindow *window, int width, int height)
{
	gWidth = width;
	gHeight = height;

	glViewport(0, 0, gWidth, gHeight);

	userInterface->reshape();

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	if (isPerspectiveOrOrto) {
		gluPerspective(45.0f, (float)gWidth / (float)gHeight, 1.0f, 1000.0f);
	}
	else {
		glOrtho(-10, 10, -10, 10, 1, 1000);
	}

	glMatrixMode(GL_MODELVIEW);
}

void keyInput(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (TwEventKeyGLFW(key, action))
		return;

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
		}
	}
}

void mouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (TwEventMouseButtonGLFW(button, action))
		return;
}

void cursorPos(GLFWwindow* window, double x, double y)
{
	if (TwEventMousePosGLFW(int(x), int(y)))
		return;
}

void charInput(GLFWwindow* window, unsigned int scanChar)
{
	if (TwEventCharGLFW(scanChar, GLFW_PRESS))
		return;
}

void destroy()
{
	for (unsigned int i = 0; i < models.size(); i++)
		delete models[i];

	delete userInterface;

	TwTerminate();
	glfwDestroyWindow(gWindow);
	glfwTerminate();
}

bool initGlfw()
{
	if (!glfwInit())
		return false;

	gWindow = glfwCreateWindow(gWidth, gHeight, "Chapter 2", NULL, NULL);

	if (!gWindow)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(gWindow);

	const GLFWvidmode * vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(gWindow, (vidMode->width - gWidth) >> 1, (vidMode->height - gHeight) >> 1);

	glfwSetWindowSizeCallback(gWindow, reshape);
	glfwSetKeyCallback(gWindow, keyInput);
	glfwSetMouseButtonCallback(gWindow, mouseButton);
	glfwSetCursorPosCallback(gWindow, cursorPos);
	glfwSetCharCallback(gWindow, charInput);

	return true;
}

bool initUserInterface()
{
	if (!TwInit(TW_OPENGL, NULL))
		return false;

	userInterface = CUserInterface::Instance();

	return true;
}

bool initScene()
{
	//AddModel("../files/sphere.obj");
	AddModel("../files/Apple.off");
	//AddModel("../files/Oso.obj");
	//AddModel("../files/house3.obj");
	//AddModel("../files/Umbrella.obj");
	//AddModel("../files/baymax.obj");
	//AddModel("../files/dragon.off");
	//AddModel("../files/seashell.off");
	//AddModel("../files/space_shuttle.off");
	//AddModel("../files/space_station.off");
	//AddModel("../files/teapot.off");
	//AddModel("../files/dragon.off");
	//AddModel("../files/Batman.obj");
	//AddModel("../files/sphere2.off");

	return true;
}

int main(void)
{
	gWidth = 1200;
	gHeight = 680;
	picked = -1;
	vec3 a;
	//cout << openfilename();

	if (!initGlfw() || !initScene() || !initUserInterface())
		return EXIT_FAILURE;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	reshape(gWindow, gWidth, gHeight);
	glMatrixMode(GL_MODELVIEW);
	glPointSize(5);
	glLoadIdentity();
	//glOrtho(10.0f, gWidth, gHeight, 0.0f, 0.0f, 1.0f);
	gluLookAt(3.0f, 3.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER,glm::value_ptr(glm::vec3(1,1,1)));

	AddLight("../files/Lightbulb.obj");
	AddLight("../files/Lightbulb.obj");

	while (!glfwWindowShouldClose(gWindow))
	{
		display();

		TwDraw();

		updateUserInterface();

		glfwSwapBuffers(gWindow);

		glfwPollEvents();
	}

	destroy();

	return EXIT_SUCCESS;
}
