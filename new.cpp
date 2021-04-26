#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "points.h"
#include "colors.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
glm::mat4 MVP, R, S, T;
float angle = 70.0f;
float t = 1;
const glm::mat4 Projection = glm::perspective(glm::radians(angle), 900.0f / 900.0f, 0.1f, 200.0f);
const glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
const int SCR_WIDTH = 900;
const int SCR_HEIGHT = 600;
bool colored = false;
int letter = -1;
int values = 0;
int point_color = 0;
float all_letters_coordinates[100000];
float color[100000];
bool positioned_default = false;
float radius = 10.0f;
double xpos, ypos;
float xpos1, ypos1;
bool lbutton_down;
glm::mat4 rotates = glm::mat4(1.0f);
glm::mat4 identity = glm::mat4(1.0f);
struct Letter
{
    struct Point *letter;
    int n_points;
    glm::mat4 MVP;
    glm::mat4 inicial_pos;
    bool actions = false;
};

struct Letter *the_letter;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void resize(int s);
void reset();
void positions(struct Letter *letter, float *positions_x, float positions_y, int size);
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void getNormalizedCoords();
void selection(GLFWwindow *window);
void letters_lined();
void rotation(GLFWwindow *window, int selected);
void translation(GLFWwindow *window, int selected);
void zoom(GLFWwindow *window, int selected);

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

    unsigned int VAO[6];
    unsigned int VBO[6];
    unsigned int EBO[6];

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
    //scaling
    S = glm::mat4(1.0f);
    S = glm::scale(S, glm::vec3(0.9f, 0.9f, 0.9f));

    //translation
    T = glm::mat4(1.0f);
    //T = glm::translate(T, glm::vec3(0.0f, 0.0f, 0.0f));

    //rotation
    R = glm::mat4(1.0f);

    letters_lined();
    //reset();
    //reset();
    //reset();

    while (!glfwWindowShouldClose(window))
    {
        unsigned int MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        processInput(window);

        glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (int i = 0; i < 6; i++)
        {
            glGenVertexArrays(1, &VAO[i]);
            glBindVertexArray(VAO[i]);

            glGenBuffers(1, &VBO[i]);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
            // printf("%i %d\n", i, the_letter[i].n_points);
            glBufferData(GL_ARRAY_BUFFER, the_letter[i].n_points * 3 * sizeof(float), convert_to_arr_float(the_letter[i].letter, the_letter[i].n_points), GL_STATIC_DRAW);
        }
        for (int i = 0; i < 6; i++)
        {
            glGenBuffers(1, &EBO[i]);
            glBindBuffer(GL_ARRAY_BUFFER, EBO[i]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
        }

        for (int i = 0; i < 6; i++)
        {
            //printf("ant\n");
            //std::cout << the_letter[i].MVP << std::endl;
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &the_letter[i].MVP[0][0]); //////// matrix com as letras todas
            //printf("dep\n");
            //std::cout << the_letter[i].MVP << std::endl;
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glDrawArrays(GL_TRIANGLES, 0, the_letter[i].n_points);
        }
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glfwSwapBuffers(window);
        glfwPollEvents();
        //printf("velo:%f\n",radius);
    }
    for (int i = 0; i < 6; i++)
    {
        glDeleteVertexArrays(1, &VAO[i]);
        glDeleteBuffers(1, &VBO[i]);
    }
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        reset();
        letter = -1;
    }
    selection(window);
    rotation(window, letter);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && letter != -1)
    {
        if (!the_letter[letter].actions)
        {
            the_letter[letter].actions = true;
            the_letter[letter].MVP = glm::mat4(1);
            T = glm::translate(T, glm::vec3((-5.5) + (letter * 2.2), 0.0f, 0.0f));
            the_letter[letter].MVP = glm::mat4(1.0f) * glm::lookAt(glm::vec3(0, 0, 0.5), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)) * glm::ortho(-7.0f, 7.0f, -7.0f, 7.0f, -100.0f, 100.0f) * T;
        }
        translation(window, letter);
    }
}
void selection(GLFWwindow *window)
{
    float pos = -5.5;
    T = glm::mat4(1.0f);
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
    {
        letter = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
    {
        letter = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
    {
        letter = 2;
    }
    if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
    {
        letter = 3;
    }
    if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
    {
        letter = 4;
    }
    if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)
    {
        letter = 5;
    }
}

void rotation(GLFWwindow *window, int s)
{
    if (!the_letter[s].actions && (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS))
    {
        the_letter[s].actions = true;
        the_letter[letter].MVP = glm::mat4(1);
        T = glm::translate(T, glm::vec3((-5.5) + (letter * 2.2), 0.0f, 0.0f));
        the_letter[letter].MVP = glm::mat4(1.0f) * glm::lookAt(glm::vec3(0, 0, 0.5), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)) * glm::ortho(-7.0f, 7.0f, -7.0f, 7.0f, -100.0f, 100.0f) * T;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        positioned_default = false;
        R = glm::mat4(1.0f);
        R = glm::rotate(R, glm::radians(radius), glm::vec3(1, 0, 0));
        //printf("rotate\n");
        //std::cout << R << std::endl;
        //printf("mvp\n");
        //std::cout << the_letter[s].MVP << std::endl;
        the_letter[s].MVP = the_letter[s].MVP * R;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        positioned_default = false;
        R = glm::mat4(1.0f);
        R = glm::rotate(R, glm::radians(-radius), glm::vec3(1, 0, 0));
        //printf("rotate\n");
        //std::cout << R << std::endl;
        //printf("mvp\n");
        //std::cout << the_letter[s].MVP << std::endl;
        the_letter[s].MVP = the_letter[s].MVP * R;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        positioned_default = false;
        R = glm::mat4(1.0f);
        R = glm::rotate(R, glm::radians(-radius), glm::vec3(0, 1, 0));
        //printf("rotate\n");
        //std::cout << R << std::endl;
        //printf("mvp\n");
        //std::cout << the_letter[s].MVP << std::endl;
        the_letter[s].MVP = the_letter[s].MVP * R;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        positioned_default = false;
        R = glm::mat4(1.0f);
        R = glm::rotate(R, glm::radians(radius), glm::vec3(0, 1, 0));
        //printf("rotate\n");
        //std::cout << R << std::endl;
        //printf("mvp\n");
        //std::cout << the_letter[s].MVP << std::endl;
        the_letter[s].MVP = the_letter[s].MVP * R;
    }
}
void translation(GLFWwindow *window, int s)
{
    positioned_default = false;
    glfwGetCursorPos(window, &xpos, &ypos);
    getNormalizedCoords();
    T = glm::translate(glm::mat4(1), glm::vec3(xpos1, ypos1, 0));
    T = glm::mat4(-1) + T;
    //printf("mvpp\n");
    //std::cout << the_letter[s].MVP[3][0] << std::endl;
    the_letter[s].MVP[3][0] = T[3][0];
    the_letter[s].MVP[3][1] = T[3][1];
    zoom(window, s);
}
void zoom(GLFWwindow *window, int s)
{
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
    {
        positioned_default = false;
        glm::mat4 T1 = glm::mat4(1.0f);
        T1 = glm::translate(T1, glm::vec3(0.0f, 0.0f, 0.1f));
        the_letter[s].MVP = the_letter[s].MVP * S * inverse(T1);
        //printf("s\n");
        //std::cout << S << std::endl;
       // printf("mvp\n");
        //std::cout << the_letter[s].MVP << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
    {
        positioned_default = false;
        glm::mat4 T2 = glm::mat4(1.0f);
        T2 = glm::translate(T2, glm::vec3(0.0f, 0.0f, 0.1f));
        the_letter[s].MVP = the_letter[s].MVP * inverse(S) * T2;
        //printf("s\n");
        //std::cout << inverse(S) << std::endl;
        //printf("mvp\n");
        //std::cout << the_letter[s].MVP << std::endl;
    }
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{

    glViewport(0, 0, width, height);
}
void reset()
{
    for (int i = 0; i < 6; i++)
    {
        float position[16] = {
            the_letter[i].inicial_pos[0][0], the_letter[i].inicial_pos[0][1], the_letter[i].inicial_pos[0][2], the_letter[i].inicial_pos[0][3],
            the_letter[i].inicial_pos[1][0], the_letter[i].inicial_pos[1][1], the_letter[i].inicial_pos[1][2], the_letter[i].inicial_pos[1][3],
            the_letter[i].inicial_pos[2][0], the_letter[i].inicial_pos[2][1], the_letter[i].inicial_pos[2][2], the_letter[i].inicial_pos[2][3],
            the_letter[i].inicial_pos[3][0], the_letter[i].inicial_pos[3][1], the_letter[i].inicial_pos[3][2], the_letter[i].inicial_pos[3][3]};
        the_letter[i].MVP = glm::make_mat4(position);
        the_letter[i].actions = false;
    }
}
void positions()
{
    float pos = -5.5;
    float dist = 2.2;
    for (int i = 0; i < 6; i++)
    {
        T = glm::mat4(1.0f);
        T = glm::translate(T, glm::vec3(pos, 0.0f, 0.0f));
        the_letter[i].MVP = the_letter[i].MVP * T;
        float position[16] = {
            the_letter[i].MVP[0][0], the_letter[i].MVP[0][1], the_letter[i].MVP[0][2], the_letter[i].MVP[0][3],
            the_letter[i].MVP[1][0], the_letter[i].MVP[1][1], the_letter[i].MVP[1][2], the_letter[i].MVP[1][3],
            the_letter[i].MVP[2][0], the_letter[i].MVP[2][1], the_letter[i].MVP[2][2], the_letter[i].MVP[2][3],
            the_letter[i].MVP[3][0], the_letter[i].MVP[3][1], the_letter[i].MVP[3][2], the_letter[i].MVP[3][3]};
        pos += dist;
        the_letter[i].inicial_pos = glm::make_mat4(position);
        //std::cout << the_letter[i].MVP << std::endl;
        //std::cout << the_letter[i].inicial_pos << std::endl;
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
    xpos1 = (-1.0) + ((2.0f * xpos) / (SCR_WIDTH));
    ypos1 = (1.0) + ((-2.0f * ypos) / (SCR_HEIGHT));
    xpos1 = xpos1; //* glm::radians(angle);
    ypos1 = ypos1; //* glm::radians(angle);
    //printf("%f %f\n", xpos1, ypos1);
}

void letters_lined()
{
    the_letter = (struct Letter *)malloc(6 * sizeof(struct Letter));
    the_letter[0].letter = letra_psy();
    the_letter[0].n_points = size_psi / 3;
    the_letter[1].letter = letra_omega();
    the_letter[1].n_points = size_omega / 3;
    the_letter[2].letter = letra_bruno();
    the_letter[2].n_points = size_c / 3;
    the_letter[3].letter = letra_rafa();
    the_letter[3].n_points = size_r / 3;
    the_letter[4].letter = letra_P();
    the_letter[4].n_points = size_p / 3;
    the_letter[5].letter = letra_goncalo();
    the_letter[5].n_points = size_g / 3;
    for (int i = 0; i < 6; i++)
    {
        //std::cout << the_letter[i].MVP << std::endl;
        the_letter[i].MVP = Projection * view * glm::mat4(1);
    }
    positions();
}