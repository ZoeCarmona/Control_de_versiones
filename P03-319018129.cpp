//práctica 3: Modelado Geométrico y Cámara Sintética.
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
//clases para dar orden y limpieza al còdigo
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
const float toRadians = 3.14159265f/180.0; //grados a radianes
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
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

void CrearPiramideTriangular()
{
	//-------------- BASE PIRAMIDE (NEGRA) ---------------------
	unsigned int indices_piramide_triangular[] = {
		0, 1, 2,
		1, 3, 2,
		3, 0, 2,
		1, 0, 3
	};

	float max = 2.0f;

	GLfloat vertices_piramide_triangular[] = {
		-0.65f * max, -0.55f * max, 0.0f * max,  // Vértice 0
		0.65f * max, -0.55f * max, 0.0f * max,   // Vértice 1
		0.0f * max, 0.7f * max, -0.36f * max,   // Vértice 2 Cima
		-0.1f * max, -0.57f * max, -1.1f * max   // Vértice 3 Base de atrás
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

	//-------------- PiIRAMIDES ---------------------
	unsigned int punta_indices_piramide_triangular[] = {
		0, 1, 2,
		1, 3, 2,
		3, 0, 2,
		1, 0, 3
	};

	GLfloat punta_vertices_piramide_triangular[] = {
		-0.7f, -0.7f, 0.0f,  // Vértice 0
		0.7f, -0.7f, 0.0f,   // Vértice 1
		0.0f, 0.7f, -0.3f,   // Vértice 2 Cima
		0.0f, -0.65f, -1.0f   // Vértice 3 Base de atrás
	};

	Mesh* punta = new Mesh();
	punta->CreateMesh(punta_vertices_piramide_triangular, punta_indices_piramide_triangular, 12, 12);
	meshList.push_back(punta);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
    mainWindow = Window(800, 600);
    mainWindow.Initialise();

    CrearPiramideTriangular();
    CreateShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformColor = 0;
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        deltaTime += (now - lastTime) / limitFPS;
        lastTime = now;
        glfwPollEvents();
        camera.keyControl(mainWindow.getsKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Renderizar la base (pirámide negra)
        shaderList[0].useShader();
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectLocation();
        uniformView = shaderList[0].getViewLocation();
        uniformColor = shaderList[0].getColorLocation();

        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); // Color negro
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        meshList[0]->RenderMesh(); // Renderizar la pirámide negra

    //--------------- COLOR ROJO ------------------------------------------
        // Renderizar la punta 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.0f, 0.9f, -4.42f)); 
            model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));    
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(1.0f, 0.0f, 0.0f); 
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh(); 

        // Renderizar segundo piso (1er triangulo de izquierda a derecha)
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-0.37f, 0.10f, -4.2f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));      
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(1.0f, 0.0f, 0.0f); 
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh(); 

          // Renderizar segundo piso (enmedio rotado)
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.0f, 0.18f, -4.22f)); 
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(1.0f, 0.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          // Renderizar segundo piso (3er triangulo de izquierda a derecha)
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.37f, 0.10f, -4.2f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(1.0f, 0.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          // Renderizar tercer piso (1er triangulo de izquierda a derecha)
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-0.78f, -0.68f, -4.01f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(1.0f, 0.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          //Renderizar el tercer piso (1er rotado)
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.39f, -0.6f, -4.04f));
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(1.0f, 0.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

           // Renderizar tercer piso (3er triangulo de izquierda a derecha)
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.0f, -0.68f, -4.01f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(1.0f, 0.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          //Renderizar el tercer piso (2do rotado)
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-0.39f, -0.6f, -4.04f));
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(1.0f, 0.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          // Renderizar tercer piso (5to triangulo de izquierda a derecha)
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.78f, -0.68f, -4.01f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(1.0f, 0.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

    //----------------------- COLOR AZUL -----------------------------------
    
    // Renderizar la punta 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.032f, 0.9f, -4.88f));
            model = glm::rotate(model, glm::radians(125.0f), glm::vec3(0.0f, 0.7f, 0.0f));
            model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(0.0f, 0.0f, 1.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          // Renderizar 2da fila (primero) 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-0.07f, 0.1f, -5.34f));
            model = glm::rotate(model, glm::radians(125.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(0.5f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(0.0f, 0.0f, 1.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          // Renderizar 2da fila (rotado) 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-0.3f, 0.1f, -5.05f)); //-0.3 x
            
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //Rotando
            model = glm::rotate(model, glm::radians(59.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(42.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(-21.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(0.0f, 0.0f, 1.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          // Renderizar 2da fila (segundo) 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-0.37f, 0.1f, -4.67f));
            model = glm::rotate(model, glm::radians(125.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(0.5f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(0.0f, 0.0f, 1.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          // Renderizar 3ra fila (primero) 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-0.151f, -0.68f, -5.81f));
            model = glm::rotate(model, glm::radians(125.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(0.5f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(0.0f, 0.0f, 1.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          // Renderizar 3ra fila (primero rotado) 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-0.3f, -0.68f, -5.56f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //Rotando
            model = glm::rotate(model, glm::radians(58.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(42.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(-21.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(0.0f, 0.0f, 1.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();


          // Renderizar 3ra fila (enmedio) 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-0.46f, -0.68f, -5.15f));
            model = glm::rotate(model, glm::radians(125.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(0.5f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(0.0f, 0.0f, 1.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          // Renderizar 3ra fila (segundo rotado) 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-0.7f, -0.7f, -4.8f)); //0.5 x
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //Rotando
            model = glm::rotate(model, glm::radians(59.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(42.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(-21.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(0.0f, 0.0f, 1.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();


          // Renderizar 3ra fila (tercero) 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-0.77f, -0.70f, -4.45f));
            model = glm::rotate(model, glm::radians(125.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(4.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(0.0f, 0.0f, 1.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();
   
    //---------------- COLOR VERDE ------------------------------------------------
          // Renderizar la punta 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.2f, 0.9f, -4.85f));
            model = glm::rotate(model, glm::radians(125.0f), glm::vec3(0.0f, 0.7f, 0.0f));
            //model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(0.0f, 1.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          // Renderizar la 2a fila (primera) 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.45f, 0.12f, -4.42f));
            //model = glm::rotate(model, glm::radians(125.0f), glm::vec3(0.0f, 0.7f, 0.0f));
            //model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(0.0f, 1.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          // Renderizar la 2a fila (segunda) 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.03f, 0.15f, -5.09f));
            //model = glm::rotate(model, glm::radians(125.0f), glm::vec3(0.0f, 0.7f, 0.0f));
            //model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(0.0f, 1.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          // Renderizar la 3a fila (tercera) 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.07f, -0.69f, -5.77f));
            model = glm::rotate(model, glm::radians(125.0f), glm::vec3(0.0f, 0.7f, 0.0f));
            //model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(0.0f, 1.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          // Renderizar la 3a fila (segunda) 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.4f, -0.66f, -4.87f));
            //model = glm::rotate(model, glm::radians(125.0f), glm::vec3(0.0f, 0.7f, 0.0f));
            //model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(0.0f, 1.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          // Renderizar la 3a fila (primera) 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.8f, -0.66f, -4.22f));
            //model = glm::rotate(model, glm::radians(125.0f), glm::vec3(0.0f, 0.7f, 0.0f));
            //model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(0.0f, 1.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          //------Rotados
          // Primero debajo
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.8f, -0.6f, -4.83f));
            model = glm::rotate(model, glm::radians(125.0f), glm::vec3(0.0f, 0.7f, 0.0f));
            model = glm::rotate(model, glm::radians(-25.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //Inclinando
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //Rotado
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(0.0f, 1.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

            // Segundo debajo
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.3f, -0.6f, -5.45f));
            model = glm::rotate(model, glm::radians(125.0f), glm::vec3(0.0f, 0.7f, 0.0f));
            model = glm::rotate(model, glm::radians(-25.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //Inclinando
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //Rotado
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(0.0f, 1.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

            // segunda fila
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.3f, 0.19f, -5.0f));
            model = glm::rotate(model, glm::radians(125.0f), glm::vec3(0.0f, 0.7f, 0.0f));
            model = glm::rotate(model, glm::radians(-27.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //Inclinando
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //Rotado
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(0.0f, 1.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

        //-----------------COLOR AMARILLO-----------------------
          // Renderizar esquina de abajo 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.7f, -0.8f, -4.12f));
            model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(1.0f, 1.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          // Renderizar 2da esquina de abajo 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-0.85f, -0.8f, -4.12f));
            model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(1.0f, 1.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();
          
            // Renderizar enmedio  de abajo 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-0.097f, -0.8f, -4.12f));
            model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(1.0f, 1.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

         // Renderizar punta abajo
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-0.14f, -0.8f, -5.42f));
            model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(1.0f, 1.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          // Renderizar enmedio  de abajo 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.32f, -0.8f, -4.75f)); 
            model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(1.0f, 1.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          // Renderizar 2da fila primero
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-0.49f, -0.8f, -4.75f));
            model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(1.0f, 1.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

        //Primer figura rotada
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-0.49f, -0.85f, -4.63f));
            //model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            //model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(1.0f, 1.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

         //2da figura rotada abajo (1)
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(-0.13f, -0.85f, -5.32f)); //sube z y baja x (0.092)
            //model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            //model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(1.0f, 1.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

          //2da fila figura rotada abajo 
            shaderList[0].useShader();
            uniformModel = shaderList[0].getModelLocation();
            uniformProjection = shaderList[0].getProjectLocation();
            uniformView = shaderList[0].getViewLocation();
            uniformColor = shaderList[0].getColorLocation();

            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(0.26f, -0.85f, -4.61f)); //baja z(arriba) y baja x (izquierda)
            //model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            //model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
            color = glm::vec3(1.0f, 1.0f, 0.0f);
            glUniform3fv(uniformColor, 1, glm::value_ptr(color));
            meshList[1]->RenderMesh();

        glUseProgram(0);
        mainWindow.swapBuffers();
    }
    return 0;
}



	
		