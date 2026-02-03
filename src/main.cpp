#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
enum ShaderType {
    VERTEX,
    FRAGMENT1,
    FRAGMENT2
};
enum ProgramId {
    PROGRAM1,
    PROGRAM2
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
void processInput(GLFWwindow *);
bool shaderCreation(unsigned int &shaderProgram, unsigned int &shaderProgram2);
bool checkShaderCompilation(GLuint shader, ShaderType);
bool checkProgramLink(GLuint program, ProgramId);
const char *vertexShaderSource = "#version 330\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
const char *fragmentShaderSource2 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\n\0";

int main()
{
    //Always initialize GLFW
    if(!glfwInit())
    {
        std::cout << "Failed to init GLFW\n";
        return -1;
    }
    
    //set minimum OPENGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Creating the window
    GLFWwindow* window = glfwCreateWindow(800,600,"Test",NULL,NULL);
    if(!window)
    {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
    //Setting the current context to the created window
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Loading GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD\n";
        return -1;
    }
    
    //Compiling shaders and creating shader program
    unsigned int shaderProgram, shaderProgram2;
    shaderProgram = glCreateProgram();
    shaderProgram2 = glCreateProgram();
    if(!shaderCreation(shaderProgram, shaderProgram2))
    {
        std::cout << "shader setup failed";
        return-1;
    }
    
    float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
     -0.5f,  0.5f, 0.0f,

     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);


    //Running the window
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
         
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(shaderProgram2);
        glDrawArrays(GL_TRIANGLES,3, 3 );
        
        glBindVertexArray(0);
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgram2);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

bool shaderCreation(unsigned int &shaderProgram, unsigned int &shaderProgram2)
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    if(!checkShaderCompilation(vertexShader, VERTEX))
        return false;

    unsigned int fragmentShader, fragmentShader2;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    if(!checkShaderCompilation(fragmentShader, FRAGMENT1))
        return false;

    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);

    if(!checkShaderCompilation(fragmentShader2, FRAGMENT2))
        return false;
    
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    if(!checkProgramLink(shaderProgram, PROGRAM1))
        return false;
    
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);
    if(!checkProgramLink(shaderProgram2, PROGRAM2))
        return false;
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader2);
    return true;
}

bool checkShaderCompilation(GLuint shader, ShaderType type)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);

        switch(type)
        {
            case VERTEX:
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
                break;

            case FRAGMENT1:
                std::cout << "ERROR::SHADER::FRAGMENT1::COMPILATION_FAILED\n";
                break;

            case FRAGMENT2:
                std::cout << "ERROR::SHADER::FRAGMENT2::COMPILATION_FAILED\n";
                break;
            
        }
        std::cout << infoLog << std::endl;
        return false;
    }
    return true;
}
bool checkProgramLink(GLuint program, ProgramId programid)
{
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    
    if(!success)
    {
        glGetProgramInfoLog(program,512, NULL, infoLog);

        switch(programid)
        {
            case PROGRAM1:
                std:: cout << "ERROR::PROGRAM1::LINK_FAILED\n";
                break;

            case PROGRAM2:
                std:: cout << "ERROR::PROGRAM2::LINK_FAILED\n";
                break;
        }
        std::cout << infoLog << std::endl;
        return false;
    }
    return true;
}