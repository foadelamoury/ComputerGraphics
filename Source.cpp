#include <iostream>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/glm/glm.hpp>
#include <vector>
#include <cstdlib>

using namespace std;
using namespace glm;

const GLint WIDTH = 900, HEIGHT = 800;
GLuint VBO, BasiceprogramId;

vector<vec3> positions;

GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

void GenerateSierpinskiPoints(int iterations) {
    // Define the vertices of the main triangle
    vec3 vertexA = vec3(-0.9f, -0.9f, 0.0f);
    vec3 vertexB = vec3(0.9f, -0.9f, 0.0f);
    vec3 vertexC = vec3(0.0f, 0.9f, 0.0f);

    // Starting point (can be any random point inside the triangle)
    vec3 currentPoint = vec3(0.0f, 0.0f, 0.0f);

    // Add points using the Chaos Game
    for (int i = 0; i < iterations; ++i) {
        int randomVertex = rand() % 3;

        // Move halfway towards a random vertex
        if (randomVertex == 0) {
            currentPoint = (currentPoint + vertexA) * 0.5f;
        }
        else if (randomVertex == 1) {
            currentPoint = (currentPoint + vertexB) * 0.5f;
        }
        else {
            currentPoint = (currentPoint + vertexC) * 0.5f;
        }

        // Store the point
        positions.push_back(currentPoint);
    }

    // Update the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * positions.size(), positions.data(), GL_DYNAMIC_DRAW);
}

void Drawing() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
    glEnableVertexAttribArray(0);

    GenerateSierpinskiPoints(9000);
}

int Init() {
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        cout << "Error";
        getchar();
        return 1;
    }
    else {
        if (GLEW_VERSION_3_0)
            cout << "Driver support OpenGL 3.0\nDetails:\n";
    }
    cout << "\tUsing glew " << glewGetString(GLEW_VERSION) << endl;
    cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
    cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
    cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
    cout << "\tGLSL:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    Drawing();
    glClearColor(0, 0.5, 0.5, 1);
    return 0;
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_POINTS, 0, positions.size());
}

int main() {
    sf::ContextSettings context;
    context.depthBits = 24;
    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Sierpinski's Triangle", sf::Style::Close, context);

    if (Init()) return 1;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        Render();
        window.display();
    }

    return 0;
}
