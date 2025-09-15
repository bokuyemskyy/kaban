#pragma once

#include <GL/gl.h>
#include <imgui.h>
#include <stb_image.h>

#include <stdexcept>

#include "piece.hpp"
#include "resource_manager.hpp"

class TextureManager {
   public:
    TextureManager() {
        for (Piece piece : Pieces::all()) {
            std::string     name(1, piece.toChar());
            const Resource* resource = ResourceManager::get(name + ".png");
            if (!resource) throw std::runtime_error("Texture not found");

            m_pieceTextures[piece] = create_gl_texture(resource);
        }
    }

    ImTextureID get(Piece piece) const { return m_pieceTextures.at(piece); }

    ~TextureManager() {
        for (auto& [piece, texture] : m_pieceTextures) {
            glDeleteTextures(1, reinterpret_cast<GLuint*>(&texture));
        }
    }

   private:
    std::unordered_map<Piece, ImTextureID> m_pieceTextures;

    static ImTextureID create_gl_texture(const Resource* resource);
};
