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

//Pirámide triangular regular
void CreaPiramide()
{
	//-------------------- TECHO --------------------------
		unsigned int techo[] = { 
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3
		
		};
		GLfloat techo_vertices[] = {
			-0.5f, -0.5f,0.0f,	//Esquina inferior izquierda [0]
			0.5f,-0.5f,0.0f,	//Esquina inferior derecha [1]
			0.0f, 0.2f, -0.25f,	//Punta [2]
			0.0f,-0.5f,-0.5f,	//Esquina inferior central trasera [3]

		};
		Mesh *tech = new Mesh();
		tech->CreateMesh(techo_vertices, techo, 12, 12);
		meshList.push_back(tech);

	//----------------------- PINO IZQUIERDO -----------------------------
		unsigned int hojaIzquierda[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

		};
		GLfloat hojaIzquierda_vertices[] = {
			-0.2f, -0.5f,0.0f,	//Esquina inferior izquierda [0]
			0.2f,-0.5f,0.0f,	//Esquina inferior derecha [1]
			0.0f, 0.2f, -0.25f,	//Punta [2]
			0.0f,-0.5f,-0.5f,	//Esquina inferior central trasera [3]

		};
		Mesh* hIzq = new Mesh();
		hIzq->CreateMesh(hojaIzquierda_vertices, hojaIzquierda, 12, 12);
		meshList.push_back(hIzq);

	//----------------------- PINO DERECHO   -----------------------------
		unsigned int hojaDerecha[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

		};
		GLfloat hojaDerecha_vertices[] = {
			-0.2f, -0.5f,0.0f,	//Esquina inferior izquierda [0]
			0.2f,-0.5f,0.0f,	//Esquina inferior derecha [1]
			0.0f, 0.2f, -0.25f,	//Punta [2]
			0.0f,-0.5f,-0.5f,	//Esquina inferior central trasera [3]

		};
		Mesh* hDer = new Mesh();
		hDer->CreateMesh(hojaDerecha_vertices, hojaDerecha, 12, 12);
		meshList.push_back(hDer);
}

