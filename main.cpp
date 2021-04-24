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
void resize(int s);
void reset();
void positions(struct Letter *letter, float *positions_x, float positions_y, int size);
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void getNormalizedCoords();

glm::mat4 view, Projection, MVP, Model = glm::mat4(1.0f);
const int SCR_WIDTH = 900;
const int SCR_HEIGHT = 600;
bool colored = false;
int max = 100000;
int cam = 0;
int letter = -1;
int values = 0;
int point_color = 0;
float all_letters_coordinates[100000];
float color[100000];
float radius = 10.0f;
float camX = radius;
float camZ = radius;
double xpos, ypos;
float xpos1, ypos1;
bool lbutton_down;
const float selection[] = {
    0.5f,
    -4.2f,
    0.0f,
    -0.5f,
    -5.7f,
    0.0f,
    0.5f,
    -5.7f,
    0.0f,
    -0.5f,
    -4.2f,
    0.0f,
    -0.5f,
    -5.7f,
    0.0f,
    0.5f,
    -4.2f,
    0.0f,
    0.0f,
    -4.0f,
    0.0f,
    0.2f,
    -4.2f,
    0.0f,
    -0.2f,
    -4.2f,
    0.0f,
};

struct Letter
{
  struct Point *letter;
  Point translation;
  int n_points;
};
struct Object
{
  glm::mat4 MVP;
  glm::mat4 Model, view;
  struct Letter *letter;
  int all_points;
};

struct Object all_letters;
struct Object highlight,clone_highlight;
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

