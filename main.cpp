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



int main()
{
    Planet Earth = Planet(20000);
    std::cout << Earth.Mass << std::endl;
    Earth.MassChange(10);
    std::cout << Earth.Mass << std::endl;
    return 0;
}