#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

void error_callback(int , const char*);
void processInput(GLFWwindow *);
void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
int main()
{
    //Always initialize GLFW
    if(!glfwInit())
    {
        std::cout << "Failed to init GLFW\n";
        return -1;
    }
    glfwSetErrorCallback(error_callback);
    
    //set minimum OPENGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Creating the window
    GLFWwindow* window = glfwCreateWindow(800,600,"Test",NULL,NULL);
    if(!window)
    {
        std::cout << "Failed to init window\n";
        glfwTerminate();
        return -1;
    }
    //Setting the current context to the created window
    glfwMakeContextCurrent(window);

    //Loading GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD\n";
        return -1;
    }
    glViewport(0, 0, 800, 600);
        

    //Running the window
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

        glfwSwapBuffers(window);
        glfwPollEvents();   
    }
    glfwTerminate();
    return 0;
}
void error_callback(int error, const char* description)
{
    std::ofstream errorFile("error.txt"); 
    errorFile << "Error" << description << "\n";
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
}