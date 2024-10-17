/*#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture TableroCentroTexture;
Texture Lamp;
Texture dado_8_Caras;

Model Lampara;
Model dado8Caras;
//Modelo de bicicleta
Model llantaTrasera;
Model llantaDelantera;
Model bicicleta;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		// Posición              // Coordenadas de textura    // Normal
		-35.0f, 0.0f, -35.0f,    0.0f, 0.0f,                 0.0f, -1.0f, 0.0f,  // Vértice 0
		35.0f, 0.0f, -35.0f,     1.0f, 0.0f,                 0.0f, -1.0f, 0.0f,  // Vértice 1
		-35.0f, 0.0f, 35.0f,     0.0f, 1.0f,                 0.0f, -1.0f, 0.0f,  // Vértice 2
		35.0f, 0.0f, 35.0f,      1.0f, 1.0f,                 0.0f, -1.0f, 0.0f   // Vértice 3
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		// RECORDEMOS QUE HAYD DOS PLANSO DEBEMO DE MODIFICAR LAS VALORES DE SUS NORMALES 
		// SE AGREGAN NORMALES A DONDE MIRA EL DADO
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, -1.03f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, -1.03f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,


	};
	
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);
	

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	
	TableroCentroTexture = Texture("Textures/TableroFinal_1.tga");
	TableroCentroTexture.LoadTextureA();

	Lamp = Texture("Textures/lamp.tga");
	Lamp.LoadTextureA();
	dado_8_Caras = Texture("Textures/Dado8carasFinal.tga");
	dado_8_Caras.LoadTextureA();

	Lampara = Model();
	Lampara.LoadModel("Models/Lampara.obj");
	dado8Caras = Model();
	dado8Caras.LoadModel("Models/Dado8Caras.obj");

	//Bicicleta
	llantaTrasera = Model();
	llantaTrasera.LoadModel("Models/llanta_trasera.obj");
	llantaDelantera= Model();
	llantaDelantera.LoadModel("Models/llanta_delantera.obj");
	bicicleta = Model();
	bicicleta.LoadModel("Models/bicicleta.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		6.0f, 1.5f, 0.0f,
		0.1f, 0.07f, 0.05f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	
	
	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	//Bicicleta animacion
	float posicionZ = -25.0f;  // Posición inicial en el eje Z (Inicio en casilla Gravity Falls)
	float velocidad = 4.0f;    // Velocidad de avance en Z
	float zObjetivo = 28.0f;   // Punto donde nos detenemos en z
	float radioRueda = 0.5f;    // Radio de las ruedas
	float rotacionRueda = 0.0f; // Ángulo acumulado de rotación de las ruedas
	float rotacionY = 0.0f;    // Ángulo acumulado de rotación sobre el eje Y
	float velocidadRotacion = 45.0f;  // Velocidad de rotación en grados por segundo
	float posicionX = -28.0f;  // Posición inicial en X
	float xObjetivo = 28.0f;   // Punto donde nos detenemos en x
	float zObjetivoNegativo = -28.0f;  // Punto donde nos detenemos en -z
	float xObjetivoNegativo = -28.0f;  // Punto donde nos detenemos en -x
	//Todos los límites son con respecto a como mi modelo sigue dentro del tablero
	int estado = 0;  // Control de fases: 0: Avanzar en Z positivo, 1: Girar a X, 2: Avanzar en X, 3: Girar a Z negativo, 4: Avanzar en Z negativo, 5: Gira a X negativo, 6: Avanza en X negativo y 7: Gira a Z positivo

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		//deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		switch (estado) {
			// Fase 1: Avanzar en Z positivo
		case 0:
			if (posicionZ < zObjetivo) {
				// Avanzar en Z
				posicionZ += velocidad * deltaTime;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotando las ruedas
			}
			else {
				estado = 1;  // Pasar al estado de girar hacia X
			}
			break;

			// Fase 2: Girar 90 grados para avanzar en X
		case 1:
			if (rotacionY < glm::radians(90.0f)) {
				// Girar 90 grados sobre el eje Y
				float anguloRotacion = glm::radians(velocidadRotacion) * deltaTime;
				rotacionY += anguloRotacion;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotando las ruedas mientras gira
			}
			else {
				rotacionY = glm::radians(90.0f);  // Asegurar que no pase de 90 grados
				estado = 2;  // Pasar al estado de avanzar en X
			}
			break;

			// Fase 3: Avanzar en X
		case 2:
			if (posicionX < xObjetivo) {
				// Avanzar en X
				posicionX += velocidad * deltaTime;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotar las ruedas
			}
			else {
				estado = 3;  // Pasar al estado de girar hacia Z negativo
			}
			break;

			// Fase 4: Girar otros 90 grados para orientarse en Z negativo
		case 3:
			if (rotacionY < glm::radians(180.0f)) {
				// Girar otros 90 grados (hasta 180 grados acumulados)
				float anguloRotacion = glm::radians(velocidadRotacion) * deltaTime;
				rotacionY += anguloRotacion;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotando las ruedas mientras gira
			}
			else {
				rotacionY = glm::radians(180.0f);  // Asegurar que no pase de 180 grados
				estado = 4;  // Pasar al estado de avanzar en Z negativo
			}
			break;

			// Fase 5: Avanzar en Z negativo
		case 4:
			if (posicionZ > zObjetivoNegativo) {
				// Avanzar en Z negativo
				posicionZ -= velocidad * deltaTime;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotar las ruedas
			}
			else {
				estado = 5;  // Pasar al estado de girar hacia X negativo
			}
			break;

			// Fase 6: Girar 90 grados adicionales para avanzar en X negativo
		case 5:
			if (rotacionY < glm::radians(270.0f)) {  // Ahora rotamos hasta 270 grados
				float anguloRotacion = glm::radians(velocidadRotacion) * deltaTime;
				rotacionY += anguloRotacion;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotando las ruedas mientras gira
			}
			else {
				rotacionY = glm::radians(270.0f);  // Fijar en 270 grados exactos
				estado = 6;  // Pasar al estado de avanzar en X negativo
			}
			break;

			// Fase 7: Avanzar en X negativo
		case 6:
			if (posicionX > xObjetivoNegativo) {
				// Avanzar en X negativo
				posicionX -= velocidad * deltaTime;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotar las ruedas
			}
			else {
				estado = 7;  // Pasar al estado de girar hacia Z positivo
			}
			break;

			// Fase 8: Girar 90 grados para orientarse hacia Z positivo y reiniciar
		case 7:
			if (rotacionY < glm::radians(360.0f)) {  // Rotamos hasta llegar a los 360 grados
				float anguloRotacion = glm::radians(velocidadRotacion) * deltaTime;
				rotacionY += anguloRotacion;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotando las ruedas mientras gira
			}
			else {
				rotacionY = 0.0f;  // Reiniciamos la rotación a 0 (equivalente a 360 grados)
				estado = 0;  // Volver al estado inicial para comenzar el ciclo otra vez
			}
			break;

		default:
			// Fin de la animación (no entra debido a que es un bucle
			break;
		}

		//Recibir eventos del usuario
		glfwPollEvents();
		// Movimiento de la cámara
		float velocidadCamara = 20.0f; //Incrementar velocidad
		camera.keyControl(mainWindow.getsKeys(), deltaTime * velocidadCamara);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
			glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		
		
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		if (mainWindow.getBandera() == GL_TRUE) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else if(mainWindow.getBandera() == GL_FALSE) {
			shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
		}


		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		TableroCentroTexture.UseTexture();
		meshList[2]->RenderMesh();

		//---------------Instanciando nuestra bici
		// Inicio (Casilla cabaña Gravity falls): (-28.0f, 3.9f, -25.0f) (Avanzar todo eje z hasta 25 en z)
		//-----------------Base
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posicionX, 3.9f, posicionZ));
		model = glm::rotate(model, rotacionY, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bicicleta.RenderModel();
		//-----------------Llanta delantera
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -1.9f, 3.4f));
		model = glm::rotate(model, rotacionRueda, glm::vec3(1.0f, 0.0f, 0.0f));  
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaDelantera.RenderModel();
		//-----------------Llanta trasera
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -1.9f, -5.8f));
		model = glm::rotate(model, rotacionRueda, glm::vec3(1.0f, 0.0f, 0.0f));  
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTrasera.RenderModel();



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}*/
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include <cstdlib>  // Para rand()
#include <ctime>    // Para time()
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movDado;
float movOffset;
float rotDadox;
float rotDadoxOffset;
float rotDadoy;
float rotDadoyOffset;
float rotDadoz;
float rotDadozOffset;
bool numRandom;
int randomNumber;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture Letrero;

