#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
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
int N = 9;
float vertices[100000];
const float radius = 30.0f;
float camX = radius;
float camZ = radius;

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

unsigned long long factorial(int t)
{
  int res = 1, i;
  for (i = 2; i <= t; i++)
    res *= i;
  return res;
}
float get(float t, float xp[])
{
  int k = 0;
  float result = 0;
  while (k < 10)
  {
    float xi = xp[k];
    result += (factorial(N) / (factorial(k) * factorial(N - k))) * pow(1 - t, N - k) * pow(t, k) * xi;
    //printf("%llu\n",(factorial(N) / factorial(k) * factorial(N - k)));
    k++;
  }
  return result;
}
void base_lateral_cover(int *i, float base[], float z_sub, float z_sup)
{
  printf("%d\n", (*i));
  vertices[((*i))++] = base[3];
  vertices[(*i)++] = base[4];
  vertices[(*i)++] = z_sub; //c
  vertices[(*i)++] = base[6];
  vertices[(*i)++] = base[7];
  vertices[(*i)++] = z_sub; //d
  vertices[(*i)++] = base[6];
  vertices[(*i)++] = base[7];
  vertices[(*i)++] = z_sup; //d
  vertices[(*i)++] = base[3];
  vertices[(*i)++] = base[4];
  vertices[(*i)++] = z_sup; //c
  vertices[(*i)++] = base[6];
  vertices[(*i)++] = base[7];
  vertices[(*i)++] = z_sup; //d
  vertices[(*i)++] = base[3];
  vertices[(*i)++] = base[4];
  vertices[(*i)++] = z_sub; //c

  vertices[(*i)++] = base[90];
  vertices[(*i)++] = base[91];
  vertices[(*i)++] = z_sub; //n
  vertices[(*i)++] = base[93];
  vertices[(*i)++] = base[94];
  vertices[(*i)++] = z_sub; //o
  vertices[(*i)++] = base[93];
  vertices[(*i)++] = base[94];
  vertices[(*i)++] = z_sup; //o
  vertices[(*i)++] = base[90];
  vertices[(*i)++] = base[91];
  vertices[(*i)++] = z_sup; //n
  vertices[(*i)++] = base[93];
  vertices[(*i)++] = base[94];
  vertices[(*i)++] = z_sup; //o
  vertices[(*i)++] = base[90];
  vertices[(*i)++] = base[91];
  vertices[(*i)++] = z_sub; //n

  vertices[(*i)++] = base[213];
  vertices[(*i)++] = base[214];
  vertices[(*i)++] = z_sub; //e1
  vertices[(*i)++] = base[216];
  vertices[(*i)++] = base[217];
  vertices[(*i)++] = z_sub; //f1
  vertices[(*i)++] = base[216];
  vertices[(*i)++] = base[217];
  vertices[(*i)++] = z_sup; //f1
  vertices[(*i)++] = base[213];
  vertices[(*i)++] = base[214];
  vertices[(*i)++] = z_sup; //f1
  vertices[(*i)++] = base[216];
  vertices[(*i)++] = base[217];
  vertices[(*i)++] = z_sup; //e1
  vertices[(*i)++] = base[213];
  vertices[(*i)++] = base[214];
  vertices[(*i)++] = z_sub; //f1

  vertices[(*i)++] = base[108];
  vertices[(*i)++] = base[109];
  vertices[(*i)++] = z_sub; //e1
  vertices[(*i)++] = base[111];
  vertices[(*i)++] = base[112];
  vertices[(*i)++] = z_sub; //f1
  vertices[(*i)++] = base[111];
  vertices[(*i)++] = base[112];
  vertices[(*i)++] = z_sup; //f1
  vertices[(*i)++] = base[108];
  vertices[(*i)++] = base[109];
  vertices[(*i)++] = z_sup; //f1
  vertices[(*i)++] = base[111];
  vertices[(*i)++] = base[112];
  vertices[(*i)++] = z_sup; //e1
  vertices[(*i)++] = base[108];
  vertices[(*i)++] = base[109];
  vertices[(*i)++] = z_sub; //f1

  vertices[(*i)++] = base[114];
  vertices[(*i)++] = base[115];
  vertices[(*i)++] = z_sub; //e1
  vertices[(*i)++] = base[111];
  vertices[(*i)++] = base[112];
  vertices[(*i)++] = z_sub; //f1
  vertices[(*i)++] = base[111];
  vertices[(*i)++] = base[112];
  vertices[(*i)++] = z_sup; //f1
  vertices[(*i)++] = base[114];
  vertices[(*i)++] = base[115];
  vertices[(*i)++] = z_sup; //f1
  vertices[(*i)++] = base[111];
  vertices[(*i)++] = base[112];
  vertices[(*i)++] = z_sup; //e1
  vertices[(*i)++] = base[114];
  vertices[(*i)++] = base[115];
  vertices[(*i)++] = z_sub; //f1
  printf("%d\n", (*i));
}