//Vértices de un cubo
void CrearCubo()
{
	//-------------------------- VENTANA IZQUIERDA VERDE ------------------------------
	unsigned int ventanaIzq[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat ventanaIzquierda_vertices[] = {
		// front
		-0.1f, -0.5f,  0.5f, //Esquina inferior izquierda [0]
		0.15f, -0.5f,  0.5f, //Esquina inferior derecha [1]
		0.15f,  -0.15f,  0.5f, //Esquina superior derecha [2]
		-0.1f,  -0.15f,  0.5f, //Esquina superior izquierda [3]
		// back
		-0.1f, -0.5f, -0.5f, //[4]
		0.15f, -0.5f, -0.5f,//[5]
		0.15f,  -0.15f, -0.5f,//[6]
		-0.1f,  -0.15f, -0.5f//[7]
	};
	Mesh* ventIzq = new Mesh();
	ventIzq->CreateMesh(ventanaIzquierda_vertices, ventanaIzq, 24, 36);
	meshList.push_back(ventIzq);

	//--------------------------- VENTANA DERECHA -----------------------------------
	unsigned int ventanaDer[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat ventanaDerecha_vertices[] = {
		// front
		-0.1f, -0.5f,  0.5f, //Esquina inferior izquierda [0]
		0.15f, -0.5f,  0.5f, //Esquina inferior derecha [1]
		0.15f,  -0.15f,  0.5f, //Esquina superior derecha [2]
		-0.1f,  -0.15f,  0.5f, //Esquina superior izquierda [3]
		// back
		-0.1f, -0.5f, -0.5f, //[4]
		0.15f, -0.5f, -0.5f,//[5]
		0.15f,  -0.15f, -0.5f,//[6]
		-0.1f,  -0.15f, -0.5f//[7]
	};
	Mesh* ventDer = new Mesh();
	ventDer->CreateMesh(ventanaDerecha_vertices, ventanaDer, 24, 36);
	meshList.push_back(ventDer);

	//------------------------------ PUERTA -------------------------------------
	unsigned int puerta[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat puerta_vertices[] = {
		// front
		-0.1f, -0.5f,  0.5f, //Esquina inferior izquierda [0]
		0.15f, -0.5f,  0.5f, //Esquina inferior derecha [1]
		0.15f,  -0.15f,  0.5f, //Esquina superior derecha [2]
		-0.1f,  -0.15f,  0.5f, //Esquina superior izquierda [3]
		// back
		-0.1f, -0.5f, -0.5f, //[4]
		0.15f, -0.5f, -0.5f,//[5]
		0.15f,  -0.15f, -0.5f,//[6]
		-0.1f,  -0.15f, -0.5f//[7]
	};
	Mesh* puert = new Mesh();
	puert->CreateMesh(puerta_vertices, puerta, 24, 36);
	meshList.push_back(puert);

	//-------------------------- CASA ROJA --------------------------------------------
	unsigned int casa[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat casa_vertices[] = {
		// front
		-0.4f, -0.5f,  0.5f, //Esquina inferior izquierda [0]
		0.4f, -0.5f,  0.5f, //Esquina inferior derecha [1]
		0.4f,  0.7f,  0.5f, //Esquina superior derecha [2]
		-0.4f,  0.7f,  0.5f, //Esquina superior izquierda [3]
		// back
		-0.4f, -0.5f, -0.5f, //[4]
		0.4f, -0.5f, -0.5f,//[5]
		0.4f,  0.7f, -0.5f,//[6]
		-0.4f,  0.7f, -0.5f//[7]
	};
	Mesh *casaRoja = new Mesh();
	casaRoja->CreateMesh(casa_vertices, casa,24, 36);
	meshList.push_back(casaRoja);

	//---------------------- TRONCO IZQUIERDO -------------------------------
	unsigned int troncoIzquierdo[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat troncoIzqui_vertices[] = {
		// front
		-0.2f, -0.5f,  0.5f, //Esquina inferior izquierda [0]
		0.0f, -0.5f,  0.5f, //Esquina inferior derecha [1]
		0.0f,  -0.3f,  0.5f, //Esquina superior derecha [2]
		-0.2f,  -0.3f,  0.5f, //Esquina superior izquierda [3]
		// back
		-0.2f, -0.5f, -0.5f, //[4]
		0.0f, -0.5f, -0.5f,//[5]
		0.0f,  -0.3f, -0.5f,//[6]
		-0.2f, -0.3f, -0.5f//[7]
	};
	Mesh* troncoIzq = new Mesh();
	troncoIzq->CreateMesh(troncoIzqui_vertices, troncoIzquierdo, 24, 36);
	meshList.push_back(troncoIzq);

	//--------------- TRONCO DERECHO ----------------------------
	unsigned int troncoDerecho[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat troncoDer_vertices[] = {
		// front
		-0.2f, -0.5f,  0.5f, //Esquina inferior izquierda [0]
		0.0f, -0.5f,  0.5f, //Esquina inferior derecha [1]
		0.0f,  -0.3f,  0.5f, //Esquina superior derecha [2]
		-0.2f,  -0.3f,  0.5f, //Esquina superior izquierda [3]
		// back
		-0.2f, -0.5f, -0.5f, //[4]
		0.0f, -0.5f, -0.5f,//[5]
		0.0f,  -0.3f, -0.5f,//[6]
		-0.2f, -0.3f, -0.5f//[7]
	};
	Mesh* tronDer = new Mesh();
	tronDer->CreateMesh(troncoDer_vertices, troncoDerecho, 24, 36);
	meshList.push_back(tronDer);
}


void CreateShaders()
{
	//Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	//shader1->CreateFromFiles(vShader, fShader);
	//shaderList.push_back(*shader1);
	
	// Shader rojo [0]
	Shader *shaderRojo = new Shader();
	shaderRojo->CreateFromFiles("shaders/shader.vert", "shaders/rojo.frag");
	shaderList.push_back(*shaderRojo);

	// Shader verde [1]
	Shader* shaderVerde = new Shader();
	shaderVerde->CreateFromFiles("shaders/shader.vert", "shaders/verde.frag");
	shaderList.push_back(*shaderVerde);

	// Shader azul [2]
	Shader* shaderAzul = new Shader();
	shaderAzul->CreateFromFiles("shaders/shader.vert", "shaders/azul.frag");
	shaderList.push_back(*shaderAzul);

	// Shader cafe [3]
	Shader* shaderCafe = new Shader();
	shaderCafe->CreateFromFiles("shaders/shader.vert", "shaders/cafe.frag");
	shaderList.push_back(*shaderCafe);

	// Shader verde (árbol) [4]
	Shader* shaderVerdeOscuro = new Shader();
	shaderVerdeOscuro->CreateFromFiles("shaders/shader.vert", "shaders/verdeOscuro.frag");
	shaderList.push_back(*shaderVerdeOscuro);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
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

		//--------------------------- TECHO ---------------------------
			shaderList[2].useShader();
			uniformModel = shaderList[2].getModelLocation();
			uniformProjection = shaderList[2].getProjectLocation();
			angulo += 0.01;
			//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.7f, -3.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[0]->RenderMesh();

		//---------------------- HOJA IZQUIERDA -----------------------------
			shaderList[4].useShader();
			uniformModel = shaderList[4].getModelLocation();
			uniformProjection = shaderList[4].getProjectLocation();
			angulo += 0.01;
			//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-0.7f, -0.3f, -3.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[1]->RenderMesh();

		//---------------------- HOJA DERECHA -----------------------------
			shaderList[4].useShader();
			uniformModel = shaderList[4].getModelLocation();
			uniformProjection = shaderList[4].getProjectLocation();
			angulo += 0.01;
			//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.7f, -0.3f, -3.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[2]->RenderMesh();

		//------------------------ VENTANA IZQUIERDA -------------------------
			shaderList[1].useShader();
			uniformModel = shaderList[1].getModelLocation();
			uniformProjection = shaderList[1].getProjectLocation();
			angulo += 0.01;
			//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-0.2f, 0.2f, -3.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[3]->RenderMesh();

		//------------------------ VENTANA DERECHA -------------------------
			shaderList[1].useShader();
			uniformModel = shaderList[1].getModelLocation();
			uniformProjection = shaderList[1].getProjectLocation();
			angulo += 0.01;
			//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.15f, 0.2f, -3.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[4]->RenderMesh();

		//--------------------------- PUERTA -------------------------------
			shaderList[1].useShader();
			uniformModel = shaderList[1].getModelLocation();
			uniformProjection = shaderList[1].getProjectLocation();
			angulo += 0.01;
			//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-0.03f, -0.5f, -3.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[5]->RenderMesh();

		//------------------------------------ CASA (BASE) --------------------------
			shaderList[0].useShader(); 
			uniformModel = shaderList[0].getModelLocation();
			uniformProjection = shaderList[0].getProjectLocation();
			angulo += 0.01;
			//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, -0.5f, -3.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[6]->RenderMesh();

		//--------------------------- TRONCO IZQUIERDO --------------------------------------------------
			shaderList[3].useShader();
			uniformModel = shaderList[3].getModelLocation();
			uniformProjection = shaderList[3].getProjectLocation();
			angulo += 0.01;
			//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-0.6f, -0.5f, -3.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[7]->RenderMesh();

		//------------------ TRONCO DERECHO ----------------------------------
			shaderList[3].useShader();
			uniformModel = shaderList[3].getModelLocation();
			uniformProjection = shaderList[3].getProjectLocation();
			angulo += 0.01;
			//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.8f, -0.5f, -3.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshList[8]->RenderMesh();

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