

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"
#include "esfera.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion1();
void animacion2();
void animacion3();
void animacion4();
void cuadross();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;
float cuadros[3];

//Objeto Esfera
Esfera bola(1.0f);

// Camera
Camera  camera(glm::vec3(0.0f, 15.0f, -15.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rot = 0.0f;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 PosIni(0.0f, 0.0f, -45.0f);

bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(6.0f, 6.4f, 3.0f),
	glm::vec3(-6.0f, 6.0f, 3.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(4.0f,  2.0f, -12.0f),
};

glm::vec3 LightP1;
float puertaRot = 0.0;
float puertaBRot = 0.0;
//Animación del coche
float movKitX = 0.0;
float movKitZ = 0.0;
float rotKit = 0.0;

bool mov_puerta = false;
bool cerrar_puerta = false;
bool mov_puertaB = false;
bool cerrar_puertaB = false;

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	cuadros[0] = 8.0f;
	cuadros[1] = 3.0f;
	cuadros[2] = -2.0f;
	cuadros[3] = -7.0f;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Taberna de Moe", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Model Rockola((char*)"Models/models/20344_Jukebox_bubbler_style_V2 Textured.obj");
	Model Barra((char*)"Models/models/BarStand.obj");
	Model Vodka((char*)"Models/models/LithuanianVodka.obj");
	Model botella1((char*)"Models/models/14043_16_oz._Beer_Bottle_v1_l3.obj");
	
	// Build and compile our shader program


	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};

	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  0.0f)
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// Load textures
	GLuint texture1, texture2, texture3, texture4, texture5, texture6, 
	texture7, texture8, texture9, texture10, texture11, texture12, texture13, 
	texture14, texture15, texture16, texture17, texture18, texture19,
	texture20, texture21, texture22;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);
	glGenTextures(1, &texture3);
	glGenTextures(1, &texture4);
	glGenTextures(1, &texture5);
	glGenTextures(1, &texture6);
	glGenTextures(1, &texture7);
	glGenTextures(1, &texture8);
	glGenTextures(1, &texture9);
	glGenTextures(1, &texture10);
	glGenTextures(1, &texture11);
	glGenTextures(1, &texture12);
	glGenTextures(1, &texture13);
	glGenTextures(1, &texture14);
	glGenTextures(1, &texture15);
	glGenTextures(1, &texture16);
	glGenTextures(1, &texture17);
	glGenTextures(1, &texture18);
	glGenTextures(1, &texture19);
	glGenTextures(1, &texture20);
	glGenTextures(1, &texture21);
	glGenTextures(1, &texture22);

	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *image;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Diffuse map TEXTURA 1
	image = stbi_load("images/asfalto.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 1" << std::endl;
	}
	stbi_image_free(image);

	// Specular map TEXTURA 2
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);


	image = stbi_load("images/ladrillos.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 2" << std::endl;
	}
	stbi_image_free(image);

	//TEXTURA 3
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/piso_interior.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 3" << std::endl;
	}
	stbi_image_free(image);

	//TEXTURA 4
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	image = stbi_load("images/puerta.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 4" << std::endl;
	}
	stbi_image_free(image);

	//TEXTURA 5
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	image = stbi_load("images/ventana.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 5" << std::endl;
	}
	stbi_image_free(image);

	//TEXTURA 6
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	image = stbi_load("images/madera_1.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 6" << std::endl;
	}
	stbi_image_free(image);

	//TEXTURA 7
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	image = stbi_load("images/letrero.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 7" << std::endl;
	}
	stbi_image_free(image);

	//TEXTURA 9
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	image = stbi_load("images/fondo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture9);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 9" << std::endl;
	}
	stbi_image_free(image);

	//TEXTURA 10
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	image = stbi_load("images/verde.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture10);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 10" << std::endl;
	}
	stbi_image_free(image);


	//TEXTURA 11
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	image = stbi_load("images/café.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture11);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 11" << std::endl;
	}
	stbi_image_free(image);

	//TEXTURA 12
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	image = stbi_load("images/bbb.png", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture12);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 12" << std::endl;
	}
	stbi_image_free(image);

	//TEXTURA 13
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	image = stbi_load("images/madera.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture13);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 13" << std::endl;
	}
	stbi_image_free(image);

	//TEXTURA 14
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	image = stbi_load("images/cuadro1.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture14);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 14" << std::endl;
	}
	stbi_image_free(image);

	//TEXTURA 15
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	image = stbi_load("images/cuadro2.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture15);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 15" << std::endl;
	}
	stbi_image_free(image);

	//TEXTURA 16
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	image = stbi_load("images/cuadro3.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture16);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 16" << std::endl;
	}
	stbi_image_free(image);

	//TEXTURA 17
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	image = stbi_load("images/cuadro4.png", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture17);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 17" << std::endl;
	}
	stbi_image_free(image);

	//TEXTURA 18
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	image = stbi_load("images/borde.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture18);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 18" << std::endl;
	}
	stbi_image_free(image);

	//TEXTURA 19
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	image = stbi_load("images/madera2.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture19);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 19" << std::endl;
	}
	stbi_image_free(image);

	//TEXTURA 20
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	image = stbi_load("images/metal.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture20);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 20" << std::endl;
	}
	stbi_image_free(image);

	//TEXTURA 22
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	image = stbi_load("images/guinda.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture22);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 21" << std::endl;
	}
	stbi_image_free(image);

	//TEXTURA 21
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	image = stbi_load("images/espejo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture21);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 21" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.125f, 0.125f, 0.325f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Load Model

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 0.0f);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), LightP1.x, LightP1.y, LightP1.z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), LightP1.x, LightP1.y, LightP1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 1.0f);
		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glBindVertexArray(VAO);
		glm::mat4 model(1);
		glm::mat4 modelTemp = glm::mat4(1.0f);
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(40.0f, 0.2f, 40.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//COMIENZA TABERNA DE MOE

		//Piso
		glBindTexture(GL_TEXTURE_2D, texture3);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.01f, 0.0f));
		model = glm::scale(model, glm::vec3(22.0f, 0.2f, 16.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Muro trasero
		glBindTexture(GL_TEXTURE_2D, texture2);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, 8.0f));
		model = glm::scale(model, glm::vec3(22.0f, 8.0f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Muro trasero interior
		glBindTexture(GL_TEXTURE_2D, texture10);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, 7.9f));
		model = glm::scale(model, glm::vec3(22.0f, 8.0f, 0.05));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Borde Muro trasero
		glBindTexture(GL_TEXTURE_2D, texture18);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 7.5f, 7.8f));
		model = glm::scale(model, glm::vec3(21.4f, 0.235, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Borde Muro delantero
		glBindTexture(GL_TEXTURE_2D, texture18);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 7.5f, -7.8f));
		model = glm::scale(model, glm::vec3(21.4f, 0.235, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuadro 1
		glBindTexture(GL_TEXTURE_2D, texture14);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(cuadros[0], 5.0f, 7.8f));
		model = glm::rotate(model, 3.15f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 2.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuadro 2
		glBindTexture(GL_TEXTURE_2D, texture15);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(cuadros[1], 5.0f, 7.8f));
		model = glm::rotate(model, 3.15f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 2.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuadro 3
		glBindTexture(GL_TEXTURE_2D, texture16);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(cuadros[2], 5.0f, 7.8f));
		model = glm::rotate(model, 3.15f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 2.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cuadro 4
		glBindTexture(GL_TEXTURE_2D, texture17);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(cuadros[3], 5.0f, 7.8f));
		model = glm::rotate(model, 3.15f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 2.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Muro izquierdo
		glBindTexture(GL_TEXTURE_2D, texture2);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 4.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 8.0f, 16.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Borde Muro izquierdo
		glBindTexture(GL_TEXTURE_2D, texture18);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(10.8f, 7.5f, 0.0f));
		model = glm::rotate(model, 1.57f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.7f, 0.235, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Ventana Lateral 1
		glBindTexture(GL_TEXTURE_2D, texture5);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 4.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 2.5f, 3.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera1 VL1
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 4.0f, 1.75f));
		model = glm::scale(model, glm::vec3(0.45f, 2.5f, 0.30f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera2 VL1
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 4.0f, -1.75f));
		model = glm::scale(model, glm::vec3(0.45f, 2.5f, 0.30f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera3 VL1
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 5.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.30f, 3.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera4 VL1
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 2.6f, 0.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.30f, 3.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Ventana Lateral 2
		glBindTexture(GL_TEXTURE_2D, texture5);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 4.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.3f, 2.5f, 3.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera1 VL2
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 4.0f, -3.25f));
		model = glm::scale(model, glm::vec3(0.45f, 2.5f, 0.30f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera2 VL2
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 4.0f, -6.75f));
		model = glm::scale(model, glm::vec3(0.45f, 2.5f, 0.30f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera3 VL2
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 5.4f, -5.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.30f, 3.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera4 VL2
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 2.6f, -5.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.30f, 3.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Ventana Lateral 3
		glBindTexture(GL_TEXTURE_2D, texture5);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 4.0f, 5.0f));
		model = glm::scale(model, glm::vec3(0.3f, 2.5f, 3.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera1 VL3
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 4.0f, 6.75f));
		model = glm::scale(model, glm::vec3(0.45f, 2.5f, 0.30f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera2 VL3
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 4.0f, 3.25f));
		model = glm::scale(model, glm::vec3(0.45f, 2.5f, 0.30f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera3 VL3
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 5.4f, 5.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.30f, 3.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera4 VL3
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 2.6f, 5.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.30f, 3.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Muro izquierdo interior
		glBindTexture(GL_TEXTURE_2D, texture10);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(10.9f, 3.9f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 8.0f, 16.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Muro Derecho
		glBindTexture(GL_TEXTURE_2D, texture2);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-11.0f, 4.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 8.0f, 16.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Muro derecho interior
		glBindTexture(GL_TEXTURE_2D, texture9);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.9f, 4.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 8.0f, 16.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Barra parte de atras
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.8f, 3.5f, 2.5f));
		model = glm::scale(model, glm::vec3(0.05f, 6.5f, 9.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//Smulación vidrio
		glBindTexture(GL_TEXTURE_2D, texture21);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.7f, 3.5f, 2.5f));
		model = glm::scale(model, glm::vec3(0.05f, 5.5f, 8.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Bar-Back
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-9.45f, 2.5f, 2.5f));
		model = glm::scale(model, glm::vec3(2.8f, 0.1f, 8.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Bar-Back 1
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-8.0f, 1.5f, 2.5f));
		model = glm::scale(model, glm::vec3(0.1f, 2.1f, 8.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Bar-Back 2
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-9.45f, 1.5f, 6.8f));
		model = glm::scale(model, glm::vec3(3.0f, 2.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Bar-Back 3
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-9.45f, 1.5f, -1.8f));
		model = glm::scale(model, glm::vec3(3.0f, 2.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//detalle 1
		glBindTexture(GL_TEXTURE_2D, texture19);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-7.9f, 2.3f, 2.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 8.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//detalle 2
		glBindTexture(GL_TEXTURE_2D, texture19);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-7.9f, 1.3f, 2.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 8.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Entrepaño 1
		glBindTexture(GL_TEXTURE_2D, texture19);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.3f, 4.0f, 2.5f));
		model = glm::scale(model, glm::vec3(1.5f, 0.1f, 8.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Entrepaño 2
		glBindTexture(GL_TEXTURE_2D, texture19);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.3f, 5.5f, 2.5f));
		model = glm::scale(model, glm::vec3(1.5f, 0.1f, 8.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Borde Muro derecho
		glBindTexture(GL_TEXTURE_2D, texture18);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.8f, 7.5f, 0.0f));
		model = glm::rotate(model, 1.57f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.7f, 0.235, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Techo
		glBindTexture(GL_TEXTURE_2D, texture2);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 8.01f, 0.0f));
		model = glm::scale(model, glm::vec3(22.0f, 0.2f, 16.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Techo_interior
		glBindTexture(GL_TEXTURE_2D, texture1);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 7.8f, 0.0f));
		model = glm::scale(model, glm::vec3(22.0f, 0.05f, 16.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Frente 1
		glBindTexture(GL_TEXTURE_2D, texture2);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.25f, 4.0f, -8.0f));
		model = glm::scale(model, glm::vec3(9.5f, 8.0f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Frente 1 Interior
		glBindTexture(GL_TEXTURE_2D, texture11);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.25f, 4.0f, -7.9f));
		model = glm::scale(model, glm::vec3(9.5f, 8.0f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Frente 2
		glBindTexture(GL_TEXTURE_2D, texture2);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.25f, 4.0f, -8.0f));
		model = glm::scale(model, glm::vec3(9.5f, 8.0f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Frente 2 Interior
		glBindTexture(GL_TEXTURE_2D, texture11);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.25f, 4.0f, -7.9f));
		model = glm::scale(model, glm::vec3(9.5f, 8.0f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Frente 3
		glBindTexture(GL_TEXTURE_2D, texture2);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 6.5f, -8.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Frente 3 Interior
		glBindTexture(GL_TEXTURE_2D, texture11);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 6.5f, -7.9f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//ACABADOS

		//Madera1
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 8.0f, 8.0f));
		model = glm::scale(model, glm::vec3(22.05f, 0.4f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//Madera2
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 8.0f, -8.0f));
		model = glm::scale(model, glm::vec3(22.05f, 0.4f, 0.7f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//Madera2.1
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 6.0f, -8.2f));
		model = glm::scale(model, glm::vec3(22.1f, 0.35f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//Madera3
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-11.35f, 8.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.4f, 16.7f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//Madera4
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.35f, 8.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.4f, 16.7f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//MADERAS INCLINADAS
		//Madera1
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, 7.0f, -8.2f));
		model = glm::rotate(model, 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 3.0f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera2
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(10.0f, 7.0f, -8.2f));
		model = glm::rotate(model, -0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 3.0f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera3
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(8.0f, 7.0f, -8.2f));
		model = glm::rotate(model, 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 3.0f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera4
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-8.0f, 7.0f, -8.2f));
		model = glm::rotate(model, -0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 3.0f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera5
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.0f, 7.0f, -8.2f));
		model = glm::rotate(model, 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 3.0f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera6
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.0f, 7.0f, -8.2f));
		model = glm::rotate(model, -0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 3.0f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera7
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.0f, 7.0f, -8.2f));
		model = glm::rotate(model, 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 3.0f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera8
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.0f, 7.0f, -8.2f));
		model = glm::rotate(model, -0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 3.0f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera9
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.0f, 7.0f, -8.2f));
		model = glm::rotate(model, 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 3.0f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera10
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.0f, 7.0f, -8.2f));
		model = glm::rotate(model, -0.8f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 3.0f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera11
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 7.0f, -8.2f));
		model = glm::scale(model, glm::vec3(0.2f, 2.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera12
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-1.0f, 7.0f, -8.2f));
		model = glm::scale(model, glm::vec3(0.2f, 2.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera13
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.0f, 7.0f, -8.2f));
		model = glm::scale(model, glm::vec3(0.2f, 2.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera14
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-7.0f, 7.0f, -8.2f));
		model = glm::scale(model, glm::vec3(0.2f, 2.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera15
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-5.0f, 7.0f, -8.2f));
		model = glm::scale(model, glm::vec3(0.2f, 2.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera16
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.0f, 7.0f, -8.2f));
		model = glm::scale(model, glm::vec3(0.2f, 2.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera17
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.0f, 7.0f, -8.2f));
		model = glm::scale(model, glm::vec3(0.2f, 2.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera18
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(3.0f, 7.0f, -8.2f));
		model = glm::scale(model, glm::vec3(0.2f, 2.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera19
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-9.0f, 7.0f, -8.2f));
		model = glm::scale(model, glm::vec3(0.2f, 2.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera20
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(9.0f, 7.0f, -8.2f));
		model = glm::scale(model, glm::vec3(0.2f, 2.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//MARCOS PUERTA Y VENTANAS

		//Puerta
		glBindTexture(GL_TEXTURE_2D, texture4);
		model = glm::mat4(1);
		modelTemp = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.5f, 2.5f, -8.0f));
		model = glm::rotate(model, glm::radians(puertaRot), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp = model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 5.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera1
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.5f, 2.5f, -8.0f));
		model = glm::scale(model, glm::vec3(0.25f, 5.0f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera2
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-1.5f, 2.5f, -8.0f));
		model = glm::scale(model, glm::vec3(0.25f, 5.0f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera3
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 5.125f, -8.0f));
		model = glm::scale(model, glm::vec3(3.25f, 0.3f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Ventana Izq
		glBindTexture(GL_TEXTURE_2D, texture5);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.26f, 3.5f, -8.0f));
		model = glm::scale(model, glm::vec3(4.0f, 3.0f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera1
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.25f, 1.875f, -8.0f));
		model = glm::scale(model, glm::vec3(4.0f, 0.35f, 0.45f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera2
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.25f, 5.1f, -8.0f));
		model = glm::scale(model, glm::vec3(4.0f, 0.25f, 0.45f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera3
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-8.375f, 3.475f, -8.0f));
		model = glm::scale(model, glm::vec3(0.25f, 3.5f, 0.45f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera4
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.375f, 3.475f, -8.0f));
		model = glm::scale(model, glm::vec3(0.25f, 3.5f, 0.45f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Ventana Der
		glBindTexture(GL_TEXTURE_2D, texture5);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.26f, 3.5f, -8.0f));
		model = glm::scale(model, glm::vec3(4.0f, 3.0f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera1
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.25f, 1.875f, -8.0f));
		model = glm::scale(model, glm::vec3(4.0f, 0.35f, 0.45f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera2
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.25f, 5.1f, -8.0f));
		model = glm::scale(model, glm::vec3(4.0f, 0.25f, 0.45f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera3
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(8.375f, 3.475f, -8.0f));
		model = glm::scale(model, glm::vec3(0.25f, 3.5f, 0.45f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera4
		glBindTexture(GL_TEXTURE_2D, texture6);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.375f, 3.475f, -8.0f));
		model = glm::scale(model, glm::vec3(0.25f, 3.5f, 0.45f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Letrero
		glBindTexture(GL_TEXTURE_2D, texture7);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 8.0f, -9.0f));
		model = glm::rotate(model, 3.15f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 0.3f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.7f, 2.8f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//INTERIOR

		//Paño mesa de billar
		glBindTexture(GL_TEXTURE_2D, texture12);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.0f, 1.7f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 0.0f, 6.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Fondo mesa de billar
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.0f, 0.76f, 0.0f));
		model = glm::scale(model, glm::vec3(2.15f, 0.0f, 6.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera 1 mesa
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.5f, 1.8f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.3f, 6.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera 2 mesa
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.5f, 1.8f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.3f, 6.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera 3 mesa
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.0f, 1.8f, 3.25f));
		model = glm::scale(model, glm::vec3(3.5f, 0.3f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera 4 mesa
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.0f, 1.8f, -3.25f));
		model = glm::scale(model, glm::vec3(3.5f, 0.3f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera base 1 mesa
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.3f, 1.3f, 0.0f));
		model = glm::rotate(model, -0.4f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.01f, 1.2f, 6.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera base 2 mesa
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.7f, 1.3f, 0.0f));
		model = glm::rotate(model, 0.4f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.01f, 1.2f, 6.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera base 3 mesa
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.0f, 1.3f, 3.0f));
		model = glm::scale(model, glm::vec3(2.1f, 1.1f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera base 4 mesa
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.0f, 1.3f, -3.0f));
		model = glm::scale(model, glm::vec3(2.1f, 1.1f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera palo 1
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.25f, 0.9f, -3.0f));
		model = glm::scale(model, glm::vec3(0.4f, 1.5f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera palo 2
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.25f, 0.9f, 3.0f));
		model = glm::scale(model, glm::vec3(0.4f, 1.5f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera palo 3
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.75f, 0.9f, -3.0f));
		model = glm::scale(model, glm::vec3(0.4f, 1.5f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Madera palo 4
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.75f, 0.9f, 3.0f));
		model = glm::scale(model, glm::vec3(0.4f, 1.5f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Puerta barra
		glBindTexture(GL_TEXTURE_2D, texture13);
		model = glm::mat4(1);
		modelTemp = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.5f, 1.0f, -2.6f));
		model = glm::rotate(model, glm::radians(puertaBRot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.4f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cadena 1 Lámpara
		glBindTexture(GL_TEXTURE_2D, texture20);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.0f, 7.5f, 2.0f));
		model = glm::scale(model, glm::vec3(0.1f, 1.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();

		//Cadena 2 Lámpara
		glBindTexture(GL_TEXTURE_2D, texture20);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.0f, 7.5f, 4.0f));
		model = glm::scale(model, glm::vec3(0.1f, 1.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();

		//Base Lámpara
		glBindTexture(GL_TEXTURE_2D, texture1);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.0f, 7.2f, 3.0f));
		model = glm::scale(model, glm::vec3(3.0f, 0.3f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();

		//Base2 Lámpara
		glBindTexture(GL_TEXTURE_2D, texture22);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.0f, 6.8f, 3.0f));
		model = glm::scale(model, glm::vec3(4.5f, 0.35f, 4.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();

		//Lámpara
		glBindTexture(GL_TEXTURE_2D, texture21);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.0f, 6.6f, 3.0f));
		model = glm::scale(model, glm::vec3(4.0f, 0.01f, 4.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();


		//Cadena 1 Lámpara2
		glBindTexture(GL_TEXTURE_2D, texture20);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.0f, 7.5f, 2.0f));
		model = glm::scale(model, glm::vec3(0.1f, 1.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();

		//Cadena 2 Lámpara2
		glBindTexture(GL_TEXTURE_2D, texture20);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.0f, 7.5f, 4.0f));
		model = glm::scale(model, glm::vec3(0.1f, 1.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();

		//Base Lámpara2
		glBindTexture(GL_TEXTURE_2D, texture1);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.0f, 7.2f, 3.0f));
		model = glm::scale(model, glm::vec3(3.0f, 0.3f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();

		//Base2 Lámpara2
		glBindTexture(GL_TEXTURE_2D, texture22);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.0f, 6.8f, 3.0f));
		model = glm::scale(model, glm::vec3(4.5f, 0.35f, 4.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();

		//Lámpara2
		glBindTexture(GL_TEXTURE_2D, texture21);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.0f, 6.6f, 3.0f));
		model = glm::scale(model, glm::vec3(4.0f, 0.01f, 4.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		view = camera.GetViewMatrix();



		//MODELOS
		//Rockola
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(-11.4f, 0.01f, 40.0f));
		model = glm::rotate(model, 4.7f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 1.6f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.36f, 0.36f, 0.36f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Rockola.Draw(lightingShader);

		//Barra1
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(-4.4f, 0.01f, 49.2f));
		model = glm::rotate(model, 1.6f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Barra.Draw(lightingShader);
		//Barra2
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(-4.5f, 0.01f, 45.8f));
		model = glm::rotate(model, 1.6f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Barra.Draw(lightingShader);
		//Barra3
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(-4.6f, 0.01f, 42.4f));
		model = glm::rotate(model, 1.6f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Barra.Draw(lightingShader);
		//Barra 4
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(-10.0f, 0.01f, 42.7f));
		model = glm::rotate(model, 3.17f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Barra.Draw(lightingShader);

		for (int i = 0; i <= 14; i++) {

		//Vodka1
		model = glm::mat4(1);
		model = glm::translate(model,glm::vec3(-10.0f, 4.0f, -1.0f+(i/2)));
		model = glm::rotate(model, 1.5f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Vodka.Draw(lightingShader);

		//Botella
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-10.0f, 4.0f, -0.5f + (i/2)));
		model = glm::rotate(model, -1.55f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		botella1.Draw(lightingShader);

		}

		for (int i = 0; i <= 14; i++) {

			//Vodka2
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-10.0f, 5.5f, -1.0f + (i/2)));
			model = glm::rotate(model, 1.5f, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Vodka.Draw(lightingShader);

			//Botella
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-10.0f, 5.5f, -1.5f + (i / 2)));
			model = glm::rotate(model, -1.55f, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			botella1.Draw(lightingShader);

		}


		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		glBindVertexArray(0);
			   		 		// Swap the screen buffers
		glfwSwapBuffers(window);

	}


	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{

	if (keys[GLFW_KEY_1])
	{
		range += 0.1;
		rot += 1;
		printf("El rango es %f\n", range);
	}

	if (keys[GLFW_KEY_2])
	{
		range -= 0.1;
		printf("El rango es %f\n", range);
	}

	if (keys[GLFW_KEY_3])
	{
		range += 0.1;
		printf("El spotangle es %f\n", range);
	}

	if (keys[GLFW_KEY_4])
	{
		range -= 0.1;
		printf("El spotangle es %f\n", range);
	}

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);

	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	if (keys[GLFW_KEY_T])
	{
		cuadross();
	}
	if (keys[GLFW_KEY_ESCAPE])
	{

	}

	if (keys[GLFW_KEY_Y])
	{
		mov_puertaB = true;
		animacion3();
	}

	if (keys[GLFW_KEY_H])
	{
		mov_puertaB = true;
		animacion4();
	}
	if (keys[GLFW_KEY_U])
	{
		mov_puerta = true;
		animacion1();
	}
	if (keys[GLFW_KEY_J])
	{
		mov_puerta = true;
		animacion2();
	}
	if (keys[GLFW_KEY_I])
	{
	
	}


}


void animacion1()
{

	if (mov_puerta) {
		if (puertaRot != -80) {
			puertaRot -= 1;
		}
		else {
			mov_puerta = false;
		}
	}

}


void animacion2()
{
	if (mov_puerta) {
		if (puertaRot != 0) {
			puertaRot += 1;
		}
		else {
			mov_puerta = false;
		}
	}

}

void animacion3()
{

	if (mov_puertaB) {
		if (puertaBRot != 80) {
			puertaBRot += 1;
		}
		else {
			mov_puertaB = false;
		}
	}

}


void animacion4()
{
	if (mov_puertaB) {
		if (puertaBRot != 0) {
			puertaBRot -= 1;
		}
		else {
			mov_puertaB = false;
		}
	}

}


void cuadross() {
	float temp;
	temp = cuadros[0];
	printf("\n%f 0 ",cuadros[0]);
	cuadros[0] = cuadros[1];
	printf("\n%f 1 ", cuadros[1]);
	cuadros[1] = cuadros[2];
	cuadros[2] = cuadros[3];
	cuadros[3] = temp;

}
// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{



	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(1.0f, 0.0f, 0.0f);
		else
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

