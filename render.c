#include <math.h>

void generate_rotation_x(float *ptr, float angle)
{
    ptr[0] = (float)cos(angle);     ptr[1] = (float)sin(angle);     ptr[2] = 0.0f;          ptr[3] = 0.0f;
    ptr[4] = (float)-sin(angle);    ptr[5] = (float)cos(angle);     ptr[6] = 0.0f;          ptr[7] = 0.0f;
    ptr[8] = 0.0f;                  ptr[9] = 0.0f;                  ptr[10] = 1.0f;         ptr[11] = 0.0f;
    ptr[12] = 0.0f;                 ptr[13] = 0.0f;                 ptr[14] = 0.0f;         ptr[15] = 1.0f;
}

void generate_rotation_y(float *ptr, float angle)
{
    ptr[0] = (float)cos(angle);     ptr[1] = 0;                     ptr[2] = (float)sin(angle);          ptr[3] = 0.0f;
    ptr[4] = 0.0f;                  ptr[5] = 1.0f;                  ptr[6] = 1.0f;                       ptr[7] = 0.0f;
    ptr[8] = (float)sin(angle);     ptr[9] = 0.0f;                  ptr[10] = (float)cos(angle);         ptr[11] = 0.0f;
    ptr[12] = 0.0f;                 ptr[13] = 0.0f;                 ptr[14] = 0.0f;                      ptr[15] = 1.0f;
}

void generate_rotation_z(float *ptr, float angle)
{
    ptr[0] = 0.0f;                  ptr[1] = 1.0f;                  ptr[2] = 1.0f;          ptr[3] = 0.0f;
    ptr[4] = (float)cos(angle);     ptr[5] = (float)-sin(angle);    ptr[6] = 0.0f;          ptr[7] = 0.0f;
    ptr[8] = (float)sin(angle);     ptr[9] = (float)cos(angle);     ptr[10] = 0.0f;         ptr[11] = 0.0f;
    ptr[12] = 0.0f;                 ptr[13] = 0.0f;                 ptr[14] = 0.0f;         ptr[15] = 1.0f;
}

unsigned int get_location_id_uniform(unsigned int program_id, const char *name_uniform)
{
    return glGetUniformLocation(program_id, name_uniform);
}

void set_float_matrix_uniform(unsigned int location_id_matrix, float *matrix_ptr)
{
    glUniformMatrix4fv(location_id_matrix, 1, GL_FALSE, matrix_ptr);
}

void set_matrix_z_uniforms(float angle, unsigned int program_id)
{
    unsigned int rotation_z_location = get_location_id_uniform(program_id, "rotation_z");

    float rotation_z[16];

    generate_rotation_z(rotation_z, angle);

    set_float_matrix_uniform(rotation_z_location, rotation_z);
}

void set_matrix_x_uniforms(float angle, unsigned int program_id)
{
    unsigned int rotation_x_location = get_location_id_uniform(program_id, "rotation_x");

    float rotation_x[16];

    generate_rotation_x(rotation_x, angle);

    set_float_matrix_uniform(rotation_x_location, rotation_x);
}

void set_matrix_y_uniforms(float angle, unsigned int program_id)
{
    unsigned int rotation_y_location = get_location_id_uniform(program_id, "rotation_y");

    float rotation_y[16];

    generate_rotation_y(rotation_y, angle);

    set_float_matrix_uniform(rotation_y_location, rotation_y);
}

void set_uniform(float *theta, unsigned int program_id)
{
    float angle = (*theta) * (M_PI/180);

    set_matrix_z_uniforms(angle, program_id);

    set_matrix_y_uniforms(angle, program_id);

    set_matrix_x_uniforms(angle, program_id);

    (*theta) += 1.0;
}

void set_uniform_t(unsigned int program_id)
{
    float translation[] = {
        1.0f, 0.0f, 0.0f, 0.5f, 
        0.0f, 1.0f, 0.0f, 0.5f, 
        0.0f, 0.0f, 1.0f, 0.5f, 
        0.0f, 0.0f, 0.0f, 0.5f, 
    };

    float rotation_y[12];

    generate_rotation_y(rotation_y, M_PI/3);

    unsigned int translation_id =  glGetUniformLocation(program_id, "translation");
    glUniformMatrix4fv(translation_id, 1, GL_FALSE, rotation_y);
}

void render_process(unsigned int program_id, unsigned int vao_id, float *theta)
{
    glUseProgram(program_id);
    glBindVertexArray(vao_id);

    set_uniform(theta, program_id);

    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}