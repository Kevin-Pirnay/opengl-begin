
const char *get_shader_code(const char *file_name)
{
    FILE *source_file = fopen(file_name, "r");

    if ( source_file == NULL ) { printf("error while opening %s\n", file_name); exit(1); }

    fseek(source_file, 0, SEEK_END);

    int length_file = ftell(source_file);

    fseek(source_file, 0, SEEK_SET);

    char *content = malloc(length_file + 1);
    int content_ptr = 0;
    char c;
    while (1)
    {
        c = fgetc(source_file);

        if ( c == EOF ) break;

        content[content_ptr] = c;

        content_ptr++; 
    }

    content[content_ptr] = '\0';

    return (const char *)content;
}

unsigned int compiling_shader_program(unsigned int shader_type, const char *shader_source_code)
{
    unsigned int shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &shader_source_code, NULL);
    glCompileShader(shader_id);

    return shader_id;
}

void check_if_shader_compiled_successfully(unsigned int shader_id, const char *shader_name)
{
    int  success;
    char infoLog[512];

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(shader_id, 512, NULL, infoLog);

        printf("ERROR::SHADER::%s::COMPILATION_FAILED\n %s\n",shader_name, infoLog);

        exit(1);
    }
}

unsigned int create_and_link_shaders_program(unsigned int vertex_shader_id, unsigned int fragment_shader_id)
{
    unsigned int program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);     

    return program_id;
}

void check_if_program_link_successfully(unsigned int program_id)
{
    int  success;
    char infoLog[512];

    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program_id, 512, NULL, infoLog);

        printf("ERROR::PROGRAM::LINK_FAILED\n %s\n", infoLog);

        exit(1); 
    }
}

unsigned int create_program(const char *vertex_source_file, const char *fragment_source_file)
{
    const char *vertex_source_code = get_shader_code(vertex_source_file);
    const char *fragment_source_code = get_shader_code(fragment_source_file);

    unsigned int vertex_shader_id = compiling_shader_program(GL_VERTEX_SHADER, vertex_source_code);
    unsigned int fragment_shader_id = compiling_shader_program(GL_FRAGMENT_SHADER, fragment_source_code);

    check_if_shader_compiled_successfully(vertex_shader_id, "VERTEX SHADER");
    check_if_shader_compiled_successfully(fragment_shader_id, "FRAGMENT SHADER");

    free((void *)vertex_source_code);
    free((void *)fragment_source_code);

    unsigned int program_id = create_and_link_shaders_program(vertex_shader_id, fragment_shader_id);

    check_if_program_link_successfully(program_id);

    return program_id;
}

unsigned int pre_process()
{
    const char *vertex_source_file = "shaders/vertex.c";
    const char *fragment_source_file = "shaders/fragment.c";

    unsigned int program_id = create_program(vertex_source_file, fragment_source_file);

    return program_id;
}