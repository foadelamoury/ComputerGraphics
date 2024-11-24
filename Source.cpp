#include <iostream>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include<gl\glm\glm.hpp>

using namespace std;
using namespace glm;

enum DrawingMode
{
	Points,
	Lines,
	FilledTriangle
};

struct Vertex
{
	vec3 Position;
	vec3 COlor;
};

GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

const GLint WIDTH = 800, HEIGHT = 800;
GLuint VBO, BasiceprogramId;
DrawingMode Current_DrawingMode = DrawingMode::FilledTriangle;
float triangleSize = 0;
void CreateTriangle()
{
	GLfloat TriangleVertices[180];
	//=
	//{
	//	0,0,0,
	//	0,0,0,
	//	0,0,0,
	//	0,0,0,
	//	0,0,0,
	//	0,0,0
	//	/*
	/*	-1,-1,0,
		1,-1,0,
		1,0,0,
		1,1,0,
		0,1,0,
		-1,1,0*/
	//	-2,1,0,
	//	-2,0,0,
	//	-2,-2,0,
	//	-1,-3,0,
	//	1,-3,0,
	//	1,-1,0,
	//	-1,0,0*/
	//};
	triangleSize = (sizeof(TriangleVertices) / 12);
	TriangleVertices[0] = 0;
	TriangleVertices[1] = 0;
	TriangleVertices[2] = 0;
	for (int i = 3; i < triangleSize * 3; i += 3)
	{
		TriangleVertices[i] = cos((2 * 3.14 * (i - 3)) / ((triangleSize * 3)-6));
		TriangleVertices[i + 1] = sin((2 * 3.14 * (i - 3)) / ((triangleSize * 3) - 6));
		TriangleVertices[i + 2] = 0;


	}

	// create buffer object
	glGenBuffers(1, &VBO);

	// binding buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleVertices), TriangleVertices, GL_STATIC_DRAW);
	
	// shader
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	//GLuint Vertex_Position_Location = glGetAttribLocation(BasiceprogramId, "vertex_position");
}

void CreateColoredTriangle()
{
	GLfloat TriangleVertices[] =
	{
		-1,-1,0,0,1,0,
		1,-1,0,0,0,1,
		0,1,0,1,0,0
	};

	// create buffer object
	glGenBuffers(1, &VBO);

	// binding buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleVertices), TriangleVertices, GL_STATIC_DRAW);

	// shader
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(GL_FLOAT), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(GL_FLOAT), (char*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
}

void CompileShader(const char* vertex_shader_file_name, const char* fragment_shader_file_namering, GLuint& programId)
{
	programId = InitShader(vertex_shader_file_name, fragment_shader_file_namering);
	glUseProgram(programId);
}

int Init()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		cout << "Error";
		getchar();
		return 1;
	}
	else
	{
		if (GLEW_VERSION_3_0)
			cout << "Driver support OpenGL 3.0\nDetails:\n";
	}
	cout << "\tUsing glew " << glewGetString(GLEW_VERSION) << endl;
	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
	cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	CompileShader("VS.glsl", "FS.glsl", BasiceprogramId);
	CreateTriangle();
	//CreateColoredTriangle();

	glClearColor(0, 0.5, 0.5, 1);

	return 0;
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	switch (Current_DrawingMode)
	{
	case Points:
		glPointSize(10);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case Lines:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case FilledTriangle:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	default:
		break;
	}

	glDrawArrays(GL_TRIANGLE_FAN, 0, triangleSize);
}

float theta = 0;
void Update()
{
	// add all tick code
	theta += 0.0001f;

	GLuint Theta_Location = glGetUniformLocation(BasiceprogramId, "theta");
	glUniform1f(Theta_Location, theta);
}

int main()
{
	sf::ContextSettings context;
	context.depthBits = 24;
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Computer Graphics", sf::Style::Close, context);

	if (Init()) return 1;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
			{
				window.close();
				break;
			}
			case sf::Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::Num1)
				{
					Current_DrawingMode = DrawingMode::Points;
				}
				if (event.key.code == sf::Keyboard::Num2)
				{
					Current_DrawingMode = DrawingMode::Lines;
				}
				if (event.key.code == sf::Keyboard::Num3)
				{
					Current_DrawingMode = DrawingMode::FilledTriangle;
				}
				break;
			}
			}
		}

		//Update();
		Render();

		window.display();
	}
	return 0;
}