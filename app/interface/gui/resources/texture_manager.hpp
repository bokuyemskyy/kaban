#pragma once

#include <imgui.h>
#include <stb_image.h>
#include <string>
#include <stdexcept>

#include "color.hpp"

#if defined(_WIN32)
    #include <windows.h>
    #include <GL/gl.h>
#elif defined(__linux__)
    #include <GL/gl.h>
#elif defined(__APPLE__)
    #define GL_SILENCE_DEPRECATION
    #include <TargetConditionals.h>
    #include <OpenGL/gl3.h>
    #include <OpenGL/gl3ext.h>
#endif

#include "piece.hpp"
#include "resource_manager.hpp"

class TextureManager {
   public:
    TextureManager() {
        for (Piece piece : Pieces::all()) {
            std::string name = {};
            name += static_cast<char>(std::tolower(piece.toChar()));
            name += piece.color() == Colors::WHITE ? 'w' : 'b';
            const Resource* resource = ResourceManager::get(name + ".png");
            if (!resource) throw std::runtime_error("Texture not found");

            m_pieceTextures[piece] = createGlTexture(resource);
        }
    }

    ImTextureID get(Piece piece) const { return ImTextureID(m_pieceTextures.at(piece)); }

    ~TextureManager() {
        for (auto& [piece, texture] : m_pieceTextures) {
            glDeleteTextures(1, &texture);
        }
    }

   private:
    std::unordered_map<Piece, GLuint> m_pieceTextures{};

    static GLuint createGlTexture(const Resource* resource);
};
