#include <iostream>

#include "GLFW/linmath.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

struct Vertex
{
    float x, y, z;  // Vertex coordinates
    float r, g, b;  // Vertex colors
};

struct Entity
{
    Vertex vertices;
};

int main(int argc, char const *argv[])
{

    Entity triangle[3] = {
        Vertex { -0.6f, -0.4f, 0.0f, 1.f, 0.f, 0.f },
        Vertex {  0.6f, -0.4f, 0.0f, 0.f, 1.f, 0.f },
        Vertex {   0.f,  0.6f, 0.0f, 0.f, 0.f, 1.f }
    };

static const char* vertex_shader_text =
    "#version 110\n"
    "uniform mat4 MVP;\n"
    "attribute vec3 vCol;\n"
    "attribute vec3 vPos;\n"
    "varying vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = MVP * vec4(vPos, 2.0);\n"
    "    color = vCol;\n"
    "}\n";
    
    static const char* fragment_shader_text =
    "#version 110\n"
    "varying vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = vec4(color, 1.0);\n"
    "}\n";
    

    int width, height;
    GLFWwindow* window;

    if (!glfwInit())
    {
        std::cout << "Could not init glfw!!!" << std::endl;
    }

    window = glfwCreateWindow(640, 480, "My Window", NULL, NULL);
    if (!window)
    {
        std::cout << "Could not create a window!!!" << std::endl;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();


    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // get version info
    // const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    // const GLubyte* version = glGetString(GL_VERSION); // version as a string

    // glEnable(GL_DEPTH_TEST); // enable depth-testing
    // glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    // gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    glfwSetKeyCallback(window, key_callback);


    unsigned int vertex_buffer, vertex_shader, fragment_shader, program, mvp_location, vpos_location, vcol_location;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
 
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
 
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
 
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
 
    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");
 
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(triangle[0]), (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(triangle[0]), (void*) (sizeof(float) * 3));
 

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        mat4x4 m, p, mvp;
 
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
 
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
 
        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);
 
        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }    

    return 0;
}
