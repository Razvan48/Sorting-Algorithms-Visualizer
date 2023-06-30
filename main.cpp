#include <iostream>
#include <vector>
#include <random>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace std;

const double WINDOW_WIDTH = 1024.0;
const double WINDOW_HEIGHT = 1024.0;

const char* vertexShaderSource =
"#version 330 core \n"
"\n"
"layout (location = 0) in vec2 vertexPosition; \n"
"layout (location = 1) in vec3 inColour; \n"
"uniform mat4 ortho; \n"
"out vec3 outColour; \n"
"\n"
"void main() \n"
"{ \n"
"\n"
"   gl_Position = ortho * vec4(vertexPosition.x, vertexPosition.y, 0.0, 1.0); \n"
"   outColour = vec3(inColour.x, inColour.y, inColour.z); \n"
"\n"
"} \n"
"\0";

const char* fragmentShaderSource =
"#version 330 core \n"
"\n"
"in vec3 outColour; \n"
"out vec4 vertexColour; \n"
"\n"
"void main() \n"
"{ \n"
"\n"
"   vertexColour = vec4(outColour.x, outColour.y, outColour.z, 1.0); \n"
"\n"
"} \n"
"\0";

unsigned int VAO;
unsigned int VBO;

double currentTime;
double previousTime;
double deltaTime;

const int NMAX = 50;

long long numbers[NMAX];
bool swappedPosition[NMAX];

bool finalColoring[NMAX];

vector<double> vertices;

vector<int> appliedSwap;

random_device randomDevice;
mt19937 randomNumberGenerator;

GLFWwindow* window;

const double TIME_BETWEEN_FRAMES = 0.001;

void updateDeltaTime()
{
    currentTime = glfwGetTime();
    deltaTime = currentTime - previousTime;
    previousTime = currentTime;
}

void handleInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void drawArray()
{
    vertices.clear();

    double distX = WINDOW_WIDTH / (1.0 * NMAX);

    if (!appliedSwap.empty())
    {
        swap(numbers[appliedSwap[0]], numbers[appliedSwap[1]]);
        appliedSwap.clear();
    }

    for (int i = 0; i < NMAX; ++i)
    {
        double columnHeight = 1.0 * numbers[i] / NMAX * WINDOW_HEIGHT;

        if (finalColoring[i])
        {
            vertices.emplace_back(1.0 * i / NMAX * WINDOW_WIDTH);
            vertices.emplace_back(0.0);
            vertices.emplace_back(0.0);
            vertices.emplace_back(1.0);
            vertices.emplace_back(0.0);

            vertices.emplace_back(1.0 * (i + 1) / NMAX * WINDOW_WIDTH);
            vertices.emplace_back(0.0);
            vertices.emplace_back(0.0);
            vertices.emplace_back(1.0);
            vertices.emplace_back(0.0);

            vertices.emplace_back(1.0 * (i + 1) / NMAX * WINDOW_WIDTH);
            vertices.emplace_back(columnHeight);
            vertices.emplace_back(0.0);
            vertices.emplace_back(1.0);
            vertices.emplace_back(0.0);

            vertices.emplace_back(1.0 * (i + 1) / NMAX * WINDOW_WIDTH);
            vertices.emplace_back(columnHeight);
            vertices.emplace_back(0.0);
            vertices.emplace_back(1.0);
            vertices.emplace_back(0.0);

            vertices.emplace_back(1.0 * i / NMAX * WINDOW_WIDTH);
            vertices.emplace_back(columnHeight);
            vertices.emplace_back(0.0);
            vertices.emplace_back(1.0);
            vertices.emplace_back(0.0);

            vertices.emplace_back(1.0 * i / NMAX * WINDOW_WIDTH);
            vertices.emplace_back(0.0);
            vertices.emplace_back(0.0);
            vertices.emplace_back(1.0);
            vertices.emplace_back(0.0);
        }
        else if (!swappedPosition[i])
        {
            vertices.emplace_back(1.0 * i / NMAX * WINDOW_WIDTH);
            vertices.emplace_back(0.0);
            vertices.emplace_back(1.0 - 1.0 * numbers[i] / NMAX);
            vertices.emplace_back(1.0 - 1.0 * numbers[i] / NMAX);
            vertices.emplace_back(1.0);

            vertices.emplace_back(1.0 * (i + 1) / NMAX * WINDOW_WIDTH);
            vertices.emplace_back(0.0);
            vertices.emplace_back(1.0 - 1.0 * numbers[i] / NMAX);
            vertices.emplace_back(1.0 - 1.0 * numbers[i] / NMAX);
            vertices.emplace_back(1.0);

            vertices.emplace_back(1.0 * (i + 1) / NMAX * WINDOW_WIDTH);
            vertices.emplace_back(columnHeight);
            vertices.emplace_back(1.0 - 1.0 * numbers[i] / NMAX);
            vertices.emplace_back(1.0 - 1.0 * numbers[i] / NMAX);
            vertices.emplace_back(1.0);

            vertices.emplace_back(1.0 * (i + 1) / NMAX * WINDOW_WIDTH);
            vertices.emplace_back(columnHeight);
            vertices.emplace_back(1.0 - 1.0 * numbers[i] / NMAX);
            vertices.emplace_back(1.0 - 1.0 * numbers[i] / NMAX);
            vertices.emplace_back(1.0);

            vertices.emplace_back(1.0 * i / NMAX * WINDOW_WIDTH);
            vertices.emplace_back(columnHeight);
            vertices.emplace_back(1.0 - 1.0 * numbers[i] / NMAX);
            vertices.emplace_back(1.0 - 1.0 * numbers[i] / NMAX);
            vertices.emplace_back(1.0);

            vertices.emplace_back(1.0 * i / NMAX * WINDOW_WIDTH);
            vertices.emplace_back(0.0);
            vertices.emplace_back(1.0 - 1.0 * numbers[i] / NMAX);
            vertices.emplace_back(1.0 - 1.0 * numbers[i] / NMAX);
            vertices.emplace_back(1.0);
        }
        else
        {
            vertices.emplace_back(1.0 * i / NMAX * WINDOW_WIDTH);
            vertices.emplace_back(0.0);
            vertices.emplace_back(0.0);
            vertices.emplace_back(1.0);
            vertices.emplace_back(0.0);

            vertices.emplace_back(1.0 * (i + 1) / NMAX * WINDOW_WIDTH);
            vertices.emplace_back(0.0);
            vertices.emplace_back(0.0);
            vertices.emplace_back(1.0);
            vertices.emplace_back(0.0);

            vertices.emplace_back(1.0 * (i + 1) / NMAX * WINDOW_WIDTH);
            vertices.emplace_back(columnHeight);
            vertices.emplace_back(0.0);
            vertices.emplace_back(1.0);
            vertices.emplace_back(0.0);

            vertices.emplace_back(1.0 * (i + 1) / NMAX * WINDOW_WIDTH);
            vertices.emplace_back(columnHeight);
            vertices.emplace_back(0.0);
            vertices.emplace_back(1.0);
            vertices.emplace_back(0.0);

            vertices.emplace_back(1.0 * i / NMAX * WINDOW_WIDTH);
            vertices.emplace_back(columnHeight);
            vertices.emplace_back(0.0);
            vertices.emplace_back(1.0);
            vertices.emplace_back(0.0);

            vertices.emplace_back(1.0 * i / NMAX * WINDOW_WIDTH);
            vertices.emplace_back(0.0);
            vertices.emplace_back(0.0);
            vertices.emplace_back(1.0);
            vertices.emplace_back(0.0);
        }
    }

    if (!vertices.empty())
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(double) * vertices.size(), &(vertices.front()), GL_DYNAMIC_DRAW);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 5);
    }

    for (int i = 0; i < NMAX; ++i)
    {
        if (swappedPosition[i])
        {
            appliedSwap.emplace_back(i);
            swappedPosition[i] = false;
        }
    }
}

