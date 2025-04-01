#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#include <GL/gl.h>
#include <string>

class TextureLoader {
public:
  static GLuint loadTexture(const std::string &filename);
};

#endif