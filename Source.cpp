	#include <iostream>
	#include <gl/glew/glew.h>
	#include <SFML/Graphics.hpp>
	#include <SFML/OpenGL.hpp>
	#include<gl\glm\glm.hpp>
	#include<gl\glm\gtc\type_ptr.hpp>
	#include<gl\glm\gtx\transform.hpp>

	using namespace std;
	using namespace glm;

	enum DrawingMode
	{
		Points,
		Lines,
		FilledTriangle
	};

	struct vertex
	{
		vec3 position;
		vec3 color;
		vertex() {}
		vertex(vec3 _position, vec3 _color) :position{ _position }, color{ _color }
		{
		}
		vertex(vec3 _position) :position{ _position }, color{ _position }
		{
		}
	};

	GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

	const GLint WIDTH = 800, HEIGHT = 800;
	GLuint VBO_Triangle, VBO_Cube, IBO, BasiceprogramId;
	DrawingMode Current_DrawingMode = DrawingMode::FilledTriangle;

	// transformation
	GLuint modelMatLoc, viewMatLoc, projMatLoc;

	void CreateTriangle()
	{
		vertex TriangleVertices[] =
		{
			vec3(-1,-1,0),vec3(0,1,0),
			vec3(1,-1,0),vec3(0,0,1),
			vec3(0,1,0),vec3(1,0,0)
		};

		glGenBuffers(1, &VBO_Triangle);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_Triangle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleVertices), TriangleVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(GL_FLOAT), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(GL_FLOAT), (char*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);

		//GLuint Vertex_Position_Location = glGetAttribLocation(BasiceprogramId, "vertex_position");
	}

	void BindTriangle()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Triangle);

		glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(GL_FLOAT), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(GL_FLOAT), (char*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);
	}


	vertex cube_core_vertices[] = {
	{vec3(-0.5, 0.5, 0.5),vec3(1,0,0)},
	{vec3(-0.5, -0.5, 0.5),vec3(0,1,0)},
	{vec3(0.5, -0.5, 0.5),vec3(1,0,1)},
	{vec3(0.5, 0.5, 0.5),vec3(0,0,1)},
	{vec3(0.5, 0.5, -0.5),vec3(1,1,0)},
	{vec3(0.5, -0.5, -0.5),vec3(0,1,1)},
	{vec3(-0.5, -0.5, -0.5),vec3(1,1,1)},
	{vec3(-0.5, 0.5, -0.5),vec3(0,0,0)}
	};

	void CreateCube()
	{
		int vertices_Indeces[] = {
			//front
			0,
			1,
			2,

			0,
			2,
			3,
			//Right
			3,
			2,
			5,

			3,
			5,
			4,
			//Back
			4,
			5,
			6,

			4,
			6,
			7,
			//Left
			7,
			6,
			1,

			7,
			1,
			0,
			//Top
			7,
			0,
			3,

			7,
			3,
			4,
			//Bottom
			2,
			1,
			6,

			2,
			6,
			5
		};

		glGenBuffers(1, &VBO_Cube);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_core_vertices), cube_core_vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices_Indeces), vertices_Indeces, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertex), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(vertex), (char*)(sizeof(vec3)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(vertex), (char*)(2 * sizeof(vec3)));
		glEnableVertexAttribArray(2);

	}

	void BindCube()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertex), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(vertex), (char*)(sizeof(vec3)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(vertex), (char*)(2 * sizeof(vec3)));
		glEnableVertexAttribArray(2);
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
		CreateCube();

		modelMatLoc = glGetUniformLocation(BasiceprogramId, "modelMat");
		viewMatLoc = glGetUniformLocation(BasiceprogramId, "viewMat");
		projMatLoc = glGetUniformLocation(BasiceprogramId, "projMat");

		 mat4 viewMat =  lookAt( vec3(0, 0, 3),  vec3(0, 0, 0),  vec3(0, 1, 0));
		glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE,  value_ptr(viewMat));

		 mat4 projMat =  perspectiveFov(60.0f, (float)WIDTH, (float)HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(projMatLoc, 1, GL_FALSE,  value_ptr(projMat));

		glClearColor(0, 0.5, 0.5, 1);
		glEnable(GL_DEPTH_TEST);

		return 0;
	}

	float theta = 0;

