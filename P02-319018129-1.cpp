//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

void CrearLetrasyFiguras()
{	//--------------------- LETRA Z  (definimos 18 vertices; 18x6=108) ------------------------
	GLfloat vertices_z[] = {	
			//Parte de arriba de la Z (6)
			//X			Y			Z			R		G		B
			0.0f,	0.9f,		0.5f,			1.0f,	1.0f,	0.0f, //Esquina superior izquierda
			0.6f,	0.9f,		0.5f,			1.0f,	1.0f,	0.0f, //Esquina superior derecha
			0.0f,	0.7f,		0.5f,			1.0f,	1.0f,	0.0f, //Esquina inferior izquierda
			0.6f,	0.9f,		0.5f,			1.0f,	1.0f,	0.0f, //Esquina superior derecha
			0.0f,	0.7f,		0.5f,			1.0f,	1.0f,	0.0f, //Esquina inferior izquierda
			0.6f,   0.7f,       0.5f,           1.0f,	1.0f,	0.0f, //Esquina inferior derecha

			//Parte de enmedio de la Z (6)
			//X			Y			Z			R		G		B
			0.4f,	0.7f,		0.5f,			1.0f,	1.0f,	0.0f, //Esquina superior izquierda
			0.6f,   0.7f,       0.5f,           1.0f,	1.0f,	0.0f, //Esquina superior derecha
			0.0f,   0.1f,       0.5f,           1.0f,	1.0f,	0.0f, //Esquina inferior izquierda
			0.6f,   0.7f,       0.5f,           1.0f,	1.0f,	0.0f, //Esquina superior derecha
			0.0f,   0.1f,       0.5f,           1.0f,	1.0f,	0.0f, //Esquina inferior izquierda
			0.2f,   0.1f,       0.5f,           1.0f,	1.0f,	0.0f, //Esquina inferior derecha

			//Parte de abajo de la Z (6)
			//X			Y			Z			R		G		B
			0.0f,   0.1f,       0.5f,           1.0f,	1.0f,	0.0f, //Esquina superior izquierda
			0.6f,   0.1f,       0.5f,           1.0f,	1.0f,	0.0f, //Esquina superior derecha
			0.0f,  -0.1f,       0.5f,           1.0f,	1.0f,	0.0f, //Esquina inferior izquierda
			0.6f,   0.1f,       0.5f,           1.0f,	1.0f,	0.0f, //Esquina superior derecha
			0.0f,  -0.1f,       0.5f,           1.0f,	1.0f,	0.0f, //Esquina inferior izquierda
			0.6f,  -0.1f,       0.5f,           1.0f,	1.0f,	0.0f, //Esquina inferior derecha
	};
	MeshColor *letrasZ = new MeshColor();
	letrasZ->CreateMeshColor(vertices_z,108);
	meshColorList.push_back(letrasZ);

	//--------------------- LETRA C  (definimos 18 vertices; 18x6=108) ------------------------
	GLfloat vertices_c[] = {
		//Parte de arriba de la C (6)
		//X			Y			Z			R		G		B
		0.0f,	0.9f,		0.5f,			0.0f,	1.0f,	1.0f, //Esquina superior izquierda
		0.6f,	0.9f,		0.5f,			0.0f,	1.0f,	1.0f, //Esquina superior derecha
		0.0f,	0.7f,		0.5f,			0.0f,	1.0f,	1.0f, //Esquina inferior izquierda
		0.6f,	0.9f,		0.5f,			0.0f,	1.0f,	1.0f, //Esquina superior derecha
		0.0f,	0.7f,		0.5f,			0.0f,	1.0f,	1.0f, //Esquina inferior izquierda
		0.6f,   0.7f,       0.5f,			0.0f,	1.0f,	1.0f, //Esquina inferior derecha

		//Parte de enmedio de la C (6)
		0.0f,	0.7f,		0.5f,			0.0f,	1.0f,	1.0f, //Esquina superior izquierda
		0.17f,  0.7f,       0.5f,			0.0f,	1.0f,	1.0f, //Esquina superior derecha
		0.0f,   0.3f,       0.5f,			0.0f,	1.0f,	1.0f, //Esquina inferior izquierda
		0.17f,  0.7f,       0.5f,			0.0f,	1.0f,	1.0f, //Esquina superior derecha
		0.0f,   0.3f,       0.5f,			0.0f,	1.0f,	1.0f, //Esquina inferior izquierda
		0.17f,  0.3f,       0.5f,			0.0f,	1.0f,	1.0f, //Esquina inferior derecha

		//Parte de abajo de la C (6)
		0.0f,   0.3f,       0.5f,			0.0f,	1.0f,	1.0f, //Esquina superior izquierda
		0.6f,   0.3f,       0.5f,			0.0f,	1.0f,	1.0f, //Esquina superior derecha
		0.0f,   0.1f,       0.5f,			0.0f,	1.0f,	1.0f, //Esquina inferior izquierda
		0.6f,   0.3f,       0.5f,			0.0f,	1.0f,	1.0f, //Esquina superior derecha
		0.0f,   0.1f,       0.5f,			0.0f,	1.0f,	1.0f, //Esquina inferior izquierda
		0.6f,   0.1f,       0.5f,			0.0f,	1.0f,	1.0f, //Esquina inferior derecha
		
	};
	MeshColor* letrasC = new MeshColor();
	letrasC->CreateMeshColor(vertices_c, 108);
	meshColorList.push_back(letrasC);

	//--------------------- LETRA A  (definimos  vertices; x6=) ------------------------
	GLfloat vertices_a[] = {
		//Parte de arriba de la A (6)
		//X			Y			Z			R		G		B
		0.0f,	0.9f,		0.5f,			1.0f,	0.0f,	1.0f, //Esquina superior izquierda
		0.4f,	0.9f,		0.5f,			1.0f,	0.0f,	1.0f, //Esquina superior derecha
		0.0f,	0.7f,		0.5f,			1.0f,	0.0f,	1.0f, //Esquina inferior izquierda
		0.4f,	0.9f,		0.5f,			1.0f,	0.0f,	1.0f, //Esquina superior derecha
		0.0f,	0.7f,		0.5f,			1.0f,	0.0f,	1.0f, //Esquina inferior izquierda
		0.4f,   0.7f,       0.5f,			1.0f,	0.0f,	1.0f, //Esquina inferior derecha

		//Parte izquierda de la A (6)
		0.0f,	0.9f,		0.5f,			1.0f,	0.0f,	1.0f, //Esquina superior izquierda
	   0.12f,   0.9f,		0.5f,			1.0f,	0.0f,	1.0f, //Esquina superior derecha
		0.0f,  -0.1f,       0.5f,           1.0f,	0.0f,	1.0f, //Esquina inferior izquierda
	   0.12f,   0.9f,		0.5f,			1.0f,	0.0f,	1.0f, //Esquina superior derecha
		0.0f,  -0.1f,       0.5f,           1.0f,	0.0f,	1.0f, //Esquina inferior izquierda
	   0.12f,  -0.1f,       0.5f,           1.0f,	0.0f,	1.0f, //Esquina inferior derecha
		
	   //Parte derecha de la A (6)
		0.4f,   0.7f,       0.5f,			1.0f,	0.0f,	1.0f, //Esquina superior derecha
	    0.28f,  0.7f,       0.5f,			1.0f,	0.0f,	1.0f, //Esquina superior izquierda
		0.4f,  -0.1f,       0.5f,           1.0f,	0.0f,	1.0f, //Esquina inferior derecha
		0.28f,  0.7f,       0.5f,			1.0f,	0.0f,	1.0f, //Esquina superior izquierda
		0.4f,  -0.1f,       0.5f,           1.0f,	0.0f,	1.0f, //Esquina inferior derecha
		0.28f, -0.1f,       0.5f,           1.0f,	0.0f,	1.0f, //Esquina inferior izquierda

		//Parte de enmedio de la A ()
		0.12f,	0.5f,		0.5f,			1.0f,	0.0f,	1.0f, //Esquina superior izquierda
		0.28f,  0.5f,		0.5f,			1.0f,	0.0f,	1.0f, //Esquina superior derecha
		0.12f,  0.3f,       0.5f,           1.0f,	0.0f,	1.0f, //Esquina inferior izquierda
		0.28f,  0.5f,		0.5f,			1.0f,	0.0f,	1.0f, //Esquina superior derecha
		0.12f,  0.3f,       0.5f,           1.0f,	0.0f,	1.0f, //Esquina inferior izquierda
		0.28f,  0.3f,       0.5f,           1.0f,	0.0f,	1.0f, //Esquina inferior derecha
	};
	MeshColor* letrasA = new MeshColor();
	letrasA->CreateMeshColor(vertices_a, 144);
	meshColorList.push_back(letrasA);
}


void CreateShaders()
{
	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(1200, 680);
	mainWindow.Initialise();

	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
													
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		
		//-------------------- Renderizando letra Z ----------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.9f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		//-------------------- Renderizando letra C ----------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.2f, -1.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();

		//-------------------- Renderizando letra A ----------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();
		

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/