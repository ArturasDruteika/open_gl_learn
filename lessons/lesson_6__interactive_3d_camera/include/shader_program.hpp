#pragma once

#include <string>


class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    bool create(
        const std::string& vertex_source,
        const std::string& fragment_source
    );

    void destroy();
    void use() const;
    int get_uniform_location(const char* name) const;

private:
    static bool compile_shader(
        unsigned int shader_id,
        const std::string& source
    );

private:
    unsigned int m_id;
};