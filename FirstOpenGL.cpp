
#include <iostream>
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <conio.h>
#include "Classes/Shader.hpp"
#include "stb_image.h"

// Method declarations
void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void initializeGLFWConfig();
bool validateShaderCompilation(unsigned int shader, std::string shaderType);
bool validateProgramCompilation(unsigned int program);

int main()
{
    initializeGLFWConfig();
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if(window == nullptr)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    // This is to get the function pointers for OS specific functions through GLAD, still don't really understand this
    // Needs to be called before any OpenGL functions are called. defined with gl
    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }
    
    //This gets called whenever the window is resized. the callback will set the viewport to the new size of the window
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    // Create and configure vertex array objects and vertex buff objects 
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Triangle with color example
    // User Shader class to create shaders and program
    Shader triColorShader;
    try
    {
        triColorShader.initializeProgram("Shaders/triColor.vert", "Shaders/triColor.frag");
    }
    catch(std::exception &e)
    {
        std::cout << e.what() << "\n";
        std::cout << "Press any key to continue...";
        _getch();
        return -1;
    }
    float triColorVertices[] {
        // positions        // colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };

    //Tricolor Triangle
    // glBindVertexArray(VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(triColorVertices), triColorVertices, GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr); // This Attribute pointer is located at 0, and contains the vertices coordinates in Normalized space.
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FLOAT, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));// This attribute pointer is located at 1, and contains the color data that will be passed to the fragment shader
    // glEnableVertexAttribArray(1);
    // *** I am assuming that in the statement above, the last argument would be the offset of where the pointer starts in the array. which would be 12 (3 * the size of a float, which is 4 bytes)

    // Rectangle with texture example
    Shader rectangleWithTextureShader;
    try
    {
        rectangleWithTextureShader.initializeProgram("Shaders/rectangleWithTexture.vert","Shaders/rectangleWithTexture.frag");
    }
    catch(std::exception &e)
    {
        std::cout << e.what() << "\n";
        std::cout << "Press any key to continue...";
        _getch();
        return -1;
    }

    float rectangleWithTextureVertices[] {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
   };
    
    int drawOrderIndices[]{
        0, 1, 3,
        1, 2, 3
    };

    // Rectangle with Texture
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleWithTextureVertices), rectangleWithTextureVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(drawOrderIndices), drawOrderIndices, GL_STATIC_DRAW);
    // vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr); // This Attribute pointer is located at 0, and contains the vertices coordinates in Normalized space.
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));// This attribute pointer is located at 1, and contains the color data that will be passed to the fragment shader
    glEnableVertexAttribArray(1);
    // texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // Get texture information using external library stb_image
    int width;
    int height;
    int nrChannels; // number of color channels
    unsigned char *data = stbi_load("Textures/container.jpg", &width, &height, &nrChannels, 0);

    // Setting up texture for binding
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // Be sure to bind the texture first before adding any parameters to it.
    // Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_TEXTURE_WRAP_S tells OpenGL to wrap the image at the "s" axis (equivalent to "x" axis). same of GL_TEXTURE_WRAP_T
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture magnifying/minifying parameters with mipmap and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << "\n";
        return -1; // Exit the program, or have backup image in case this cannot be found. Have the image source set by the user at some point
    }
    stbi_image_free(data); // Remove the texture data from memory. Good practice
    
    //--- RENDER LOOP ---
    // Checks if the window is instructed to close
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        // This is using states. first line sets the state, second line clears the window with the state that was set in the first. States are the name of the game
        glClearColor(0.2f, 0.3f,0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // TriColor triangle
        // triColorShader.use();
        // glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        //Rectangle with textures
        // glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        rectangleWithTextureShader.use();
        // rectangleWithTextureShader.setInt("ourTexture", 0);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0); // Unbind just in case
        
        // There is a back and front buffer for displaying images. The front buffer is what is currently drawn, and the back buffer is painting the next image on the screen.
        // When the back buffer is finished drawing, the buffers are switched so that the back buffer is the front and vice versa.
        glfwSwapBuffers(window); 
        glfwPollEvents(); // Checks if any events are triggered like keyboard events or mouse movement. We can register callback to respond to these events
    }

    glDeleteVertexArrays(1, &VAO); // reference not needed because it is an array, which is already a pointer
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glDeleteProgram(triColorShader.program_id);
    glDeleteProgram(rectangleWithTextureShader.program_id);

    glfwTerminate(); // Cleans up any resources and closes the window
    
    return 0;
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0, width, height);
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void initializeGLFWConfig()
{
    glfwInit();
    // Tells GLFW that we want to use version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // We should use the Core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

// TUTORIAL
    // Configure the vertex buffers
    //SINGLE TRIANGLE VETICES
    // float vertices[] =
    // {
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     0.0f, 0.5f, 0.0f
    // };
    //
    // // SQUARE VERTICES WITH ELEMENT BUFFER
    // float squareVertices[]
    // {
    //     0.5f, 0.5f, 0.0f, // top right
    //     0.5f, -0.5f, 0.0f, // bottom right
    //     -0.5f, -0.5f, 0.0f, // bottom left
    //     -0.5f, 0.5f, 0.0f // top left
    // };
    // unsigned int drawSquareIndeces[] =
    // {
    //     0, 1, 3,
    //     1, 2, 3
    // };
    
    // unsigned int VAO; 
    // unsigned int VBO;
    // unsigned int EBO;
    // glGenVertexArrays(1, &VAO); 
    // glGenBuffers(1, &EBO); // SQUARE
    // glGenBuffers(1, &VBO); // SINGLE TRIANGLE
    
    // Bind the Vertex Array Object
    // glBindVertexArray(VAO);

    // SINGLE TRIANGLE DATA
    // Copy vertices array to a buffer
    // glBindBuffer(GL_ARRAY_BUFFER, VBO); // Binds the generated to the GL buffer
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Applies the data to the GL buffer

    // SQUARE DATA
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
    //
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(drawSquareIndeces), drawSquareIndeces, GL_STATIC_DRAW);
    
    // // Set the vertex attributes pointers
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
    // glEnableVertexAttribArray(0);
    //
    // // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    //  glBindBuffer(GL_ARRAY_BUFFER,0);
    //
    // glBindVertexArray(0); // Generally this is not needed unless we need to bind another VAO if needed
    //
    // // to draw a wireframe, set to polygon mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);