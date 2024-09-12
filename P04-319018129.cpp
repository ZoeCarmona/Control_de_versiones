/*Pr�ctica 4: Modelado Jer�rquico.
Se implementa el uso de matrices adicionales para almacenar informaci�n de transformaciones geom�tricas que se quiere
heredar entre diversas instancias para que est�n unidas
Teclas de la F a la K para rotaciones de articulaciones
*/
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
#include <gtc\random.hpp>
//clases para dar orden y limpieza al c�digo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z
using std::vector;
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

void CrearCubo()
{
	unsigned int cubo_indices[] = {
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

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pir�mide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro y cono con arreglos din�micos vector creados en el Semestre 2023 - 1 : por S�nchez P�rez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los v�rtices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el c�rculo
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los v�rtices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//funci�n para crear un cono
void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	//ciclo for para crear los v�rtices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res + 2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//funci�n para crear pir�mide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resoluci�n (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//�ndice 0 en MeshList
	CrearPiramideTriangular();//�ndice 1 en MeshList
	CrearCilindro(20, 1.0f);//�ndice 2 en MeshList  AGREGANDO SEGMENTOS (10 se ve bien)
	CrearCono(25, 2.0f);//�ndice 3 en MeshList
	CrearPiramideCuadrangular();//�ndice 4 en MeshList
	CreateShaders();



	/*C�mara se usa el comando: glm::lookAt(vector de posici�n, vector de orientaci�n, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posici�n,
	glm::vec3 vector up,
	GlFloat yaw rotaci�n para girar hacia la derecha e izquierda
	GlFloat pitch rotaci�n para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posici�n inicial est� en 0,0,1 y ve hacia 0,0,-1.
	*/
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);

	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarqu�a
	glm::mat4 modelOriginal = model; //Inicializando matriz auxiliar para brazo

	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//C�mara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		//------------------------ PRIMER EJERCICIO -----------------------------------------------------------
		/*
				// Creando el brazo de una gr�a
				//articulacion1 hasta articulaci�n5 s�lo son puntos de rotaci�n o articulaci�n, en este caso no dibujaremos esferas que los representen

				//para reiniciar la matriz de modelo con valor de la matriz identidad
				model = glm::mat4(1.0);
				//AQU� SE DIBUJA LA CABINA, LA BASE, LAS 4 LLANTAS
				//----------------- cabina ----------------------------------
				modelaux = model;
				model = glm::translate(model, glm::vec3(0.0f, 6.0f, -2.0f));
				model = glm::scale(model, glm::vec3(4.0f, 3.0f, 3.0f));
				model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				color = glm::vec3(0.5f, 0.5f, 0.5f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				meshList[0]->RenderMesh(); // Renderizar el cubo (cabina)

				//para descartar la escala que no quiero heredar se carga la informaci�n de la matrix auxiliar
				model = modelaux;

				//-----------Base para llantas
				modelaux = model;
				model = glm::translate(model, glm::vec3(0.0f, 4.3f, -2.0f));
				model = glm::scale(model, glm::vec3(5.0f, 1.5f, 3.5f));
				color = glm::vec3(0.2f, 0.2f, 0.2f); // gris m�s oscuro
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[4]->RenderMesh();

				// Restaurar el estado original de la cabina
				model = modelaux;

				//-------------------LLANTAS
				// Guardar el estado actual de la cabina
				modelaux = model;
				//-------articulacion primera llanta (izquierda enfrente)
				model = glm::translate(model, glm::vec3(-2.5f, 3.5f, 0.0f));
				//TECLA Y
				model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
				//dibujar una peque�a esfera
				//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				//sp.render();
				//----------------- Dibujar la llanta izquierda -------------------
				// Trasladamos la matriz un poco hacia abajo para colocar la llanta
				model = glm::translate(model, glm::vec3(0.0f, 0.02f, 0.0f)); 
				model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.5f)); // 1.5f, 1.5f, 1.0f
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				color = glm::vec3(0.1f, 0.1f, 0.1f); // gris oscuro 
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[2]->RenderMeshGeometry(); 

				// Restaurar el estado original de la cabina
				model = modelaux;

				// Guardar el estado actual de la cabina
				modelaux = model;
				//-------articulacion segunda llanta (derecha enfrente)
				model = glm::translate(model, glm::vec3(-2.5f, 3.5f, -4.0f));
				//Tecla U
				model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
				//dibujar una peque�a esfera
				
				//----------------- Dibujar la llanta derecha -------------------
				// Trasladamos la matriz un poco hacia abajo para colocar la llanta
				model = glm::translate(model, glm::vec3(0.0f, 0.02f, 0.0f));
				model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.5f)); // 1.5f, 1.5f, 1.0f
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				color = glm::vec3(0.1f, 0.1f, 0.1f); // gris oscuro 
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[2]->RenderMeshGeometry();

				// Restaurar el estado original de la cabina
				model = modelaux;

				// Guardar el estado actual de la cabina
				modelaux = model;
				//-------articulacion tercera llanta (izquierda atr�s)
				model = glm::translate(model, glm::vec3(2.5f, 3.5f, 0.0f));
				//TECLA Y
				model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f));
				//dibujar una peque�a esfera
				//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				//sp.render();
				//----------------- Dibujar la llanta izquierda -------------------
				// Trasladamos la matriz un poco hacia abajo para colocar la llanta
				model = glm::translate(model, glm::vec3(0.0f, 0.02f, 0.0f));
				model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.5f)); // 1.5f, 1.5f, 1.0f
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				color = glm::vec3(0.1f, 0.1f, 0.1f); // gris oscuro 
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[2]->RenderMeshGeometry();

				// Restaurar el estado original de la cabina
				model = modelaux;

				// Guardar el estado actual de la cabina
				modelaux = model;
				//-------articulacion cuarta llanta (derecha atr�s)
				model = glm::translate(model, glm::vec3(2.5f, 3.5f, -4.0f));
				//TECLA Y
				model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 0.0f, 1.0f));
				//dibujar una peque�a esfera
				//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				//sp.render();
				//----------------- Dibujar la llanta derecha -------------------
				// Trasladamos la matriz un poco hacia abajo para colocar la llanta
				model = glm::translate(model, glm::vec3(0.0f, 0.02f, 0.0f));
				model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.5f)); // 1.5f, 1.5f, 1.0f
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				color = glm::vec3(0.1f, 0.1f, 0.1f); // gris oscuro 
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[2]->RenderMeshGeometry();

				// Restaurar el estado original de la cabina
				model = modelaux;

				// SE EMPIEZA EL DIBUJO DEL BRAZO
				//------------------------articulaci�n 1-------------------------
				model = glm::translate(model, glm::vec3(0.0f, 6.0f, -4.0f));
				//rotaci�n alrededor de la articulaci�n que une con la cabina
				model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
				modelaux = model;

				//----------------------------------primer brazo que conecta con la cabina-----------------
				// //Traslaci�n inicial para posicionar en -Z a los objetos
				//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
				//otras transformaciones para el objeto
				model = glm::translate(model, glm::vec3(-1.0f, 2.0f, 0.0f)); //Punto central del brazo
				model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));

				modelaux = model; //La matriz auxiliar guarda todas las transformaciones anteriores
				model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f)); //Se estableci� 5 
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				//la l�nea de proyecci�n solo se manda una vez a menos que en tiempo de ejecuci�n
				//se programe cambio entre proyecci�n ortogonal y perspectiva
				glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
				color = glm::vec3(1.0f, 0.85f, 0.0f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
				meshList[0]->RenderMesh(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular
				//meshList[2]->RenderMeshGeometry(); //dibuja las figuras geom�tricas cilindro y cono

				//para descartar la escala que no quiero heredar se carga la informaci�n de la matrix auxiliar
				model = modelaux;

				//-------------------------articulaci�n 2----------------
				model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));//Poniendo artoculaci�n en la punta del brazo
				model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));//Indicamos que nuestro brazo se mueva en z
				modelaux = model;
				//dibujar una peque�a esfera
				model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				sp.render();

				//-----------------------------segundo brazo-------------------------------------
				model = modelaux; //para descartar la escala que no quiero heredar se carga la informaci�n de la matriz auxiliar
				model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f)); //y negativa debido a que heredamos traslacion
				modelaux = model;
				model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				color = glm::vec3(1.0f, 1.0f, 0.0f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
				meshList[0]->RenderMesh(); //dibuja cubo y pir�mide triangular

				model = modelaux;

				//-------------articulaci�n 3 extremo derecho del segundo brazo----------------------
				model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
				model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
				modelaux = model;
				//dibujar una peque�a esfera
				model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				sp.render();

				//-----------------------------tercer brazo-------------------------------------
				model = modelaux; //para descartar la escala que no quiero heredar se carga la informaci�n de la matriz auxiliar
				model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f)); //y negativa debido a que heredamos traslacion
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				modelaux = model;
				model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				color = glm::vec3(1.0f, 0.85f, 0.0f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
				meshList[0]->RenderMesh(); //dibuja cubo y pir�mide triangular

				//para descartar la escala que no quiero heredar se carga la informaci�n de la matrix auxiliar
				model = modelaux;

				//-------------articulaci�n 4 ----------------------
				model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
				model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(1.0f, 0.0f, 0.0f));
				modelaux = model;
				//dibujar una peque�a esfera
				model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				sp.render();

				//-----------------------------canasta-------------------------------------
				model = modelaux; //para descartar la escala que no quiero heredar se carga la informaci�n de la matriz auxiliar
				model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f)); //y negativa debido a que heredamos traslacion
				//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				modelaux = model;
				model = glm::scale(model, glm::vec3(2.5f, 0.5f, 2.5f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				color = glm::vec3(1.0f, 1.0f, 0.0f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
				meshList[0]->RenderMesh(); //dibuja cubo y pir�mide triangular

				//------segunda pared
				model = modelaux; //para descartar la escala que no quiero heredar se carga la informaci�n de la matriz auxiliar
				model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f)); //y negativa debido a que heredamos 
				modelaux = model;
				model = glm::scale(model, glm::vec3(2.5f, 0.5f, 2.5f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				color = glm::vec3(1.0f, 1.0f, 0.0f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
				meshList[0]->RenderMesh(); //dibuja cubo y pir�mide triangulartraslacion

				//----------------------tercera pared
				model = modelaux; //para descartar la escala que no quiero heredar se carga la informaci�n de la matriz auxiliar
				model = glm::translate(model, glm::vec3(0.0f, 1.5f, 1.0f)); //y negativa debido a que heredamos 
				modelaux = model;
				model = glm::scale(model, glm::vec3(2.5f, 2.5f, 0.5f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				color = glm::vec3(1.0f, 1.0f, 0.0f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
				meshList[0]->RenderMesh(); //dibuja cubo y pir�mide triangulartraslacion

				//----------------------cuarta pared
				model = modelaux; //para descartar la escala que no quiero heredar se carga la informaci�n de la matriz auxiliar
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f)); //y negativa debido a que heredamos 
				modelaux = model;
				model = glm::scale(model, glm::vec3(2.5f, 2.5f, 0.5f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				color = glm::vec3(1.0f, 1.0f, 0.0f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
				meshList[0]->RenderMesh(); //dibuja cubo y pir�mide triangulartraslacion

				//----------------------base
				model = modelaux; //para descartar la escala que no quiero heredar se carga la informaci�n de la matriz auxiliar
				model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f)); //y negativa debido a que heredamos 
				modelaux = model;
				model = glm::scale(model, glm::vec3(0.5f, 2.5f, 2.5f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				color = glm::vec3(0.8f, 0.7f, 0.0f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
				meshList[0]->RenderMesh(); //dibuja cubo y pir�mide triangulartraslacion
		*/
		//------------------------------------ SEGUNDO EJERCICIO -------------------------------------------
				model = glm::mat4(1.0);
		//----------- Cuerpo 
				model = glm::translate(model, glm::vec3(0.0f, 6.0f, -4.0f));
				modelaux = model;
				model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f)); //Se estableci� 5 
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
				color = glm::vec3(1.0f, 0.47f, 0.13f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color)); 
				//meshList[0]->RenderMesh(); //dibuja cubo, pir�mide triangular, pir�mide base cuadrangular
				meshList[2]->RenderMeshGeometry(); //dibuja las figuras geom�tricas cilindro y cono
				model = modelaux;
		//------- Collar 
				model = glm::translate(model, glm::vec3(0.0f, 1.05f, 0.0f));
				modelaux = model;
				model = glm::scale(model, glm::vec3(1.1f, 0.1f, 1.1f)); 
				color = glm::vec3(1.0f, 0.0f, 0.0f);  
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				//meshList[0]->RenderMesh();
				meshList[2]->RenderMeshGeometry();
				model = modelaux;

		//------ Cascabel
				model = glm::translate(model, glm::vec3(0.0f, -0.27f, 1.2f));
				modelaux = model;
				model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
				color = glm::vec3(1.0f, 0.84f, 0.0f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				sp.render();
				model = modelaux;
		//-------- Cola
				//-------Articulacion 1
				model = glm::translate(model, glm::vec3(0.0f, -1.2f, -2.3f));
				//TECLA F
				model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
				//dibujar una peque�a esfera
				//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
				//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				//sp.render();
				model = glm::translate(model, glm::vec3(0.0f, 1.0f, -0.4f));
				model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.5f, 3.0f, 0.5f)); 
				color = glm::vec3(1.0f, 0.47f, 0.13f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[0]->RenderMesh();
				model = modelaux;
		//--------- Cabeza
				model = glm::translate(model, glm::vec3(0.0f, 1.17f, -1.2f));
				modelaux = model;
				model = glm::scale(model, glm::vec3(2.2f, 1.7f, 2.2f));
				color = glm::vec3(1.0f, 0.47f, 0.13f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[0]->RenderMesh();
				modelaux = model;
		//------Ojo izquierdo
				model = glm::translate(model, glm::vec3(-0.2f, 0.2f, 0.45f));
				modelaux = model;
				model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.2f));
				modelaux = model;
				color = glm::vec3(0.0f, 0.0f, 0.0f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[0]->RenderMesh();
				modelaux = model;
		//------Ojo derecho
				model = glm::translate(model, glm::vec3(4.3f, 0.04f, 0.05f));
				modelaux = model;
				//model = glm::scale(model, glm::vec3(0.5f, 0.6f, 0.6f));
				color = glm::vec3(0.0f, 0.0f, 0.0f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[0]->RenderMesh();
				model = modelaux;
		//------Nariz
				model = glm::translate(model, glm::vec3(-2.3f, -1.27f, 0.3f));
				modelaux = model;
				model = glm::scale(model, glm::vec3(1.0f, 0.5f, 0.5f));
				color = glm::vec3(1.0f, 0.75f, 0.8f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				sp.render();
				model = modelaux;
		//------Boca
				model = glm::translate(model, glm::vec3(0.0f, -0.8f, -0.2f));
				modelaux = model;
				model = glm::scale(model, glm::vec3(0.2f, 0.6f, 0.6f));
				color = glm::vec3(0.0f, 0.0f, 0.0f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[0]->RenderMesh();
				model = modelaux;

				model = glm::translate(model, glm::vec3(0.0f, -0.25f, 0.0f));
				modelaux = model;
				model = glm::scale(model, glm::vec3(1.5f, 0.1f, 0.6f));
				color = glm::vec3(0.0f, 0.0f, 0.0f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[0]->RenderMesh();
				model = modelaux;
		//-----Oreja izquierda
				model = glm::translate(model, glm::vec3(-5.1f, 3.8f, -2.5f));
				modelaux = model;
				model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::scale(model, glm::vec3(0.9f, 3.8f, 0.5f));
				color = glm::vec3(1.0f, 0.8f, 0.6f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				//meshList[0]->RenderMesh();
				meshList[3]->RenderMeshGeometry();
				model = modelaux;
		//------Oreja derecha
				model = glm::translate(model, glm::vec3(10.3f, 0.1f, 0.1f));
				modelaux = model;
				model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::scale(model, glm::vec3(0.9f, 3.8f, 0.5f));
				color = glm::vec3(1.0f, 0.8f, 0.6f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				//meshList[0]->RenderMesh();
				meshList[3]->RenderMeshGeometry();
				model = modelaux;
				
		//-------Brazo Izquierdo
				//------- Articulacion 2 (Brazo izquierdo, parte pegada al cuerpo)
				model = glm::translate(model, glm::vec3(-10.0f, -6.7f, 0.0f));
				// TECLA H
				model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
				modelaux = model;
				// Dibujar parte del brazo izquierdo pegada al cuerpo
				model = glm::translate(model, glm::vec3(-0.3f, 0.3f, 1.2f));
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(2.0f, 3.0f, 1.0f));
				color = glm::vec3(1.0f, 0.6f, 0.3f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[0]->RenderMesh();
				model = modelaux;
				//------- Articulacion 3 (Brazo izquierdo, cubo peque�o)
				model = glm::translate(model, glm::vec3(-0.4f, 0.3f, 2.0f));
				// TECLA J
				model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
				modelaux = model;
				// Dibujar el cubo peque�o del brazo izquierdo
				model = glm::translate(model, glm::vec3(0.0f, 0.35f, 1.1f));
				model = glm::scale(model, glm::vec3(2.0f, 1.8f, 0.8f));
				color = glm::vec3(1.0f, 0.8f, 0.6f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[0]->RenderMesh();
				model = modelaux;
				model = modelOriginal;
		//-------BRAZO DERECHO
				//------- Articulacion 4 (Brazo derecho, parte pegada al cuerpo)
				model = glm::translate(model, glm::vec3(0.8f, 6.70f, -3.9f));
				// TECLA J
				model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(1.0f, 0.0f, 0.0f));
				modelaux = model;
				// Dibujar parte del brazo derecho pegada al cuerpo
				model = glm::translate(model, glm::vec3(0.4f, 0.0f, 0.5f)); // Sim�trico al brazo izquierdo
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.47f, 1.1f, 0.33f));
				color = glm::vec3(1.0f, 0.6f, 0.3f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[0]->RenderMesh();
				model = modelaux;
				//------- Articulacion 5 (Brazo derecho, cubo peque�o)
				model = glm::translate(model, glm::vec3(0.4f, -0.05f, 0.1f));
				// TECLA K
				model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
				modelaux = model;
				// Dibujar el cubo peque�o del brazo derecho
				model = glm::translate(model, glm::vec3(0.0f, 0.15f, 1.1f));
				model = glm::scale(model, glm::vec3(0.54f, 0.65f, 0.3f));
				color = glm::vec3(1.0f, 0.8f, 0.6f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[0]->RenderMesh();
		//-----Pie Izquierdo
				model = modelOriginal;  
				//------- Articulacion 6 (Pie izquierdo, parte pegada al cuerpo)
				model = glm::translate(model, glm::vec3(-0.8f, 4.44f, -4.3f));
				// TECLA Y
				model = glm::rotate(model, glm::radians(-mainWindow.getarticulacion6()), glm::vec3(0.0f, 1.0f, 0.0f));
				modelaux = model;
				// Dibujar parte del pie izquierdo pegada al cuerpo
				model = glm::translate(model, glm::vec3(0.1f, 0.3f, 0.7f));
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.5f, 1.3f, 0.5f));
				color = glm::vec3(1.0f, 0.6f, 0.3f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[0]->RenderMesh();
				model = modelaux;

				//------- Articulacion 7 (Pie izquierdo, cubo peque�o)
				model = glm::translate(model, glm::vec3(0.12f, 0.1f, 1.5f));
				// TECLA U
				model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(1.0f, 0.0f, 0.0f));
				modelaux = model;
				// Dibujar el cubo peque�o del pie izquierdo
				model = glm::translate(model, glm::vec3(0.0f, 0.2f, 0.0f));
				model = glm::scale(model, glm::vec3(0.5f, 0.6f, 0.3f));
				color = glm::vec3(1.0f, 0.8f, 0.6f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[0]->RenderMesh();
				model = modelOriginal;  


		//-----Pie Derecho
				model = modelOriginal;
				//------- Articulacion 8 (Pie izquierdo, parte pegada al cuerpo)
				model = glm::translate(model, glm::vec3(0.6f, 4.44f, -4.3f));
				// TECLA Y
				model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 1.0f, 0.0f));
				modelaux = model;
				// Dibujar parte del pie izquierdo pegada al cuerpo
				model = glm::translate(model, glm::vec3(0.1f, 0.3f, 0.7f));
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.5f, 1.3f, 0.5f));
				color = glm::vec3(1.0f, 0.6f, 0.3f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[0]->RenderMesh();
				model = modelaux;

				//------- Articulacion 9 (Pie izquierdo, cubo peque�o)
				model = glm::translate(model, glm::vec3(0.12f, 0.1f, 1.5f));
				// TECLA U
				model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(1.0f, 0.0f, 0.0f));
				modelaux = model;
				// Dibujar el cubo peque�o del pie izquierdo
				model = glm::translate(model, glm::vec3(0.0f, 0.2f, 0.0f));
				model = glm::scale(model, glm::vec3(0.5f, 0.6f, 0.3f));
				color = glm::vec3(1.0f, 0.8f, 0.6f);
				glUniform3fv(uniformColor, 1, glm::value_ptr(color));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshList[0]->RenderMesh();
				model = modelOriginal;


		


		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}