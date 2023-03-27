#include "Texture.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

#include <iostream>

Texture::Texture(const std::string& name) {
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    std::string ext = name.substr(name.length() - 3);
    unsigned char* imageData = stbi_load(name.c_str(), &width, &height, &nrChannels, 0);
    if (imageData) {
        if (ext == "jpg") glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        else if (ext == "png") glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        else std::cout << "invalid file type\n";
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture\n";
    }
    stbi_image_free(imageData);
    // assocaite each texture with a new texture unit
    static size_t u = 0;
    unit = u++;
}

void Texture::setWrapMode(Wrap_type type) {
    glBindTexture(GL_TEXTURE_2D, ID);
    if (type == Wrap_type::RPT) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else if (type == Wrap_type::MRP) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    }
    else if (type == Wrap_type::CTE) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else if (type == Wrap_type::CTB) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);       
    }
    else {
        std::cout << "Wrap type not implemetated";
    }
}

void Texture::setMinFilter(Filter_type type) {
    glBindTexture(GL_TEXTURE_2D, ID);
    switch (type) {
        case Filter_type::LIN:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case Filter_type::NER:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        case Filter_type::LML:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            break;
        case Filter_type::LMN:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            break;
        case Filter_type::NML:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            break;
        case Filter_type::NMN:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            break;
        default:
            std::cout << "Filter type not implemented.\n";
    }
}

void Texture::setMagFilter(Filter_type type) {
    glBindTexture(GL_TEXTURE_2D, ID);
    if (type == Filter_type::LIN) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    else if (type == Filter_type::NER) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    else std::cout << "Magnification filter only accepts LIN (linear) or NER (nearest) filter settings\n";
}

void Texture::setBoarderCol(std::vector<float>& rgba) {
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, rgba.data());
}

void Texture::bind(void) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}

Texture::~Texture(void) {
    glDeleteTextures(1, &ID);
}