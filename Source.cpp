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

const GLint WIDTH = 900, HEIGHT = 800;
GLuint VBO, BasiceprogramId;
DrawingMode Current_DrawingMode = DrawingMode::Lines;
GLboolean canDraw = false;
vector<int> noDraws = { 0 };
vector<vec3> positions;
void onMouseMove(int x, int y) {
	if (canDraw) {
		float normalizedX = (2.0f * x) / WIDTH - 1.0f;
		float normalizedY = 1.0f - (2.0f * y) / HEIGHT; 

		positions.push_back(vec3(normalizedX, normalizedY, 0.0f));

		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * positions.size(), positions.data(), GL_DYNAMIC_DRAW);
	}
}

void Drawing() {
	glClearColor(1.0, 1.0, 1.0, 0.0); 
	glLineWidth(5);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * positions.capacity(), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
	glEnableVertexAttribArray(0);
}


void CreateColoredTriangle()
{
	GLfloat TriangleVertices[] =
	{
		-1,-1,0,0,1,0,
		1,-1,0,0,0,1,
		0,1,0,1,0,0
	};

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleVertices), TriangleVertices, GL_STATIC_DRAW);

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
	Drawing();

	glClearColor(0, 0.5, 0.5, 1);

	return 0;
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < noDraws.size() - 1; i++)
	{
		glDrawArrays(GL_LINE_STRIP, noDraws[i], noDraws[i + 1] - noDraws[i]);
	}
	glDrawArrays(GL_LINE_STRIP, noDraws[noDraws.size() - 1], positions.size() - noDraws[noDraws.size() - 1]);
}

float theta = 0;
void Update()
{
	theta += 0.0001f;

	GLuint Theta_Location = glGetUniformLocation(BasiceprogramId, "theta");
	glUniform1f(Theta_Location, theta);
}

int main()
{
	sf::ContextSettings context;
	context.depthBits = 24; 
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "I hear you paint houses", sf::Style::Close, context);

	if (Init()) return 1;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				canDraw = false;
				noDraws.push_back(positions.size());
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				canDraw = true;
				noDraws.push_back(positions.size());
			}
			else if (event.type == sf::Event::MouseMoved) {
				onMouseMove(event.mouseMove.x, event.mouseMove.y);
			}
		}

		Update();

		glClear(GL_COLOR_BUFFER_BIT);
		Render();

		window.display();
	}

	return 0;
}
