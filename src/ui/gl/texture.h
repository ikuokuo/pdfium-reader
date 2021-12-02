#pragma once

#include "gl.h"

class Texture {
 public:
  explicit Texture(bool create = true) {
    if (create) Create();
  }
  ~Texture() {
    Delete();
  }

  GLuint Id() {
    return id_;
  }

  void Create() {
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    id_ = texture_id;
  }

  void Bind(GLint n, GLuint program, const GLchar *name) {
    glActiveTexture(GL_TEXTURE0 + n);
    glBindTexture(GL_TEXTURE_2D, id_);
    glUniform1i(glGetUniformLocation(program, name), n);
  }

  void Fill(GLenum format, GLsizei width, GLsizei height,
      GLenum pixel_format, const void *pixels) {
    glBindTexture(GL_TEXTURE_2D, id_);
    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, pixel_format,
                 GL_UNSIGNED_BYTE, pixels);
  }

  void Delete() {
    if (id_ == GL_ZERO) return;
    glDeleteTextures(1, &id_);
    id_ = GL_ZERO;
  }

 private:
  GLuint id_ = GL_ZERO;
};