Texture dado10Caras;
Texture TableroCentroTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model Arco;
Model Cartel;
//Modelo de bicicleta
Model llantaTrasera;
Model llantaDelantera;
Model bicicleta;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		// Posición              // Coordenadas de textura    // Normal
		-35.0f, 0.0f, -35.0f,    0.0f, 0.0f,                 0.0f, -1.0f, 0.0f,  // Vértice 0
		35.0f, 0.0f, -35.0f,     1.0f, 0.0f,                 0.0f, -1.0f, 0.0f,  // Vértice 1
		-35.0f, 0.0f, 35.0f,     0.0f, 1.0f,                 0.0f, -1.0f, 0.0f,  // Vértice 2
		35.0f, 0.0f, 35.0f,      1.0f, 1.0f,                 0.0f, -1.0f, 0.0f   // Vértice 3
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	// Definir los índices de las caras del decaedro (10 triángulos en total)
	unsigned int dado10_indices[] = {
		// Cada triángulo formado por 3 vértices
		0, 1, 2,    // Cara 1
		3, 4, 5,    // Cara 2
		6, 7, 8,    // Cara 3
		9, 10, 11,  // Cara 4
		12, 13, 14, // Cara 5
		15, 16, 17, // Cara 6
		18, 19, 20, // Cara 7
		21, 22, 23, // Cara 8
		24, 25, 26, // Cara 9
		27, 28, 29  // Cara 10
	};

	// Definir los vértices del decaedro con texturas (S, T) y normales (NX, NY, NZ)
	GLfloat dado10_vertices[] = {
		// Pirámide superior (5 caras, 15 vértices)
	   // Cara 1
	   // recordar que la normal vectorizada de un punto es v1 = p2 - p1 y v2 = p3 - p1 
	   // n = v1 x v2 y solo se normaliza con raiz
	   // x      y      z        S        T        NX       NY       NZ
	   0.0f,    0.25f,  0.0f,    0.60938f,   0.73828f,    -0.7278f, -0.4367f, -0.5288f,  // Vértice superior 0
	   0.3f,    0.0f,  0.0f,     0.72266f,   0.96094f,    -0.7278f, -0.4367f, -0.5288f,  // Vértice 1              // numero 4
	   0.0927f, 0.0f,  0.2853f,  0.87891f,   0.88281f,    -0.7278f, -0.4367f, -0.5288f,  // Vértice 2

	   // Cara 2
	   0.0f,    0.25f,  0.0f,    0.60938f,  0.73828f,    0.2781f, -0.4366f, -0.8556f,  // Vértice superior 3
	   0.0927f, 0.0f,  0.2853f,  0.38281f,  0.92969f,    0.2781f, -0.4366f, -0.8556f,  // Vértice 4              // numero 6
	  -0.2427f, 0.0f,  0.1763f,  0.56641f,  0.96484f, 	 0.2781f, -0.4366f, -0.8556f,  // Vértice 5

	  // Cara 3
	  0.0f,    0.25f,  0.0f,    0.60938f, 0.73828f,      0.8996f, -0.4367f, 0.0f,    // Vértice superior 6
	 -0.2427f, 0.0f,  0.1763f,  0.23828f, 0.72852f,      0.8996f, -0.4367f, 0.0f,    // Vértice 7                // numero 2
	 -0.2427f, 0.0f, -0.1763f,  0.29102f, 0.84766f,      0.8996f, -0.4367f, 0.0f,    // Vértice 8

	 // Cara 4
	 0.0f,    0.25f,  0.0f,    0.60938f, 0.73828f,       0.2781f, -0.4366f, 0.8556f,  // Vértice superior 9
	-0.2427f, 0.0f, -0.1763f,  0.38086f, 0.54102f,       0.2781f, -0.4366f, 0.8556f,  // Vértice 10              // numero 8
	 0.0927f, 0.0f, -0.2853f,  0.28125f, 0.67188f,       0.2781f, -0.4366f, 0.8556f,  // Vértice 11

	 // Cara 5
	 0.0f,    0.25f,  0.0f,     0.60938f, 0.73828f,     -0.7278f, -0.4367f, 0.5288f,  // Vértice superior 12
	 0.0927f, 0.0f, -0.2853f,   0.70703f, 0.51562f,     -0.7278f, -0.4367f, 0.5288f,  // Vértice 13               // numero 10
	 0.3f,    0.0f,  0.0f,      0.45312f, 0.52734f,     -0.7278f, -0.4367f, 0.5288f,  // Vértice 14

	 // Pirámide inferior (5 caras, 15 vértices)
	 // Cara 6
	 0.0f,   -0.25f,  0.0f,    0.42969f, 0.25781f,    -0.7278f,0.4367f, -0.5288f,  // Vértice superior 0
	 0.3f,    0.0f,  0.0f,     0.52734f, 0.49219f,    -0.7278f,0.4367f, -0.5288f,  // Vértice 1                 // numero 5 
	 0.0927f, 0.0f,  0.2853f,  0.33984f, 0.47656f ,   -0.7278f,0.4367f, -0.5288f,  // Vértice 2

	 // Cara 7
	 0.0f,   -0.25f,  0.0f,    0.42969f, 0.25781f,      0.2781f,0.4366f, -0.8556f,  // Vértice superior 3
	 0.0927f, 0.0f,  0.2853f,  0.20898f, 0.44531f,      0.2781f,0.4366f, -0.8556f,  // Vértice 4
	-0.2427f, 0.0f,  0.1763f,  0.11133f, 0.32617f,      0.2781f,0.4366f, -0.8556f,  // Vértice 5

	// Cara 8
	0.0f,   -0.25f,  0.0f,    0.42969f, 0.25781f,       0.8996f, 0.4367f, 0.0f,    // Vértice superior 6
   -0.2427f, 0.0f,  0.1763f,  0.07617f, 0.27344f,       0.8996f, 0.4367f, 0.0f,    // Vértice 7
   -0.2427f, 0.0f, -0.1763f,  0.13281f, 0.11914f,       0.8996f, 0.4367f, 0.0f,    // Vértice 8

   // Cara 9
   0.0f,   -0.25f,  0.0f,   0.42969f, 0.25781f,         0.2781f,0.4366f, 0.8556f,  // Vértice superior 9
  -0.2427f, 0.0f, -0.1763f, 0.18750f, 0.09180f,         0.2781f,0.4366f, 0.8556f,  // Vértice 10
   0.0927f, 0.0f, -0.2853f, 0.41406f, 0.01953f,         0.2781f,0.4366f, 0.8556f,  // Vértice 11

   // Cara 10
   0.0f,   -0.25f,  0.0f,    0.42969f, 0.25781f,       -0.7278f,0.4367f, 0.5288f,  // Vértice superior 12
   0.0927f, 0.0f, -0.2853f,  0.55273f, 0.03711f,       -0.7278f,0.4367f, 0.5288f,  // Vértice 13
   0.3f,    0.0f,  0.0f,     0.68945f, 0.09375f,       -0.7278f,0.4367f, 0.5288f,  // Vértice 14

	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* dado10 = new Mesh();
	dado10->CreateMesh(dado10_vertices, dado10_indices, 240, 30);
	meshList.push_back(dado10);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	Letrero = Texture("Textures/Letrero.tga");
	Letrero.LoadTextureA();
	dado10Caras = Texture("Textures/Dado_10Caras.tga");
	dado10Caras.LoadTextureA();

	TableroCentroTexture = Texture("Textures/TableroFinal_1.tga");
	TableroCentroTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	//Bicicleta
	llantaTrasera = Model();
	llantaTrasera.LoadModel("Models/llanta_trasera.obj");
	llantaDelantera = Model();
	llantaDelantera.LoadModel("Models/llanta_delantera.obj");
	bicicleta = Model();
	bicicleta.LoadModel("Models/bicicleta.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	movCoche = 0.0f;
	movDado = 0.0f;
	movOffset = 0.05f;
	rotDadox = 0.0f;
	rotDadoxOffset = 3.0f;
	rotDadoy = 0.0f;
	rotDadoyOffset = 3.0f;
	rotDadoz = 0.0f;
	rotDadozOffset = 3.0f;
	numRandom = false;  

	//Bicicleta animacion
	float posicionZ = -25.0f;  // Posición inicial en el eje Z (Inicio en casilla Gravity Falls)
	float velocidad = 4.0f;    // Velocidad de avance en Z
	float zObjetivo = 28.0f;   // Punto donde nos detenemos en z
	float radioRueda = 0.5f;    // Radio de las ruedas
	float rotacionRueda = 0.0f; // Ángulo acumulado de rotación de las ruedas
	float rotacionY = 0.0f;    // Ángulo acumulado de rotación sobre el eje Y
	float velocidadRotacion = 45.0f;  // Velocidad de rotación en grados por segundo
	float posicionX = -28.0f;  // Posición inicial en X
	float xObjetivo = 28.0f;   // Punto donde nos detenemos en x
	float zObjetivoNegativo = -28.0f;  // Punto donde nos detenemos en -z
	float xObjetivoNegativo = -28.0f;  // Punto donde nos detenemos en -x
	//Todos los límites son con respecto a como mi modelo sigue dentro del tablero
	int estado = 0;  // Control de fases: 0: Avanzar en Z positivo, 1: Girar a X, 2: Avanzar en X, 3: Girar a Z negativo, 4: Avanzar en Z negativo, 5: Gira a X negativo, 6: Avanza en X negativo y 7: Gira a Z positivo


	while (!mainWindow.getShouldClose())
	{
		//Ejercicio 1
		/*GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS; 
		lastTime = now;



		if (mainWindow.getAvanza() == true) {
			// Inicializar el generador de números aleatorios solo una vez
			static bool initialized = false;

			if (!initialized) {
				std::srand(static_cast<unsigned int>(std::time(nullptr)));
				initialized = true;
			}
			if (!numRandom) {
				randomNumber = (std::rand() % 10) + 1;
				//randomNumber = 3;
				numRandom = true;
			}

			if (movDado > -7.0f) {
				movDado -= movOffset * deltaTime;
			}



			switch (randomNumber) {

			case 1:
				if (rotDadoz < 360 + 45) {
					rotDadoz += rotDadozOffset * deltaTime;
				}
				if (rotDadoy < 360) {
					rotDadoy += rotDadoyOffset * deltaTime;
				}
				break;
			case 2:

				if (rotDadoy < 360 + 15) {
					rotDadoy += rotDadoyOffset * deltaTime;
				}
				if (rotDadox < 360 + 45) {
					rotDadox += rotDadoxOffset * deltaTime;
				}

				break;
			case 3:
				if (rotDadox < 360) {
					rotDadox += rotDadoxOffset * deltaTime;
				}
				if (rotDadoz > -45) {
					rotDadoz -= rotDadozOffset * deltaTime;
				}
				if (rotDadoy < 360 + 35) {
					rotDadoy += rotDadoyOffset * deltaTime;
				}

				break;
			case 4:

				if (rotDadox > -360 - 45) {
					rotDadox -= rotDadoxOffset * deltaTime;
				}
				if (rotDadoy < 360 + 50) {
					rotDadoy += rotDadoyOffset * deltaTime;
				}

				break;

			case 5:
				if (rotDadox > -360 - 45) {
					rotDadox -= rotDadozOffset * deltaTime;
				}
				if (rotDadoy > -360 - 20) {
					rotDadoy -= rotDadoyOffset * deltaTime;
				}
				break;
			case 6:
				if (rotDadox < 180) {
					rotDadox += rotDadoxOffset * deltaTime;
				}
				if (rotDadoz < 360 + 45) {
					rotDadoz += rotDadozOffset * deltaTime;
				}
				if (rotDadoy < 360 + 35) {
					rotDadoy += rotDadoyOffset * deltaTime;   
				}
				break;
			case 7:
				if (rotDadox < 180) {
					rotDadox += rotDadoxOffset * deltaTime;
				}
				if (rotDadoz > -360 - 45) {
					rotDadoz -= rotDadozOffset * deltaTime;
				}
				if (rotDadoy < 360 + 75) {
					rotDadoy += rotDadoyOffset * deltaTime;
				}
				break;

			case 8:
				if (rotDadox < 180) {
					rotDadox += rotDadoxOffset * deltaTime;
				}
				if (rotDadoz > -360 - 45) {
					rotDadoz -= rotDadozOffset * deltaTime;
				}
				if (rotDadoy < 360 + 5) {
					rotDadoy += rotDadoyOffset * deltaTime;
				}
				break;
			case 9:
				if (rotDadox < 180) {
					rotDadox += rotDadoxOffset * deltaTime;
				}
				if (rotDadoz < 360 + 45) {
					rotDadoz += rotDadozOffset * deltaTime;
				}
				if (rotDadoy > -360 - 35) {
					rotDadoy -= rotDadoyOffset * deltaTime;//
				}
				break;

			case 10:
				if (rotDadoz < 360) {
					rotDadoz += rotDadozOffset * deltaTime;
				}
				if (rotDadox < 180 + 45) {
					rotDadox += rotDadoxOffset * deltaTime;
				}
				if (rotDadoy > -360 - 20) {
					rotDadoy -= rotDadoyOffset * deltaTime;
				}
				break;
			}


		}

		else {
			movDado = 0.0f;
			movOffset = 0.05f;
			rotDadox = 0.0f;
			rotDadoxOffset = 3.0f;
			rotDadoy = 0.0f;
			rotDadoyOffset = 3.0f;
			rotDadoz = 0.0f;
			rotDadozOffset = 3.0f;
			numRandom = false;

		}

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());*/
		
		//Ejercicio 2
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		//deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		switch (estado) {
			// Fase 1: Avanzar en Z positivo
		case 0:
			if (posicionZ < zObjetivo) {
				// Avanzar en Z
				posicionZ += velocidad * deltaTime;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotando las ruedas
			}
			else {
				estado = 1;  // Pasar al estado de girar hacia X
			}
			break;

			// Fase 2: Girar 90 grados para avanzar en X
		case 1:
			if (rotacionY < glm::radians(90.0f)) {
				// Girar 90 grados sobre el eje Y
				float anguloRotacion = glm::radians(velocidadRotacion) * deltaTime;
				rotacionY += anguloRotacion;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotando las ruedas mientras gira
			}
			else {
				rotacionY = glm::radians(90.0f);  // Asegurar que no pase de 90 grados
				estado = 2;  // Pasar al estado de avanzar en X
			}
			break;

			// Fase 3: Avanzar en X
		case 2:
			if (posicionX < xObjetivo) {
				// Avanzar en X
				posicionX += velocidad * deltaTime;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotar las ruedas
			}
			else {
				estado = 3;  // Pasar al estado de girar hacia Z negativo
			}
			break;

			// Fase 4: Girar otros 90 grados para orientarse en Z negativo
		case 3:
			if (rotacionY < glm::radians(180.0f)) {
				// Girar otros 90 grados (hasta 180 grados acumulados)
				float anguloRotacion = glm::radians(velocidadRotacion) * deltaTime;
				rotacionY += anguloRotacion;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotando las ruedas mientras gira
			}
			else {
				rotacionY = glm::radians(180.0f);  // Asegurar que no pase de 180 grados
				estado = 4;  // Pasar al estado de avanzar en Z negativo
			}
			break;

			// Fase 5: Avanzar en Z negativo
		case 4:
			if (posicionZ > zObjetivoNegativo) {
				// Avanzar en Z negativo
				posicionZ -= velocidad * deltaTime;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotar las ruedas
			}
			else {
				estado = 5;  // Pasar al estado de girar hacia X negativo
			}
			break;

			// Fase 6: Girar 90 grados adicionales para avanzar en X negativo
		case 5:
			if (rotacionY < glm::radians(270.0f)) {  // Ahora rotamos hasta 270 grados
				float anguloRotacion = glm::radians(velocidadRotacion) * deltaTime;
				rotacionY += anguloRotacion;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotando las ruedas mientras gira
			}
			else {
				rotacionY = glm::radians(270.0f);  // Fijar en 270 grados exactos
				estado = 6;  // Pasar al estado de avanzar en X negativo
			}
			break;

			// Fase 7: Avanzar en X negativo
		case 6:
			if (posicionX > xObjetivoNegativo) {
				// Avanzar en X negativo
				posicionX -= velocidad * deltaTime;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotar las ruedas
			}
			else {
				estado = 7;  // Pasar al estado de girar hacia Z positivo
			}
			break;

			// Fase 8: Girar 90 grados para orientarse hacia Z positivo y reiniciar
		case 7:
			if (rotacionY < glm::radians(360.0f)) {  // Rotamos hasta llegar a los 360 grados
				float anguloRotacion = glm::radians(velocidadRotacion) * deltaTime;
				rotacionY += anguloRotacion;
				float distanciaRecorrida = velocidad * deltaTime;
				rotacionRueda += (distanciaRecorrida / radioRueda);  // Rotando las ruedas mientras gira
			}
			else {
				rotacionY = 0.0f;  // Reiniciamos la rotación a 0 (equivalente a 360 grados)
				estado = 0;  // Volver al estado inicial para comenzar el ciclo otra vez
			}
			break;

		default:
			// Fin de la animación (no entra debido a que es un bucle
			break;
		}

		//Recibir eventos del usuario
		glfwPollEvents();
		// Movimiento de la cámara
		float velocidadCamara = 20.0f; //Incrementar velocidad
		camera.keyControl(mainWindow.getsKeys(), deltaTime * velocidadCamara);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
		shaderList[0].SetSpotLights(spotLights, spotLightCount - 1);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		//Tablero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -.90f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		TableroCentroTexture.UseTexture();
		meshList[2]->RenderMesh();
		//Piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//-------------Dado
		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		//inciamos en 8
		model = glm::translate(model, glm::vec3(0.0f, 8.0f + movDado, 0.0f));
		model = glm::rotate(model, rotDadox * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotDadoy * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotDadoz * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado10Caras.UseTexture();
		meshList[4]->RenderMesh();

		//---------------Instanciando nuestra bici
		// Inicio (Casilla cabaña Gravity falls): (-28.0f, 3.9f, -25.0f) (Avanzar todo eje z hasta 25 en z)
		//-----------------Base
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posicionX, 4.0f, posicionZ));
		model = glm::rotate(model, rotacionY, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bicicleta.RenderModel();
		//-----------------Llanta delantera
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -1.9f, 3.4f));
		model = glm::rotate(model, rotacionRueda, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaDelantera.RenderModel();
		//-----------------Llanta trasera
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -1.9f, -5.8f));
		model = glm::rotate(model, rotacionRueda, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTrasera.RenderModel();

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

