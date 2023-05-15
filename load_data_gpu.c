
static unsigned int generate_vertex_array_object() //for structure of memory
{
    unsigned int vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    return vao_id;
}

static unsigned int generate_vertex_buffer_object() //for memory
{
    unsigned int vbo_id;
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

    return vbo_id;
}

static unsigned int generate_indices_buffer_object()
{
    unsigned int ibo_id;
    glGenBuffers(1, &ibo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);

    return ibo_id;
}

static void allocate_memory_on_gpu_for_vbo(int memory_size, int storage_mode)
{
    glBufferData(GL_ARRAY_BUFFER, memory_size, NULL, storage_mode);
}

static void load_data_into_gpu_memory_allocated(int offset, int nb_bytes, const void *data)
{
    glBufferSubData(GL_ARRAY_BUFFER, offset, nb_bytes, data);
}

static unsigned int get_position_variable_id(unsigned int program_id, const char *variable_name)
{
    unsigned int position_variable_id = glGetAttribLocation(program_id, variable_name);

    return position_variable_id;
}

static void assign_position_from_memory_for_attib_id_and_tell_how_to_read_it(unsigned int position_variable_id, int nb_element, int type, int normalized, int stride, const void *offset)
{
    glVertexAttribPointer(position_variable_id, nb_element, type, normalized, stride, offset);
}

unsigned int load_texture_into_graphic_card(const char *path)
{
    //stbi_set_flip_vertically_on_load(1);
    int width, height, bit_depth;

    const void *pixels_data = stbi_load(path, &width, &height, &bit_depth, 4);

    if ( !pixels_data ) { printf("failed to find %s", path); exit(1); }

    unsigned int texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_data);

    glGenerateMipmap(GL_TEXTURE_2D);

    if(pixels_data) stbi_image_free((void *)pixels_data);

    return texture_id;
}

unsigned int load_data_into_gpu_memory(unsigned int program_id)
{
    float vertices[] = {
    -0.5f,  0.0f, 0.0f,
     0.5f,  0.0f, 0.0f,
     0.0f,  0.5f, 0.0f,
     0.0f, -0.5f, 0.0f
    };  

    int size_vertices = sizeof(float) * (3 * 4);

    unsigned int indices[] = {  
        0, 1, 2,   
        0, 1, 3 
    };  

    int size_indices = sizeof(unsigned int) * (3 * 2);

    float colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f
    }; 

    int size_colors = sizeof(float) * (3 * 4);

    float texCoords[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
    };

    int size_texCoords = sizeof(float) * (2 * 4);

    //make the buffer

    unsigned int vbo_id = generate_vertex_buffer_object();

    allocate_memory_on_gpu_for_vbo(size_vertices + size_colors + size_texCoords, GL_STATIC_DRAW);

    load_data_into_gpu_memory_allocated(0, size_vertices, vertices); // load positions

    load_data_into_gpu_memory_allocated(size_vertices, size_colors, colors); // load colors

    load_data_into_gpu_memory_allocated(size_vertices + size_colors, size_texCoords, texCoords); // load texCoords

    //add to the buffer the description of this buffer

    unsigned int vao_id = generate_vertex_array_object();

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id); // Attach the active buffer to the active array,

    //assign data to pos_input into vertex shader program

    unsigned int position_variable_id = glGetAttribLocation(program_id, "pos_input");

    glVertexAttribPointer(position_variable_id, 3, GL_FLOAT, GL_FALSE, 0, 0); //assign to ptr

    glEnableVertexAttribArray(position_variable_id);

    //assign data to col_input into vertex shader program

    unsigned int color_variable_id = glGetAttribLocation(program_id, "col_input");

    glVertexAttribPointer(color_variable_id, 3, GL_FLOAT, GL_FALSE, 0, (void *)((unsigned long)size_vertices)); //assign to ptr

    glEnableVertexAttribArray(color_variable_id);

    //use texture 
    unsigned int texture_id = load_texture_into_graphic_card("img/wall.jpeg");

    //assign data to texCoords_input into vertex shader program

    unsigned int texCoords_variable_id = glGetAttribLocation(program_id, "texCoords_input");

    glVertexAttribPointer(texCoords_variable_id, 2, GL_FLOAT, GL_FALSE, 0, (void *)((unsigned long)(size_vertices + size_colors))); //assign to ptr

    glEnableVertexAttribArray(texCoords_variable_id);

    //use indices
    
    unsigned int iao_id =  generate_indices_buffer_object();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_indices, indices, GL_STATIC_DRAW); 

    return vao_id;
}
