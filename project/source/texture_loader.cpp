#include "texture_loader.hpp"

#include <GL/gl.h>

#include <error_handler.hpp>

#include "resource_manager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLuint TextureLoader::loadTexture(const std::string &filename) {
    int width{};
    int height{};
    int channels{};

    Resource resource = ResourceManager::getResource(filename);
    unsigned char *data = stbi_load_from_memory(resource.first, static_cast<int>(resource.second),
                                                &width, &height, &channels, 4);

    if (!data) {
        handleError(1, stbi_failure_reason());
        return 0;
    }

    for (int i = 0; i < height / 2; ++i) {
        for (int j = 0; j < width * 4; ++j) {
            std::swap(data[i * width * 4 + j], data[(height - i - 1) * width * 4 + j]);
        }
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    stbi_image_free(data);
    return texture;
}