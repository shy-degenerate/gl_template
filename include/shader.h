#ifndef _SHADER_H_
#define _SHADER_H_

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
    int program_id;

public:
    Shader(const std::string &vertex_path, const std::string &fragment_path);

    void use();

    void set_bool(const std::string &name, bool value) const;
    void set_int(const std::string &name, int32_t value) const;
    void set_float(const std::string &name, float value) const;
    void set_mat4(const std::string &name, const glm::mat4 &value) const;
    void set_vec3(const std::string &name, glm::vec3 &value) const;
    void set_vec3(const std::string &name, float x, float y, float z) const;

    inline int getProgram() const { return this->program_id; };
};

#endif