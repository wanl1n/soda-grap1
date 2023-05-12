#include <GLFW/glfw3.h>
#include "cmath"

int main(void)
{
    GLFWwindow* window;
    // Center of window is 0,0
    // Sides are 1 or -1.

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "Kate Nicole Young", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        float length = 0.3f;
        float angle, x, y;

        glBegin(GL_POLYGON);
        glVertex2f(length, 0.f);

        for (int i = 1; i <= 4; i++) {
            angle = (72 * i) * (3.1416 / 180);
            x = length * cos(angle);
            y = length * sin(angle);
            glVertex2f(x, y);
        }

        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}