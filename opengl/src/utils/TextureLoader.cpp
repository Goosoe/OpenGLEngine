#include "TextureLoader.h"

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/**
 * Loads a texture from path to the selected textureUnit.
 * It checks if it s a jpeg or png and does the necessary modifications
 */
void texture::loadTexture(const GLenum textureUnit, const std::string path)
{
    stbi_set_flip_vertically_on_load(true);  
    // texture load
    unsigned int texture;
    int width, height, nrChannels;
    
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    glGenTextures(1, &texture);
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (!data)
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    GLuint colorVal = GL_RGB;
    //if its a png file extension
    if(path.find(".png") != std::string::npos){
        colorVal = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, colorVal, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}