const char *fragmentShaderSource = "#version 330 core\n"
                                   "in vec3 fragmentColor;\n;"
                                   "out vec3 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   " FragColor = fragmentColor;\n"
                                   "}\n\0";

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 8);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
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
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  all_letters.letter = (struct Letter *)malloc(6 * sizeof(struct Letter));
  highlight.letter = (struct Letter *)malloc(sizeof(struct Letter));
  float n[] = {-5.5, -3.2, -0.5, 1.5, 3.6, 5.5};
  positions(all_letters.letter, n, 0, 6);
  highlight.letter[0].n_points = 0;
  all_letters.letter[0].letter = letra_psy();
  all_letters.letter[0].n_points = size_psi / 3;
  all_letters.letter[1].letter = letra_omega();
  all_letters.letter[1].n_points = size_omega / 3;
  all_letters.letter[2].letter = letra_bruno();
  all_letters.letter[2].n_points = size_c / 3;
  all_letters.letter[3].letter = letra_rafa();
  all_letters.letter[3].n_points = size_r / 3;
  all_letters.letter[4].letter = letra_P();
  all_letters.letter[4].n_points = size_p / 3;
  all_letters.letter[5].letter = letra_goncalo();
  all_letters.letter[5].n_points = size_g / 3;
  reset();

  //printf("total:%d\n", values);
  max = values / 3;
  //printf("%d\n", values == all_letters.all_points);

  unsigned int VAO_all_letters, VAO_single_letter;
  unsigned int VBO_all_letters, VBO_single_letter;
  unsigned int EBO;

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  Projection = glm::ortho(-7.0f, 7.0f, -7.0f, 7.0f, -100.0f, 100.0f);
  view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
  Model = glm::rotate(Model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  all_letters.MVP = Projection * view * Model;
  all_letters.Model = Model;
  all_letters.view = view;
  MVP = Projection * view * Model;
  clone_highlight=highlight;

  glfwSetMouseButtonCallback(window, mouseButtonCallback);
  while (!glfwWindowShouldClose(window))
  {
    processInput(window);
    highlight.MVP = Projection * highlight.view * highlight.Model;
    //_----------------------------
    //----------------------
    all_letters.MVP = Projection * all_letters.view * all_letters.Model;

    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO_all_letters);
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glGenVertexArrays(1, &VAO_all_letters);
    glBindVertexArray(VAO_all_letters);

    glGenBuffers(1, &VBO_all_letters);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_all_letters);
    glBufferData(GL_ARRAY_BUFFER, values * sizeof(float), all_letters_coordinates, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO_single_letter);
    glBindVertexArray(VAO_single_letter);

    glGenBuffers(1, &VBO_single_letter);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_single_letter);
    glBufferData(GL_ARRAY_BUFFER, highlight.all_points * 3 * sizeof(float), highlight.letter[0].letter, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

    unsigned int MatrixID = glGetUniformLocation(shaderProgram, "MVP");

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &all_letters.MVP[0][0]); //////// matrix com as letras todas
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_all_letters);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glDrawArrays(GL_TRIANGLES, 0, all_letters.all_points);

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &highlight.MVP[0][0]); // matrix com a letra escolhida
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_single_letter);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glDrawArrays(GL_TRIANGLES, 0, highlight.all_points);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, EBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glfwSwapBuffers(window);
    glDeleteVertexArrays(1, &VAO_all_letters);
    glDeleteBuffers(1, &VBO_all_letters);
    glDeleteVertexArrays(1, &VAO_single_letter);
    glDeleteBuffers(1, &VBO_single_letter);
    glfwPollEvents();
    //printf("velo:%f\n",radius);
  }
  glDeleteProgram(shaderProgram);
  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
  {
    highlight.Model = glm::rotate(highlight.Model, glm::radians(radius), glm::vec3(1, 0, 0));
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
  {
    highlight.Model = glm::rotate(highlight.Model, glm::radians(-radius), glm::vec3(1, 0, 0));
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
  {
    highlight.Model = glm::rotate(highlight.Model, glm::radians(-radius), glm::vec3(0, 1, 0));
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
  {
    highlight.Model = glm::rotate(highlight.Model, glm::radians(radius), glm::vec3(0, 1, 0));
  }

  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS || cam == 1)
  {
    highlight.view = glm::lookAt(glm::vec3(-1, 0, 0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    all_letters.view = glm::lookAt(glm::vec3(-1, 0, 0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    cam = 1;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || cam == 2)
  {
    highlight.view = glm::lookAt(glm::vec3(1, 0, 0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 0.0));
    all_letters.view = glm::lookAt(glm::vec3(1, 0, 0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 0.0));
    cam = 2;
  }
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS || cam == 3)
  {
    highlight.view = glm::lookAt(glm::vec3(0.0, 0.0, 1), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    all_letters.view = glm::lookAt(glm::vec3(0.0, 0.0, 1), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    cam = 3;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    radius = radius * 0.3f;
  }
  if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
  {
    if (radius > 0)
      radius = radius * (-0.3f);
  }
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS || cam == 5)
  {
    highlight.view = glm::lookAt(glm::vec3(0.0, 1, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
    all_letters.view = glm::lookAt(glm::vec3(0.0, 1, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
    cam = 5;
  }
  if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS || cam == 6)
  {
    highlight.view = glm::lookAt(glm::vec3(0.0, -1, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
    all_letters.view = glm::lookAt(glm::vec3(0.0, -1, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
    cam = 6;
  }

  if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
  {
    letter = 0;
    resize(0);
  }
  if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
  {
    letter = 1;
    resize(1);
  }
  if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
  {
    letter = 2;
    resize(2);
  }
  if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
  {
    letter = 3;
    resize(3);
  }
  if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
  {
    letter = 4;
    resize(4);
  }
  if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)
  {
    letter = 5;
    resize(5);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    letter = -1;
    reset();
  }
  if (lbutton_down)
  {
    glfwGetCursorPos(window, &xpos, &ypos);
    getNormalizedCoords();
    highlight.Model = glm::translate(clone_highlight.Model,glm::vec3(xpos1,ypos1,5));
    highlight.Model = glm::scale(highlight.Model, glm::vec3(2, 2, 2));
  }

  glm::mat4 S = glm::mat4(1.0f);
  S = glm::scale(S, glm::vec3(0.9f, 0.9f, 0.9f));

  if(glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    if(MVP[0][0] <= 1.73205) // MAX VALUE
      MVP = MVP * inverse(S);
    }

  if(glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    if(MVP[0][0] >= 0.0270633) // MIN VALUE
      MVP = MVP * S;
  }
}

/* glfw: whenever the window size changed (by OS or user resize) this
   callback function executes
   -------------------------------------------------------------------*/
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{

  glViewport(0, 0, width, height);
}

void resize(int s)
{
  if (all_letters.Model == Model)
  {
    all_letters.Model = glm::scale(all_letters.Model, glm::vec3(0.7, 0.7, 0.7));
    all_letters.Model = glm::translate(all_letters.Model, glm::vec3(0.0, -5, 0.0));
  }
  highlight.letter[0] = all_letters.letter[s];
  highlight.letter[0].n_points = all_letters.letter[s].n_points;
  highlight.all_points = highlight.letter[0].n_points;
  highlight.MVP = MVP;
  highlight.Model = Model;
  highlight.view = view;
  highlight.Model = glm::scale(highlight.Model, glm::vec3(2, 2, 2));
  highlight.Model = glm::translate(highlight.Model, glm::vec3(0, 1, 0));
}
void reset()
{
  highlight.all_points = 0;
  all_letters.Model = Model;
  all_letters.Model = glm::translate(all_letters.Model, glm::vec3(0.0, 0, 0.0));
  values = 0;
  for (int i = 0; i < 6; i++)
  {
    //printf("%d\n", all_letters.letter[i].n_points);
    for (int j = 0; j < all_letters.letter[i].n_points; j++)
    {
      //printf("v:%f %f %f\n", letters[i].letter[j].x, letters[i].letter[j].y, letters[i].letter[j].z);
      all_letters_coordinates[values++] = all_letters.letter[i].letter[j].x + all_letters.letter[i].translation.x;
      all_letters_coordinates[values++] = all_letters.letter[i].letter[j].y + all_letters.letter[i].translation.y;
      all_letters_coordinates[values++] = all_letters.letter[i].letter[j].z;
    }
    all_letters.all_points += all_letters.letter[i].n_points * 3;
  }
}
void positions(struct Letter *letter, float *positions_x, float positions_y, int size)
{

  for (int i = 0; i < size; i++)
  {
    struct Point p;
    p.x = positions_x[i];
    p.y = positions_y;
    letter[i].translation = p;
  }
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{

  if (button == GLFW_MOUSE_BUTTON_LEFT)
  {
    if (GLFW_PRESS == action)
      lbutton_down = true;
    else if (GLFW_RELEASE == action)
      lbutton_down = false;
  }
}

void getNormalizedCoords()
{
  xpos1 = (-1.0)+((2.0f * xpos) / (SCR_WIDTH));
  ypos1 = (1.0)+((-2.0f * ypos) / (SCR_HEIGHT));
  xpos1=xpos1*7;
  ypos1=ypos1*7;
  printf("%f %f\n",xpos1,ypos1);
}