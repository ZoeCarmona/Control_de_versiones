/*
Práctica 8: Iluminación 2
*/
//para cargar imagen
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
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
//Creando instancias separadas de cada parte del carro
Model Cuerpo;
Model Cofre;
Model LlantaDerDelantera;
Model LlantaIzqDelantera;
Model LlantaDerTrasera;
Model LlantaIzqTrasera;
//Instancias Lámpara
Model lampara;
//Instancias otro modelo
Model moto;


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
PointLight pointLights1[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLights1[MAX_SPOT_LIGHTS];
SpotLight spotLights2[MAX_SPOT_LIGHTS];

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
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
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
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	//Importando parte por parte
	Cuerpo = Model();
	Cofre = Model();
	LlantaDerDelantera = Model();
	LlantaIzqDelantera = Model();
	LlantaDerTrasera = Model();
	LlantaIzqTrasera = Model();
	Cuerpo.LoadModel("Models/Cuerpo.obj");
	Cofre.LoadModel("Models/Cofre.obj");
	LlantaDerDelantera.LoadModel("Models/Llanta_derecha_delantera.obj");
	LlantaIzqDelantera.LoadModel("Models/Llanta_izquierda_delantera.obj");
	LlantaDerTrasera.LoadModel("Models/Llanta_derecha_trasera.obj");
	LlantaIzqTrasera.LoadModel("Models/Llanta_izquierda_trasera.obj");
	//Importando lámpara
	lampara = Model();
	lampara.LoadModel("Models/lampara.obj");
	//Importando moto
	moto = Model();
	moto.LoadModel("Models/moto.obj");
	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	//Matriz auxiliar para el cuerpo
	glm::mat4 modelcuerpo(1.0);


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
		-15.0f, 3.0f, 10.0f, //Posición
		0.1f, 0.07f, 0.05f);
	pointLightCount++;

	//Luz moto
	unsigned int pointLightCount1 = 0;
	//Declaración de primer luz puntual
	pointLights1[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		-25.0f, 3.0f, 0.0f, //Posición
		0.1f, 0.07f, 0.05f);
	pointLightCount1++;
	//Nuevas luces puntuales*/

	//-------------- Luz para el cofre del carro
	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(0.0f, 1.0f, 0.0f, //Verde
		0.5f, 2.0f,        
		0.0f, 0.0f, 0.0f,  // Posición inicial del Spotlight (se actualiza luego)
		0.0f, -1.0f, 0.0f, 
		1.0f, 0.01f, 0.01f,
		20.0f);
	spotLightCount++;

	unsigned int spotLightCountLuzCoche = 0;
	//-------------- Luz para una linterna del carro (izquierda delantera)

	spotLights1[0] = SpotLight(0.0f, 0.0f, 1.0f,  // Color Azul
		1.5f, 5.0f,        // Incremento significativo en las componentes ambiental y difusa
		0.0f, 0.0f, 0.0f,  // Posición inicial
		0.0f, 0.0f, 1.0f,  // Dirección hacia adelante
		1.0f, 0.03f, 0.005f,  // Menor atenuación para mantener la intensidad a distancias cortas
		15.0f);            // Ángulo de apertura
	//-------------- Luz para una linterna del carro (derecha trasera)
	spotLights2[0] = SpotLight(0.0f, 0.0f, 1.0f,  // Color Azul
		1.5f, 5.0f,        // Incremento significativo en las componentes ambiental y difusa
		0.0f, 0.0f, 0.0f,  // Posición inicial
		0.0f, 0.0f, -1.0f,  // Dirección hacia adelante
		1.0f, 0.03f, 0.005f,  // Menor atenuación para mantener la intensidad a distancias cortas
		15.0f);            // Ángulo de apertura
	spotLightCountLuzCoche++;
	
	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		//Obtenemos de cámara y vemos donde esta para su vector de visualizacion
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
		/*	glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f; 
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection()); //Apuntando en direccion de la camara 
		*/

		// Actualizar el faro frontal azul con la posición y dirección del coche
		glm::vec3 carPosition = glm::vec3(modelcuerpo[3][0], modelcuerpo[3][1], modelcuerpo[3][2]); // Posición del coche
		glm::vec3 carDirection = glm::normalize(glm::vec3(modelcuerpo[2][0], modelcuerpo[2][1], modelcuerpo[2][2])); // Dirección hacia adelante del coche
		// Actualizar la posición y la dirección del faro azul
		//spotLights1[0].SetFlash(carPosition + glm::vec3(-2.5f, 0.7f, 4.5f), carDirection); //5.5
		//spotLights2[0].SetFlash(carPosition + glm::vec3(-2.5f, 0.6f, -4.5f), -carDirection);

		//Cambio luces del carro
		if (mainWindow.getLuzDelantera() == true) {
			spotLights1[0].SetFlash(carPosition + glm::vec3(-2.5f, 0.7f, 4.5f), carDirection); //5.5
			shaderList[0].SetSpotLights(spotLights1, spotLightCountLuzCoche);


		}
		else if (mainWindow.getLuzTrasera() == true) {
			spotLights2[0].SetFlash(carPosition + glm::vec3(-2.5f, 0.6f, -4.5f), -carDirection);
			shaderList[0].SetSpotLights(spotLights2, spotLightCountLuzCoche);

		}
		else
		{
			shaderList[0].SetSpotLights(spotLights, spotLightCount);
		}

		//Lámpara
		if (mainWindow.getBandera() == GL_TRUE) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else if (mainWindow.getBandera() == GL_FALSE) {
			shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
		}

		//Moto
		if (mainWindow.getFlag() == GL_TRUE) {
			shaderList[0].SetPointLights(pointLights1, pointLightCount1);
		}
		else if (mainWindow.getFlag() == GL_FALSE) {
			shaderList[0].SetPointLights(pointLights1, pointLightCount1 - 1);
		}

		//Establecemos la posición inicial de nuestra luz del cofre
		glm::vec3 posicionBaseLuzCofre = glm::vec3(0.0f, 0.0f, 3.0f); 

		


		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetPointLights(pointLights, pointLightCount);
		

		//shaderList[0].SetSpotLights(spotLights, spotLightCount);

		


		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		//Material donde todos los modelos se aplican a los objetos
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();
	
		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
		//Carro
		//-----------------Cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.02f, mainWindow.getPosicionCarroZ()));
		modelcuerpo = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cuerpo.RenderModel();
		//-----------------Cofre
		model = modelcuerpo;
		model = glm::translate(model, glm::vec3(-0.2f, 1.23f, 3.0f));  
		//Tecla F abre, Tecla G cierra
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));

		// Actualizar la posición y dirección del SpotLight del cofre
		// Calcular la posición del SpotLight 
		glm::vec3 cofrePosition = glm::vec3(model * glm::vec4(posicionBaseLuzCofre, 1.0f));
		glm::vec3 cofreDirection = glm::normalize(-glm::vec3(model[1][0], model[1][1], model[1][2]));
		spotLights[0].SetFlash(cofrePosition, cofreDirection);

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre.RenderModel();
		//-----------------Llanta derecha delantera
		model = modelcuerpo;
		model = glm::translate(model, glm::vec3(2.3f, -0.8f, 4.3f));  
		//Tecla H adelante, Tecla J atrás
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaDerDelantera.RenderModel();
		//--------------------Llanta izquierda delantera
		model = modelcuerpo;
		model = glm::translate(model, glm::vec3(-2.6f, -0.8f, 4.3f));
		//Tecla H adelante, Tecla J atrás
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaIzqDelantera.RenderModel();
		//------------------Llanta derecha trasera
		model = modelcuerpo;
		model = glm::translate(model, glm::vec3(2.5f, -0.86f, -3.5f));
		//Tecla H adelante, Tecla J atrás
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaDerTrasera.RenderModel();
		//--------------------Llanta izquierda trasera
		model = modelcuerpo;
		model = glm::translate(model, glm::vec3(-3.0f, -0.88f, -3.5f));
		//Tecla H adelante, Tecla J atrás
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaIzqTrasera.RenderModel();

		//Lámpara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, -0.9f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		//Moto
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, 1.3f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		moto.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
