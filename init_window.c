
void check_if_window_is_created_successfully(GLFWwindow *window)
{
    if ( !window )
    {
        glfwTerminate();

        printf("error while creating window\n");

        exit(1);
    }
}

GLFWwindow *initialize_window()
{
    if ( !glfwInit() ) { printf("error while initiating glfw\n"); exit(1); }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "opengl execise", NULL, NULL);

    check_if_window_is_created_successfully(window);

    unsigned int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    unsigned int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);

    printf("oepngl shader version: major: %d - minor: %d\n", major, minor);

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE; 

    if ( glewInit() != GLEW_OK ) { printf("error while initiating glew\n");  exit(1); }

    return window;
}

void init_window(unsigned int (*pre_process_program_ptr)(void), unsigned int (*pre_load_data_ptr)(unsigned int), void (*render_process_ptr)(unsigned int, unsigned int, float *theta))
{
    GLFWwindow *window = initialize_window();

    unsigned int program_id = (*pre_process_program_ptr)();

    unsigned int vao_id = (*pre_load_data_ptr)(program_id);

    float theta = 0;

    while ( !glfwWindowShouldClose(window) )
    {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        (*render_process_ptr)(program_id, vao_id, &theta);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
}