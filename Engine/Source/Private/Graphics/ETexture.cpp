#include "Graphics/ETexture.h"

// External Libs
#include <GLEW/glew.h>
#include <STB_IMAGE/stb_image.h>

ETexture::ETexture()
{
    m_path = m_fileName = "";
    m_ID = 0U;
    m_width = m_height = m_channels = 0;
}

ETexture::~ETexture()
{
    // If ID was generated, delete the texture
    if (m_ID > 0)
        glDeleteTextures(1, &m_ID);

    // EDebug::Log("Texture destroyed: " + m_fileName);
}

bool ETexture::LoadTexture(const EString& fileName, const EString& path, bool repeat, bool linear)
{
    // Assign the file name and path
    m_fileName = fileName;
    m_path = path;

    // STB Image imports images upside down
    // But OpenGL reads them in an inverted state
    stbi_set_flip_vertically_on_load(true);

    // Load the image into a computer readable format
    unsigned char* data = stbi_load(
        m_path.c_str(),         // Path to the image
        &m_width, &m_height,    // Width and height of the image
        &m_channels,            // RGBA
        0   // Limit of required amount of channels (0 = no limit)
    );

    // Log channels
    //EDebug::Log("Texture: " + fileName + " | Channels: " + std::to_string(m_channels));

    // Test if the data imported correctly
    if (data == nullptr) {
        EString errorMsg = "Failed to load texture - " + m_fileName + ": " + stbi_failure_reason();
        EDebug::Log(errorMsg, LT_ERROR);
        return false;
    }

    // Test the amount of channels
    if (m_channels > 4 || m_channels < 3) {
        EDebug::Log("Failed to import texture - " + m_fileName 
            + ": Incorrect number of channels, must have 3 or 4 channels");
        return false;
    }

    // Generate the texture ID in OpenGL
    glGenTextures(1, &m_ID);

    // Test if the generate failed
    if (m_ID == 0) {
        EString error = reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
        EString errorMsg = "Failed to generate texture ID - " + m_fileName + ": " + error;
        EDebug::Log(errorMsg, LT_ERROR);
        return false;
    }
    
    // Bind the texture
    // Tells OpenGL that we want to use this texture
    glBindTexture(GL_TEXTURE_2D, m_ID);

    // Set default parameters for the texture
    // Set the texture wrapping parameters
    // If the texture does not fit the model, repeat texture
    GLint wrapMode = repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

    // Set the filtering parameters
    // How much to blur pixels 
    // The resolution of the texture is lower than the size of the model
    GLint filter = linear ? GL_LINEAR : GL_NEAREST;
    GLint minFilter = linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    // Set the default format at 3 channels
    GLint intFormat = GL_RGB;

    // If the imported image channels is 4, set the import format to RGBA
    if (m_channels == 4) {
        intFormat = GL_RGBA;
    }

    // RGB textures are 3 bytes per pixel which may not be 4-byte aligned
    // OpenGL defaults to 4-byte alignment so we must set it to 1 for RGB
    if (m_channels == 3) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    // Load the image data into the texture we just updated
    glTexImage2D(
        GL_TEXTURE_2D,      // Use a 2D Texture
        0,                  // Levels
        intFormat,          // Internal Texture format
        m_width, m_height,  // Width & height
        0,                  // Image border (legacy)
        intFormat,          // External texture format
        GL_UNSIGNED_BYTE,   // Data type passed in
        data                // Image Data from STBI
    );

    // Reset alignment to 4-byte default
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    // Generate mip maps
    // Lower resolutions versions of texture
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind the texture from OpenGL
    // Makes room for next texture
    Unbind();

    // Clear STBI Image data
    stbi_image_free(data);

    // Log the success of the import
    // EDebug::Log("Successfully imported texture - " + m_fileName, LT_SUCCESS);

    return true;
}

void ETexture::BindTexture(const EUi32& textureNumber)
{
    // Active texture in the shader
    glActiveTexture(GL_TEXTURE0 + textureNumber);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void ETexture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
