#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "points.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
glm::mat4 view;
glm::mat4 Model;
const int SCR_WIDTH = 900;
const int SCR_HEIGHT = 600;
bool colored = false;
int max = 100000;
int cam = 0;
int letter=0;
int point = 0;
float vertices[100000];
const float radius = 30.0f;
float camX = radius;
float camZ = radius;

struct Point{
  int x;
  int y;
  int z;
};


struct Point *positions(){
  struct Point *positions=(struct Point *)malloc(6*sizeof(struct Point));
  float n[]={-6.5,-3.5,-1.5,1.5,6.5,3.5};
  for(int i=0;i<6;i++){
    struct Point p;p.x=n[i];
    positions[i]=p;
  }
  return positions;
}

float colors[] = {0.2f, 0.3f, 0.3f};
// Input vertex data, different for all executions of this shader.
// Output data color, will be interpolated for each fragment.
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 vertexColor;\n"
                                 "out vec3 fragmentColor;\n"
                                 "uniform mat4 MVP;\n"
                                 "void main()\n"
                                 "{\n"
                                 " gl_Position = MVP * vec4(aPos, 1.0);\n"
                                 " fragmentColor = vertexColor;\n"
                                 "}\0";

// declare and define fshader, position in color vector declaration
// are RGBA from [0,1] simply in and out
const char *fragmentShaderSource = "#version 330 core\n"
                                   "in vec3 fragmentColor;\n;"
                                   "out vec3 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   " FragColor = fragmentColor;\n"
                                   "}\n\0";
void add_letters(float *t,struct Point position,int max)
{
  int i = 0;
  while (i < max)
  {
    vertices[point++] = t[i++]+position.x;
    vertices[point++] = t[i++];
    vertices[point++] = t[i++];
  }
}

int main()
{

  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 8);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "p3", NULL,
                                        NULL);

  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive
  // vertices give a triangle. A cube has 6 faces with 2 triangles each, so this
  // makes 6*2=12 triangles, and 12*3 vertices
  // ------------------------------------------------------------------
  float colors[100000];
  struct Point *position=positions();
  float *omega = letter_omega();
  max = size_omega;
  add_letters(omega,position[0],max);

  float *g = letra_goncalo();
  max = size_g;
  add_letters(g,position[1],max);

  //float *psi = letra_psy();

  float *p = letra_P();
  max = size_p;
  add_letters(p,position[2],max);

  float *r = letra_rafa();
  max = size_r;
  add_letters(r,position[3],max);

  float *c = letra_bruno();
  max = size_c;
  add_letters(c,position[4],max);
  printf("%d\n",point);
  max = point / 3;






  //set_colors(colors, vertices, i);
  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  //max = 36 / 3;
  //printf("%d\n",max);
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  //------------------------------------------------------------
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ARRAY_BUFFER, EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(
      0,        // attribute. No particular reason for 0, but
                // must match the layout in the shader.
      3,        // size
      GL_FLOAT, // type
      GL_FALSE, // normalized?
      0,        // stride
      (void *)0 // array buffer offset
  );

  // 2nd attribute buffer : colors
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, EBO);
  glVertexAttribPointer(
      1,        // attribute. No particular reason for 1, but
                // must match the layout in the shader.
      3,        // size
      GL_FLOAT, // type
      GL_FALSE, // normalized?
      0,        // stride
      (void *)0 // array buffer offset
  );

  /* note that this is allowed, the call to glVertexAttribPointer 
       registered VBO as the vertex attribute's bound vertex buffer 
       object so afterwards we can safely unbind*/
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  /* You can unbind the VAO afterwards so other VAO calls won't 
       accidentally modify this VAO, but this rarely happens. Modifying 
       other VAOs requires a call to glBindVertexArray anyways so we 
       generally don't unbind VAOs (nor VBOs) when it's not directly 
       necessary.*/
  glBindVertexArray(0);

  // uncomment this call to draw in wireframe polygons.
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // build and compile our shader program
  // ------------------------------------
  // vertex shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // check for vertex shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  // fragment shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // check for fragment shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // link shaders : puts together the vertex and the fragment shaders
  // into a "shader program object"
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // check for shader linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }
  // delete shaders, we don't need them anymore
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  // Get a handle for our "MVP" uniform
  //unsigned int MatrixID = glGetUniformLocation(shaderProgram, "MVP");
  // create transformations

  // Projection matrix : 45� Field of View, 4:3 ratio,
  // display range : 0.1 unit <-> 100 units
  glm::mat4 Projection = glm::ortho(-7.0f, 7.0f, -7.0f, 7.0f, 0.0f, 100.0f);
  //glm::perspective(glm::radians(30.0f), 4.0f / 3.0f, 0.1f, 100.0f);
  // View camera matrix
  view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
  // Model matrix : an identity matrix (model will be at the origin)
  Model = glm::mat4();
  Model = glm::rotate(Model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  // Our ModelViewProjection : multiplication of our 3 matrices
  // Remember, matrix multiplication is the other way around
  glm::mat4 MVP = Projection * view * Model;

  // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive
  // vertices give a triangle. A cube has 6 faces with 2 triangles each, so this
  // makes 6*2=12 triangles, and 12*3 vertices
  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // input
    // -----
    unsigned int MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    processInput(window);
    glm::mat4 MVP = Projection * view * Model;
    // render
    // ------
    //glClearColor(0.5f, 0.5f, 0.5f, 1.0f); //grey rendering
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //green? rendering
    glClear(GL_COLOR_BUFFER_BIT);

    // draw our first triangle: using shader program
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // update the uniform color
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    //glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);
    // Dark blue background
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // also clear the depth buffer now!
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // seeing as we only have a single VAO there's no need to bind
    // it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, max);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // glBindVertexArray(0); // no need to unbind it every time
    /* glfw: swap buffers and poll IO events (keys pressed/released, 
	 mouse moved etc.)
	 --------------------------------------------------------------*/
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);
  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
  {
    Model = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(0, 1, 0)); //sentido horario sobre o eixo X
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
  {
    Model = glm::rotate(glm::mat4(1.0f), glm::radians(-10.0f), glm::vec3(0, 1, 0)); //sentido anti-horario sobre o eixo X
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
  {
    Model = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(0, 1, 0)); //sentido para a direita
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
  {
    Model = glm::rotate(glm::mat4(1.0f), glm::radians(-10.0f), glm::vec3(0, 1, 0)); //sentido para a esquerda
  }

  //cams
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS || cam == 1)
  {
    view = glm::lookAt(glm::vec3(-1, 0, 0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    cam = 1;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || cam == 2)
  {
    view = glm::lookAt(glm::vec3(1, 0, 0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 0.0));
    cam = 2;
  }
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS || cam == 3)
  {
    view = glm::lookAt(glm::vec3(0.0, 0.0, 1), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    cam = 3;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || cam == 4)
  {
    view = glm::lookAt(glm::vec3(0.0, 0.0, -1), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    cam = 4;
  }
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS || cam == 5)
  {
    view = glm::lookAt(glm::vec3(0.0, 1, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
    cam = 5;
  }
  if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS || cam == 6)
  {
    view = glm::lookAt(glm::vec3(0.0, -1, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
    cam = 6;
  }

  //select letter
  if(glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS || letter==1){

  }
}

/* glfw: whenever the window size changed (by OS or user resize) this
   callback function executes
   -------------------------------------------------------------------*/
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that
  // width and height will be significantly larger than specified on
  // retina displays.
  glViewport(0, 0, width, height);
}