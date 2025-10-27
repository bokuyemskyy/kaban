#include "texture_manager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "resource_manager.hpp"

ImTextureID TextureManager::createGlTexture(const Resource* resource) {
    GLuint texture{};
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int            width = 0, height = 0, channels = 0;
    unsigned char* data =
        stbi_load_from_memory(resource->data(), static_cast<int>(resource->size()), &width, &height, &channels, 4);
    if (!data) throw std::runtime_error(stbi_failure_reason());

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return ImTextureID(texture);
}