void set_colors(float colors[], float vertices[], int size)
{
  float max = 1, min = 0.0;
  float color = ((max - min) * ((float)rand() / RAND_MAX)) + min;
  for (int k = 0; k < size; k++)
  {
    if (k % 3 == 1)
      vertices[k] -= 0.03;
    if (k % 3 == 0)
    {
      color = ((max - min) * ((float)rand() / RAND_MAX)) + min;
    }
    if (k % 3 == 1)
    {
      color = ((max - min) * ((float)rand() / RAND_MAX)) + min;
    }
    if (k % 3 == 2)
    {
      color = ((max - min) * ((float)rand() / RAND_MAX)) + min;
    }
    colors[k] = color;
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
  int i = 0;
  float xe[] = {0.1f, 1.1f, 0.5f, 1.35f, 0.2f, 0.004f, -0.59f, -0.652f, -1.304f, -0.1f};
  float ye[] = {-0.7f, -0.6f, 0.0f, 1.7f, 0.4f, 1.5f, 0.5f, 1.5f, -0.9f, -0.7f};
  float xi[] = {0.1, 0.9, 0.3, 0.9, 0.2, 0, -0.12, -0.652, -0.904, -0.1};
  float yi[] = {-0.7, -0.4, 0, 1.47, 0.2, 1.47, 0.5, 1.42, -0.7, -0.7};
  float triangle_scale = 0.005;
  float z_sub = -0.15, z_sup = 0.15;
  float base[] = {
      -0.8664, -0.8275, z_sub, //e
      -0.9235, -0.6959, z_sub, //c
      -0.9619, -0.8317, z_sub, //d
      -0.8, -0.7, z_sub,       //f
      -0.8664, -0.8275, z_sub, //e
      -0.9235, -0.6959, z_sub, //c
      -0.7637, -0.8239, z_sub, //g
      -0.8, -0.7, z_sub,       //f
      -0.8664, -0.8275, z_sub, //e
      -0.6952, -0.7079, z_sub, //h
      -0.7637, -0.8239, z_sub, //g
      -0.8, -0.7, z_sub,       //f
      -0.6441, -0.8251, z_sub, //i
      -0.6952, -0.7079, z_sub, //h
      -0.7637, -0.8239, z_sub, //g
      -0.584, -0.7145, z_sub,  //j
      -0.6441, -0.8251, z_sub, //i
      -0.6952, -0.7079, z_sub, //h
      -0.5348, -0.8287, z_sub, //k
      -0.584, -0.7145, z_sub,  //j
      -0.6441, -0.8251, z_sub, //i
      -0.4873, -0.7229, z_sub, //l
      -0.5348, -0.8287, z_sub, //k
      -0.584, -0.7145, z_sub,  //j
      -0.3714, -0.8347, z_sub, //m
      -0.4873, -0.7229, z_sub, //l
      -0.5348, -0.8287, z_sub, //k
      -0.1499, -0.7177, z_sub, //n
      -0.4873, -0.7229, z_sub, //l
      -0.3714, -0.8347, z_sub, //m
      -0.1873, -0.8473, z_sub, //o
      -0.1499, -0.7177, z_sub, //n
      -0.3714, -0.8347, z_sub, //m
      -0.4873, -0.7229, z_sub, //l
      -0.1499, -0.7177, z_sub, //n
      -0.4699, -0.6651, z_sub, //p
      0.9054, -0.7078, z_sub,  //q
      0.9268, -0.7278, z_sub,  //R
      0.8687, -0.8475, z_sub,  //S
      0.8687, -0.8475, z_sub,  //S
      0.9054, -0.7078, z_sub,  //q
      0.7393, -0.8392, z_sub,  //t
      0.7545, -0.7119, z_sub,  //u
      0.7393, -0.8392, z_sub,  //t
      0.9054, -0.7078, z_sub,  //q
      0.7545, -0.7119, z_sub,  //u
      0.7393, -0.8392, z_sub,  //t
      0.9054, -0.7078, z_sub,  //q
      0.7393, -0.8392, z_sub,  //t
      0.7545, -0.7119, z_sub,  //u
      0.6078, -0.8344, z_sub,  //v
      0.6058, -0.7174, z_sub,  //w
      0.6078, -0.8344, z_sub,  //v
      0.7545, -0.7119, z_sub,  //u
      0.6078, -0.8344, z_sub,  //v
      0.6058, -0.7174, z_sub,  //w
      0.4826, -0.8344, z_sub,  //z
      0.4674, -0.7216, z_sub,  //a1
      0.4826, -0.8344, z_sub,  //z
      0.6058, -0.7174, z_sub,  //w
      0.4826, -0.8344, z_sub,  //z
      0.4674, -0.7216, z_sub,  //a1
      0.3608, -0.833, z_sub,   //b1
      0.3276, -0.7278, z_sub,  //c1
      0.3608, -0.833, z_sub,   //b1
      0.4674, -0.7216, z_sub,  //a1
      0.3608, -0.833, z_sub,   //b1
      0.3276, -0.7278, z_sub,  //c1
      0.2169, -0.8372, z_sub,  //d1
      0.3276, -0.7278, z_sub,  //c1
      0.2169, -0.8372, z_sub,  //d1
      0.0795, -0.7168, z_sub,  //e1
      0.0616, -0.8448, z_sub,  //f1
      0.0795, -0.7168, z_sub,  //e1
      0.2169, -0.8372, z_sub,  //d1
      0.3276, -0.7278, z_sub,  //c1
      0.0795, -0.7168, z_sub,  //e1
      0.3262, -0.6681, z_sub,  //g1
  };
  int j = 0;
  long unsigned int size_base = (sizeof(base) / sizeof(float));
  //printf("%d\n", (int)size_base);
  bool f_def = false;
  int interval = 9;

  for (int j = 0; j < (int)size_base; j += interval)
  {
    //printf("%lf\n", base[j]);
    if (!f_def)
    {
      base[j + 1] += 0.02;
      base[j] += 0.018;
      base[j + 4] += 0.02;
      base[j + 3] += 0.018;
      base[j + 7] += 0.02;
      base[j + 6] += 0.018;
      vertices[i++] = base[j];
      vertices[i++] = base[j + 1];
      vertices[i++] = base[j + 2];

      vertices[i++] = base[j + 3];
      vertices[i++] = base[j + 4]; //1 trinagle
      vertices[i++] = base[j + 5];

      vertices[i++] = base[j + 6];
      vertices[i++] = base[j + 7];
      vertices[i++] = base[j + 8];
      //////////////////////////////////////

      vertices[i++] = base[j];
      vertices[i++] = base[j + 1];
      vertices[i++] = z_sub;

      vertices[i++] = base[j + 6];
      vertices[i++] = base[j + 7];
      vertices[i++] = z_sub;

      vertices[i++] = base[j];
      vertices[i++] = base[j + 1];
      vertices[i++] = z_sup;
      /////////////////////////////////////

      vertices[i++] = base[j];
      vertices[i++] = base[j + 1];
      vertices[i++] = z_sup;

      vertices[i++] = base[j + 6];
      vertices[i++] = base[j + 7];
      vertices[i++] = z_sup;

      vertices[i++] = base[j + 6];
      vertices[i++] = base[j + 7];
      vertices[i++] = z_sub;
      ////////////////////////////////////

      //printf("%d\n", j);
    }
    else
    {
      vertices[i++] = base[j];
      vertices[i++] = base[j + 1];
      vertices[i++] = z_sup;
      //printf("%d\n", j);
    }
    if (j == ((int)size_base) - interval && !f_def)
    {
      f_def = true;
      //printf("%d\n", j);
      j = -3;
      interval = 3;
    }
  }
  printf("%d\n", i);
  base_lateral_cover(&i, base, z_sub, z_sup);
  printf("%d\n", i);
  for (float t = 0; t <= 0.991; t += triangle_scale)
  {
    vertices[i++] = get(t, xe);
    vertices[i++] = get(t, ye);
    vertices[i++] = z_sub;
    vertices[i++] = get(t, xi);
    vertices[i++] = get(t, yi); ///1 triangle back_face
    vertices[i++] = z_sub;
    vertices[i++] = get(t + triangle_scale, xe);
    vertices[i++] = get(t + triangle_scale, ye);
    vertices[i++] = z_sub;

    vertices[i++] = get(t + triangle_scale, xe);
    vertices[i++] = get(t + triangle_scale, ye);
    vertices[i++] = z_sub;
    vertices[i++] = get(t, xi);
    vertices[i++] = get(t, yi); //2 triangle back_face
    vertices[i++] = z_sub;
    vertices[i++] = get(t + triangle_scale, xi);
    vertices[i++] = get(t + triangle_scale, yi);
    vertices[i++] = z_sub;

    vertices[i++] = get(t, xe);
    vertices[i++] = get(t, ye);
    vertices[i++] = z_sup;
    vertices[i++] = get(t, xi);
    vertices[i++] = get(t, yi); ///3 triangle front_face
    vertices[i++] = z_sup;
    vertices[i++] = get(t + triangle_scale, xe);
    vertices[i++] = get(t + triangle_scale, ye);
    vertices[i++] = z_sup;

    vertices[i++] = get(t + triangle_scale, xe);
    vertices[i++] = get(t + triangle_scale, ye);
    vertices[i++] = z_sup;
    vertices[i++] = get(t, xi);
    vertices[i++] = get(t, yi); //4 square front_face
    vertices[i++] = z_sup;
    vertices[i++] = get(t + triangle_scale, xi);
    vertices[i++] = get(t + triangle_scale, yi);
    vertices[i++] = z_sup;

    vertices[i++] = get(t, xe);
    vertices[i++] = get(t, ye);
    vertices[i++] = z_sub;
    vertices[i++] = get(t, xe);
    vertices[i++] = get(t, ye); ///1 triangle lateral_face
    vertices[i++] = z_sup;
    vertices[i++] = get(t + triangle_scale, xe);
    vertices[i++] = get(t + triangle_scale, ye);
    vertices[i++] = z_sub;

    vertices[i++] = get(t + triangle_scale, xe);
    vertices[i++] = get(t + triangle_scale, ye);
    vertices[i++] = z_sub;
    vertices[i++] = get(t, xe);
    vertices[i++] = get(t, ye); //2 triangle lateral_face
    vertices[i++] = z_sup;
    vertices[i++] = get(t + triangle_scale, xe);
    vertices[i++] = get(t + triangle_scale, ye);
    vertices[i++] = z_sup;

    vertices[i++] = get(t, xi);
    vertices[i++] = get(t, yi);
    vertices[i++] = z_sub;
    vertices[i++] = get(t, xi);
    vertices[i++] = get(t, yi); ///1 triangle lateral_face
    vertices[i++] = z_sup;
    vertices[i++] = get(t + triangle_scale, xi);
    vertices[i++] = get(t + triangle_scale, yi);
    vertices[i++] = z_sub;

    vertices[i++] = get(t + triangle_scale, xi);
    vertices[i++] = get(t + triangle_scale, yi);
    vertices[i++] = z_sub;
    vertices[i++] = get(t, xi);
    vertices[i++] = get(t, yi); //2 triangle lateral_face
    vertices[i++] = z_sup;
    vertices[i++] = get(t + triangle_scale, xi);
    vertices[i++] = get(t + triangle_scale, yi);
    vertices[i++] = z_sup;
  }

  max = i / 3;
  set_colors(colors, vertices, i);
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
  glm::mat4 Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 100.0f);
  //glm::perspective(glm::radians(30.0f), 4.0f / 3.0f, 0.1f, 100.0f);
  // View camera matrix
  view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
  // Model matrix : an identity matrix (model will be at the origin)
  Model = glm::mat4();
  Model = glm::rotate(Model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
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

/* process all input: query GLFW whether relevant keys are pressed/released 
this frame and react accordingly 
-----------------------------------------------------------------------*/

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS || cam == 1)
  {
    view = glm::lookAt(glm::vec3(-1 , 0, 0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
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
    view = glm::lookAt(glm::vec3(0.0, - 1, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
    cam = 6;
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
