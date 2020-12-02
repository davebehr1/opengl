#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id,1,&src,nullptr);
    glCompileShader(id);

    int result;

    glGetShaderiv(id,GL_COMPILE_STATUS,&result);
    if(result == GL_FALSE){
        int length;
        glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id,length, &length, message);
        std::cout << "Failed to compile shader!" << (type == GL_VERTEX_SHADER ? "vertex" :"fragment") << std::endl;
        std::cout << message <<std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program,vs);
    glAttachShader(program,fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // glewInit();
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "open gl tutorial", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    std::cout <<glGetString(GL_VERSION) << std::endl;
    std::cout << "shader version:" <<glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    
    float positions[6] = {
        -0.5f,-0.5f,
        0.0f, 0.5f,
        0.5f,-0.5f
    };


    unsigned int buffer;
    glGenBuffers(1,&buffer);
    glBindBuffer(GL_ARRAY_BUFFER,buffer);
    glBufferData(GL_ARRAY_BUFFER,6 * sizeof(float),positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, sizeof(float) * 2,0);

    std::string vertexShader = 
        "#version 120 core \n"
        "\n"
        "layout(location = 0) in vec4 position;"
        "\n"
        "void main()\n"
        "{\n"
        "  gl_position = position;\n"
        "}\n";

    std::string fragmentShader = 
    "#version 120 core \n"
    "\n"
    "layout(location = 0) out vec4 color;"
    "\n"
    "void main()\n"
    "{\n"
    "  color = vec4(1.0,0.0,0.0,1.0);\n"
    "}\n";


    unsigned int shader = CreateShader(vertexShader,fragmentShader);
    glUseProgram(shader);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}