#pragma region Camera Variables
	 vec3 cameraPos(0.0f, 0.0f, 3.0f);
	 vec3 cameraFront(0.0f, 0.0f, -1.0f);
	 vec3 cameraUp(0.0f, 1.0f, 0.0f);
	float cY = -90.f;
#pragma endregion


	void Update() {
		
		 vec3 front;
		front.x = cos( radians(cY));
		front.y = 0; 
		front.z = sin( radians(cY));
		cameraFront =  normalize(front);

		 mat4 viewMat =  lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE,  value_ptr(viewMat));
	}



	void DrawHuman()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

		BindCube();
		GLsizei elementSize = 36;
		mat4 rotation = rotate(theta, vec3(0, 1, 0));

		mat4 ModelMat = rotation *  translate( vec3(0, .75f, 0)) *
			 scale( vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE,  value_ptr(ModelMat));
		glDrawElements(GL_TRIANGLES, elementSize, GL_UNSIGNED_INT, NULL);

		ModelMat = rotation *  translate( vec3(0, 0.25f, 0)) *
			 scale( vec3(0.5f, 0.75f, 0.25f));
		glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE,  value_ptr(ModelMat));
		glDrawElements(GL_TRIANGLES, elementSize, GL_UNSIGNED_INT, NULL);

		ModelMat = rotation *  translate( vec3(-0.375f, 0.375f, 0)) *
			 scale( vec3(0.15f, 0.5f, .15f));
		glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE,  value_ptr(ModelMat));
		glDrawElements(GL_TRIANGLES, elementSize, GL_UNSIGNED_INT, NULL);

		ModelMat = rotation *  translate( vec3(0.375f, 0.375f, 0)) *
			 scale( vec3(0.15f, .5f, .15f));
		glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE,  value_ptr(ModelMat));
		glDrawElements(GL_TRIANGLES, elementSize, GL_UNSIGNED_INT, NULL);

		ModelMat = rotation *  translate( vec3(-0.15f, -0.375f, 0)) *
			 scale( vec3(0.2f, .5f, .2f));
		glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE,  value_ptr(ModelMat));
		glDrawElements(GL_TRIANGLES, elementSize, GL_UNSIGNED_INT, NULL);

		ModelMat = rotation *  translate( vec3(0.15f, -0.375f, 0)) *
			 scale( vec3(0.2f, .5f, .2f));
		glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE,  value_ptr(ModelMat));
		glDrawElements(GL_TRIANGLES, elementSize, GL_UNSIGNED_INT, NULL);
	}

	void Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

		DrawHuman();
	}

	int main()
	{
		sf::ContextSettings context;
		context.depthBits = 24;
		sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "It's a bird, it's a plane, it's a... human??!", sf::Style::Close, context);

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
					else if (event.key.code == sf::Keyboard::W) 
					{ cameraPos += 0.05f * cameraFront; }
					else if (event.key.code == sf::Keyboard::S) 
					{ cameraPos -= 0.05f * cameraFront; }
					else if (event.key.code == sf::Keyboard::A) 
					{ cameraPos -=  normalize( cross(cameraFront, cameraUp)) * 0.05f; }
					else if (event.key.code == sf::Keyboard::D) 
					{ cameraPos +=  normalize( cross(cameraFront, cameraUp)) * 0.05f; }
					else if (event.key.code == sf::Keyboard::Q) {
						cY -= 1.0f; 
					}
					else if (event.key.code == sf::Keyboard::E) {
						cY += 1.0f; 
					}
				}
	
					break;
				}
				
			}

			Update();
			Render();

			window.display();
		}
		return 0;
	}
