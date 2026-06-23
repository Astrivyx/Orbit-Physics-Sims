#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

class Planet
{
    public:

    int Mass;

    Planet(int M)
    {
        Mass = M;
    }
    
    void MassChange(int Change)
    {
        Mass += Change;
    }

};


    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
    glViewport(0, 0, width, height);
    }  

    void processInput(GLFWwindow *window) // uses window to process input
    {
        if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) // uses glfw to detect key pressed while user is in window
        {
            glfwSetWindowShouldClose(window, true); // closes
        }
    }


    std::vector<float> PlanetMath()
    {
        const int  Segments = 50;
        float Radius = 0.3f;
        float Pi = 3.14159;
        float x = -0.5f;
        float y = 0.5f;
        std::vector<float> Verticies;
        Verticies.push_back(x);
        Verticies.push_back(y);
        Verticies.push_back(0.0f);

        for (int i = 0; i <= Segments; i++) // using basic trig to convert points of a cirlce into a neat segments for the draw thing to run
        {
            float angle = (2.0f * Pi * i) / Segments;
            Verticies.push_back(x+(Radius * cos(angle)));
            Verticies.push_back(y+(Radius * sin(angle)));
            Verticies.push_back(0.0f);
        }
        for (int i = 0; i < Verticies.size(); i += 3)
        {
            std::cout << "v" << i/3 << ": ("
                    << Verticies[i]     << ", "
                    << Verticies[i + 1] << ", "
                    << Verticies[i + 2] << ")\n";
        }
        return Verticies;
    };
int main()
{
    // ---- WINDOW SETUP ----
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* OrbitSim = glfwCreateWindow(800, 800, "Orbit Sim", NULL, NULL);
    if (OrbitSim == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(OrbitSim);

    // ---- GLAD (must come before ANY gl* call) ----
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(OrbitSim, framebuffer_size_callback);

    // ---- VERTEX DATA ----
    std::vector<float> vertices = PlanetMath();

    // ---- BUFFER SETUP ----
    unsigned int VBO;
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO); // bind VAO first, then VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ---- VERTEX SHADER ----
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // ---- FRAGMENT SHADER ----
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
        "}\0";

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // ---- SHADER PROGRAM ----
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::Program::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ---- RENDER LOOP ----
    while (!glfwWindowShouldClose(OrbitSim))
    {
        processInput(OrbitSim);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw calls go INSIDE the loop
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 50+2);

        glfwSwapBuffers(OrbitSim);
        glfwPollEvents();
    }

    // ---- PLANET TEST ----
    Planet Earth = Planet(20000);
    std::cout << Earth.Mass << std::endl;
    Earth.MassChange(10);
    std::cout << Earth.Mass << std::endl;
    Earth.MassChange(-10);
    std::cout << Earth.Mass << std::endl;

    glfwTerminate();
    return 0;
}