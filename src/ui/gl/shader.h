#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include "gl.h"

class Shader {
 public:
  Shader() = default;
  Shader(const char *vertex_shader_code,
         const char *fragment_shader_code,
         const char *geometry_shader_code = nullptr) {
    Create(vertex_shader_code, fragment_shader_code, geometry_shader_code);
  }
  ~Shader() {
    Delete();
  }

  void Create(const char *vertex_shader_code,
              const char *fragment_shader_code,
              const char *geometry_shader_code = nullptr) {
    // vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex_shader);
    CheckCompileErrors(vertex_shader, "VERTEX");

    // fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment_shader);
    CheckCompileErrors(fragment_shader, "FRAGMENT");

    // geometry shader
    GLuint geometry_shader = 0;
    if (geometry_shader_code != nullptr) {
      geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
      glShaderSource(geometry_shader, 1, &geometry_shader_code, NULL);
      glCompileShader(geometry_shader);
      CheckCompileErrors(geometry_shader, "GEOMETRY");
    }

    // link shaders
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    if (geometry_shader_code != nullptr)
      glAttachShader(shader_program, geometry_shader);
    glLinkProgram(shader_program);
    CheckCompileErrors(shader_program, "PROGRAM");

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    if (geometry_shader_code != nullptr)
      glDeleteShader(geometry_shader);

    id_ = shader_program;
  }

  GLuint Id() {
    return id_;
  }

  void Delete() {
    if (id_ == GL_ZERO) return;
    glDeleteProgram(id_);
    id_ = GL_ZERO;
  }

  void Use() {
    glUseProgram(id_);
  }

  void SetBool(const char *name, bool value) const {
    glUniform1i(glGetUniformLocation(id_, name), static_cast<int>(value));
  }

  void SetInt(const char *name, int value) const {
    glUniform1i(glGetUniformLocation(id_, name), value);
  }

  void SetFloat(const char *name, float value) const {
    glUniform1f(glGetUniformLocation(id_, name), value);
  }

  void SetVec2(const char *name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(id_, name), 1, &value[0]);
  }
  void SetVec2(const char *name, float x, float y) const {
    glUniform2f(glGetUniformLocation(id_, name), x, y);
  }

  void SetVec3(const char *name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(id_, name), 1, &value[0]);
  }
  void SetVec3(const char *name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(id_, name), x, y, z);
  }

  void SetVec4(const char *name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(id_, name), 1, &value[0]);
  }
  void SetVec4(const char *name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(id_, name), x, y, z, w);
  }

  void SetMat2(const char *name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(
        glGetUniformLocation(id_, name), 1, GL_FALSE, &mat[0][0]);
  }

  void SetMat3(const char *name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(
        glGetUniformLocation(id_, name), 1, GL_FALSE, &mat[0][0]);
  }

  void SetMat4(const char *name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(
        glGetUniformLocation(id_, name), 1, GL_FALSE, &mat[0][0]);
  }

 private:
  GLuint id_ = GL_ZERO;

  void CheckCompileErrors(GLuint shader, const char *type) {
    GLint success;
    GLchar infoLog[1024];
    if (strcmp(type, "PROGRAM") != 0) {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
            << infoLog << std::endl;
      }
    } else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
            << infoLog << std::endl;
      }
    }
  }
};