void drawRoutine()
{
    for (int i = 0; i < 2; ++i)
    {
        updateDeltaTime();

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // handleInput();

        drawArray();

        glfwSwapBuffers(window);
        glfwPollEvents();

        while (glfwGetTime() - currentTime < TIME_BETWEEN_FRAMES) {}
    }
}

void bubbleSort()
{
    bool bubbleSortSorted = false;

    while (!bubbleSortSorted)
    {
        bubbleSortSorted = true;

        for (int i = 0; i < NMAX - 1; ++i)
        {
            if (numbers[i] > numbers[i + 1])
            {
                bubbleSortSorted = false;

                swappedPosition[i] = true;
                swappedPosition[i + 1] = true;
                drawRoutine();
            }
        }
    }
}

void quickSort(int left, int right)
{
    if (left >= right)
        return;

    int pivot = randomNumberGenerator() % (right - left + 1) + left;

    if (pivot != right)
    {
        swappedPosition[pivot] = true;
        swappedPosition[right] = true;
        drawRoutine();

        pivot = right;
    }

    int leftMargin = left - 1;

    for (int i = left; i < right; ++i)
    {
        if (numbers[i] < numbers[pivot])
        {
            ++leftMargin;
            if (i != leftMargin)
            {
                swappedPosition[i] = true;
                swappedPosition[leftMargin] = true;
                drawRoutine();
            }
        }
    }

    ++leftMargin;
    if (pivot != leftMargin)
    {
        swappedPosition[pivot] = true;
        swappedPosition[leftMargin] = true;
        drawRoutine();
    }

    pivot = leftMargin;

    quickSort(left, pivot - 1);
    quickSort(pivot + 1, right);
}

void heapify(int index, int size)
{
    int maxIndex = index;

    if ((index + 1) * 2 - 1 < size && numbers[(index + 1) * 2 - 1] > numbers[maxIndex])
        maxIndex = (index + 1) * 2 - 1;

    if ((index + 1) * 2 + 1 - 1 < size && numbers[(index + 1) * 2 + 1 - 1] > numbers[maxIndex])
        maxIndex = (index + 1) * 2 + 1 - 1;

    if (maxIndex != index)
    {
        swappedPosition[maxIndex] = true;
        swappedPosition[index] = true;
        drawRoutine();

        heapify(maxIndex, size);
    }
}

void heapSort()
{
    for (int i = NMAX - 1; i >= 0; --i)
        heapify(i, NMAX);

    for (int i = NMAX - 1; i >= 0; --i)
    {
        if (i != 0)
        {
            swappedPosition[i] = true;
            swappedPosition[0] = true;
            drawRoutine();

            heapify(0, i);
        }
    }
}

void sort()
{
    //bubbleSort();
    //quickSort(0, NMAX - 1);
    heapSort();
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Sorting Algorithms Visualizer", 0, 0);
    //glfwGetPrimaryMonitor();

    glfwMakeContextCurrent(window);

    glewInit();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

    int orthoPath = glGetUniformLocation(shaderProgram, "ortho");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 5 * sizeof(double), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 5 * sizeof(double), (void*)(2 * sizeof(double)));
    glEnableVertexAttribArray(1);

    glm::mat4 ortho = glm::ortho(0.0, 1.0 * WINDOW_WIDTH, 0.0, 1.0 * WINDOW_HEIGHT);
    glUniformMatrix4fv(orthoPath, 1, GL_FALSE, glm::value_ptr(ortho));

    //
    randomNumberGenerator = mt19937(randomDevice());

    for (int i = 0; i < NMAX; ++i)
        numbers[i] = i + 1;

    for (int i = NMAX - 1; i >= 1; --i)
    {
        int poz = randomNumberGenerator() % (i + 1);
        swap(numbers[i], numbers[poz]);
    } 
    //

    drawRoutine();

    sort();

    for (int i = 0; i < NMAX; ++i)
    {
        finalColoring[i] = true;

        drawRoutine();
    }

    while (!glfwWindowShouldClose(window))
    {
        updateDeltaTime();

        handleInput();

        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}