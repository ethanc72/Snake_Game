#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <iomanip>

#include "shader.h"
#include "shaderprogram.h"


#include "snake.h"
#include "food.h"
#include<vector>
//initialization
bool key_states[256];
Head head;
Food food;
float positionx = head.x;
float positiony = head.y;
float foodpx = food.x;
float foodpy = food.y;
//shaders
ShaderProgram myShader;
ShaderProgram PerspectiveShader;
//objects
GLuint snake_VAO;
GLuint snake_VBO[2];

std::vector<glm::vec3> snake_vertices;

std::vector<glm::vec3> snake_colors;

GLuint food_VAO;
GLuint food_VBO[2];
std::vector<glm::vec3> food_vertices;
std::vector<glm::vec3> food_colors;


// Function that prints snake location
void print_snake(Head head) {
	using namespace std;
	cout << "Head: (" << head.x << ", " << head.y << ")" << head.score << "\n";
	Body* body = head.body;
	while (body != 0) {
		cout << "Body: (" << body->x << ", " << body->y << ")" << body->life_span << "\n";
		body = body->nextBody;
	}
	cout << "\n";
}

float roundUp(float num) {
	float absnum = std::abs(num);
	float numRoundedUp = std::round(absnum * 10) / 10;
	if (num < 0) {
		numRoundedUp = -numRoundedUp;
	}
	std::cout << numRoundedUp << std::endl;
	return numRoundedUp;
}
void keyboard_func(unsigned char key,int x ,int y) {
	key_states[key] = true;
	switch (key)
	{
		case 'd':
		{
			head.direction = 0;
			head.x = roundUp(head.x);
			break;
		}
		case 's':
		{
			head.direction = 1;
			head.y = roundUp(head.y);

			break;
		}
		case 'a':
		{
			head.direction = 2;
			head.x = roundUp(head.x);
			break;
		}
		case 'w':
		{
			head.direction = 3;
			head.y = roundUp(head.y);
			break;
		}
	}
}

void CreateShaders() {
	myShader.Create("./shaders/simple.vert", "./shaders/simple.frag");



}
void makeVec() {

	glm::vec3 v1(positionx, positiony, 1.0f);
	glm::vec3 v2(positionx + 0.1f, positiony, 1.0f);
	glm::vec3 v3(positionx, positiony + 0.1f, 1.0f);
	glm::vec3 v4(positionx + 0.1f, positiony + 0.1f, 1.0f);
	snake_vertices.push_back(v1);
	snake_vertices.push_back(v2);
	snake_vertices.push_back(v3);
	snake_vertices.push_back(v3);
	snake_vertices.push_back(v2);
	snake_vertices.push_back(v4);

}

void CreateSnake() {

	glm::vec3 color(0.5f, 1.0f, 0.5f);
	for (int i = 0; i < snake_vertices.size(); i++) {
		snake_colors.push_back(color);
	}
	glGenVertexArrays(1, &snake_VAO);
	glBindVertexArray(snake_VAO);

	glGenBuffers(2, &snake_VBO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, snake_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, snake_vertices.size() * sizeof(glm::vec3), &snake_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, snake_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, snake_colors.size() * sizeof(glm::vec3), &snake_colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);


}

void makeFood() {
	glm::vec3 v1(foodpx, foodpy, 1.0f);
	glm::vec3 v2(foodpx + 0.1f, foodpy, 1.0f);
	glm::vec3 v3(foodpx, foodpy + 0.1f, 1.0f);
	glm::vec3 v4(foodpx + 0.1f, foodpy + 0.1f, 1.0f);
	food_vertices.push_back(v1);
	food_vertices.push_back(v2);
	food_vertices.push_back(v3);
	food_vertices.push_back(v3);
	food_vertices.push_back(v2);
	food_vertices.push_back(v4);
}

void CreateFood() {


	glm::vec3 color(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < food_vertices.size();i++) {
		food_colors.push_back(color);
	}
	glGenVertexArrays(1, &food_VAO);
	glBindVertexArray(food_VAO);

	glGenBuffers(2, &food_VBO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, food_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, food_vertices.size() * sizeof(glm::vec3), &food_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, food_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, food_colors.size() * sizeof(glm::vec3), &food_colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}


void changeViewPort(int w, int h)
{
	glViewport(0, 0, w, h);
}
int count = 10;
void timer(int value) {
	// update the scene here

	if (head.alive) {  
		while (snake_vertices.size() > 0) {
			snake_vertices.pop_back();
			snake_colors.pop_back();
		}
		while (food_vertices.size() > 0) {
			food_vertices.pop_back();
			food_colors.pop_back();
		}

		positionx = head.x;
		positiony = head.y;
			
		Body* body = head.body;
		while (body != 0) {
			positionx = body->x;
			positiony = body->y;
			makeVec();
			body = body->nextBody;
		}

		
		foodpx = food.x;
		foodpy = food.y;
		makeFood();

		CreateSnake();
		CreateFood();
		

		head.turn(food);

		glutKeyboardFunc(keyboard_func);

		// request a redraw
		glutPostRedisplay();

		// set the timer for the next frame
		glutTimerFunc(16, timer, 0);
	}
}
void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myShader.Use();

	//draw snake
	glBindVertexArray(snake_VAO);
	glDrawArrays(GL_TRIANGLES, 0, snake_vertices.size());

	glBindVertexArray(food_VAO);
	glDrawArrays(GL_TRIANGLES, 0, food_vertices.size());

	glutSwapBuffers();
}

void init(void) {
	CreateShaders();

	CreateSnake();
	CreateFood();
}
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800,800);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutCreateWindow("Snake Game");

	// Initialize GLEW
	GLenum ret = glewInit();
	if (ret != GLEW_OK) {
		std::cerr << "GLEW initialization error." << std::endl;
		glewGetErrorString(ret);
		return -1;
	} 


	//Draw snake
	glutDisplayFunc(render);
	glutReshapeFunc(changeViewPort);
	//glutKeyboardFunc(keyboard_func);


	glutTimerFunc(0, timer, 0);

	init();
	// Register callback functions

	glutMainLoop();
	return 0;
}
