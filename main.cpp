#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

std::vector<float> PlanetMath(float Radius,float x,float y)
    {
        int WorldSize = 10000;
        const int  Segments = 50;
        float Pi = 3.14159;
        x = x/(WorldSize/2);
        y= y/(WorldSize/2);
        Radius = Radius/(WorldSize/2);

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
        glBufferSubData(GL_ARRAY_BUFFER,0,Verticies.size()*sizeof(float),Verticies.data());
        return Verticies;
    };
class Planet
{
    public:

    double Mass;
    double xVel;
    double yVel;
    double Xpos;
    double Ypos;
    double Radius;
    float Red;
    float Green;
    float Blue;

    Planet(double M,double Rad,double Xv,double Yv,double X,double Y,float R,float G,float B)
    {
        Mass = M;
        Radius = Rad;
        xVel = Xv;
        yVel = Yv;
        Xpos = X;
        Ypos = Y;
        Red = R;
        Green = G;
        Blue = B;
    }
    
    void PosChange(double x,double y)
    {
     Xpos += x;
     Ypos += y;
     PlanetMath(Radius,Xpos,Ypos);
    }
    
    void VelocityChange(double x,double y)
    {
     xVel += x;
     yVel += y;
    }

    void Movement(double Dt)
    {
        Xpos += xVel*Dt;
        Ypos += yVel*Dt;
        PlanetMath(Radius,Xpos,Ypos);
    }

};

void PlanetRenderCall(Planet& Planet,GLint FragColor,GLuint PlanetVBO,GLuint PlanetVAO,double Dt)
{
    glUniform4f(FragColor, Planet.Red, Planet.Green, Planet.Blue, 0.0f);
    glBindBuffer(GL_ARRAY_BUFFER, PlanetVBO); // bind buffer so it moves the current obj
    glBindVertexArray(PlanetVAO);
    Planet.Movement(Dt);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 50+2);
}

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
    glViewport(0, 0, width, height);
    }  

    void processInput(GLFWwindow *window,Planet& Earth,Planet& Venus,double Dt) // uses window to process input
    {
        if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) // uses glfw to detect key pressed while user is in window
        {
            glfwSetWindowShouldClose(window, true); // closes
        }
        if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS) // uses glfw to detect key pressed while user is in window
        {
            Earth.VelocityChange(200*Dt,0); // Changes x vel
        }
        if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS) // uses glfw to detect key pressed while user is in window
        {
            Earth.VelocityChange(-200*Dt,0); // Changes x vel
        }
        if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS) // uses glfw to detect key pressed while user is in window
        {
            Earth.VelocityChange(0,200*Dt); // Changes y vel
        }
        if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS) // uses glfw to detect key pressed while user is in window
        {
            Earth.VelocityChange(0,-200*Dt); // Changes y vel
        }
        if(glfwGetKey(window,GLFW_KEY_RIGHT) == GLFW_PRESS) // uses glfw to detect key pressed while user is in window
        {
            Venus.VelocityChange(200*Dt,0); // Changes x vel
        }
        if(glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_PRESS) // uses glfw to detect key pressed while user is in window
        {
            Venus.VelocityChange(-200*Dt,0); // Changes x vel
        }
        if(glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS) // uses glfw to detect key pressed while user is in window
        {
            Venus.VelocityChange(0,200*Dt); // Changes y vel
        }
        if(glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS) // uses glfw to detect key pressed while user is in window
        {
            Venus.VelocityChange(0,-200*Dt); // Changes y vel
        }
    }

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


    Planet Earth = Planet(323753.610726,600,0,0,200,600,0.0f,1.0f,0.0f);
    Planet Venus = Planet(263840.598674,550,0,0,400,900,1.0f,0.0f,0.0f);

    // ---- VERTEX DATA ----
    std::vector<float> vertices = PlanetMath(Earth.Radius,Earth.Xpos,Earth.Ypos);
    std::vector<float> Venusvert = PlanetMath(Venus.Radius,Venus.Xpos,Venus.Ypos);
    // ---- BUFFER SETUP ----
    GLuint Planet1VAO , Planet1VBO;
    glGenVertexArrays(1, &Planet1VAO);
    glGenBuffers(1, &Planet1VBO);

    glBindVertexArray(Planet1VAO);
    glBindBuffer(GL_ARRAY_BUFFER, Planet1VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    // Configure vertex attribute pointers
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
        "uniform vec4 myCustomColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = myCustomColor;\n"
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

    GLuint Planet2VAO , Planet2VBO;
    glGenVertexArrays(1, &Planet2VAO);
    glGenBuffers(1, &Planet2VBO);

    glBindVertexArray(Planet2VAO);
    glBindBuffer(GL_ARRAY_BUFFER, Planet2VBO);
    glBufferData(GL_ARRAY_BUFFER, Venusvert.size()*sizeof(float), Venusvert.data(), GL_DYNAMIC_DRAW);
    // Configure vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

// Unbind to secure the state
glBindVertexArray(0);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::Program::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint FragColor = glGetUniformLocation(shaderProgram,"myCustomColor");
    // ---- RENDER LOOP ----
    double LastFrame = 0;
    double xPosChange = 0;
    while (!glfwWindowShouldClose(OrbitSim))
    {
        double StartFrameTime = glfwGetTime();
        processInput(OrbitSim,Earth,Venus,StartFrameTime-LastFrame);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
        // draw calls go INSIDE the loop
        glUseProgram(shaderProgram);

        PlanetRenderCall(Earth,FragColor,Planet1VBO,Planet1VAO,StartFrameTime-LastFrame);


        PlanetRenderCall(Venus,FragColor,Planet2VBO,Planet2VAO,StartFrameTime-LastFrame);

        glfwSwapBuffers(OrbitSim);
        glfwPollEvents();

        std::cout<<"Fps Per Second:"<<1/(StartFrameTime-LastFrame)<<std::endl;
        LastFrame = StartFrameTime;
    }

    // ---- PLANET TEST ----

    glfwTerminate();
    return 0;
}