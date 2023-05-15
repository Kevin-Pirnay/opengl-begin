#define GL_SILENCE_DEPRECATION (1)
#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stb_image.h"


#include "init_window.c"
#include "pre_process.c"
#include "render.c"
#include "load_data_gpu.c"


int main(int argc, char const *argv[])
{
    init_window(&pre_process, &load_data_into_gpu_memory, &render_process);

    return 0;
}
