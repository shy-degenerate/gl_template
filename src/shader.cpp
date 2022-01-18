#include "shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string &vertex_path, const std::string &fragment_path)
{
    std::string vertex_shader_src = "";
    std::string fragment_shader_src = "";

    std::ifstream vertex_file(vertex_path);
    std::ifstream fragment_file(fragment_path);

    if (!vertex_file.is_open() && !fragment_file.is_open())
    {
        std::cout << "Failed to load files" << std::endl;
    }
    else
    {
        std::string line = "";
        while (!vertex_file.eof())
        {
            std::getline(vertex_file, line);
            vertex_shader_src += line + '\n';
        }
        while (!fragment_file.eof())
        {
            std::getline(fragment_file, line);
            fragment_shader_src += line + '\n';
        }

        vertex_file.close();
        fragment_file.close();
    }

    uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    const char *vertex_c = vertex_shader_src.c_str();
    const char *fragment_c = fragment_shader_src.c_str();

    glShaderSource(vertex_shader, 1, &vertex_c, nullptr);
    glShaderSource(fragment_shader, 1, &fragment_c, nullptr);

    glCompileShader(vertex_shader);
    glCompileShader(fragment_shader);

    int32_t success;
    char infoLog[512];

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED "
                  << vertex_path << '\n'
                  << infoLog << std::endl;
    }

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED "
                  << fragment_path << '\n'
                  << infoLog << std::endl;
    }

    this->program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);

    glLinkProgram(program_id);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program_id, 512, nullptr, infoLog);
    }
}

void Shader::use()
{
    glUseProgram(program_id);
}

void Shader::set_bool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(this->program_id, name.c_str()), (int32_t)value);
}

void Shader::set_int(const std::string &name, int32_t value) const
{
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
}

void Shader::set_float(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
}

void Shader::set_mat4(const std::string &name, const glm::mat4 &value) const
{
    glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_vec3(const std::string &name, glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
}

void Shader::set_vec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(program_id, name.c_str()), x, y, z);
}