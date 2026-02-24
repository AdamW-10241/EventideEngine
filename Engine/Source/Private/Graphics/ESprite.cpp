#include "Graphics/ESprite.h"
#include "Graphics/EShaderProgram.h"
#include "Math/ESTransform.h"

// External Libs
#include <GLEW/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

bool ESprite::CreateSprite(const EString& texturePath)
{
    // Create sprite
    if (!CreateSprite()) return false;

    // Load texture
    if (!LoadTexture(texturePath, texturePath, false, false)) {
        EDebug::Log("ESprite failed to load texture.", LT_ERROR);
        return false;
    }

    return true;
}

bool ESprite::CreateSprite()
{
    // A simple quad, two triangles
    std::vector<ESVertexData> vertices = {
        // position              color    texcoords
        { {0.0f, 1.0f, 0.0f}, {1,1,1}, {0.0f, 0.0f} }, // top-left
        { {0.0f, 0.0f, 0.0f}, {1,1,1}, {0.0f, 1.0f} }, // bottom-left
        { {1.0f, 0.0f, 0.0f}, {1,1,1}, {1.0f, 1.0f} }, // bottom-right
        { {1.0f, 1.0f, 0.0f}, {1,1,1}, {1.0f, 0.0f} }  // top-right
    };

    std::vector<uint32_t> indices = {
        0, 1, 2,
        0, 2, 3
    };

    // Create mesh
    if (!CreateMesh(vertices, indices)) {
        EDebug::Log("ESprite failed to create mesh.", LT_ERROR);
        return false;
    }

    return true;
}

void ESprite::Render(const TShared<EShaderProgram>& shader, ESTransform2D& transform)
{
    shader->Activate();
    
    // Get viewport dimensions
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float screenWidth = static_cast<float>(viewport[2]);
    float screenHeight = static_cast<float>(viewport[3]);

    // Screen-space orthographic projection
    glm::mat4 projection = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);

    // Model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(transform.position, 0.0f));

    // Rotate around center
    model = glm::translate(model, glm::vec3(0.5f * transform.scale, 0.0f));
    model = glm::rotate(model, glm::radians(transform.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * transform.scale, 0.0f));

    model = glm::scale(model, glm::vec3(transform.scale, 1.0f));

    // Set uniforms
    GLuint programID = shader->GetProgramID();

    int varID = glGetUniformLocation(programID, "projection");
    glUniformMatrix4fv(varID, 1, GL_FALSE, glm::value_ptr(projection));

    varID = glGetUniformLocation(programID, "model");
    glUniformMatrix4fv(varID, 1, GL_FALSE, glm::value_ptr(model));

    glUniform1i(glGetUniformLocation(programID, "useTexture"), !m_fileName.empty() ? 1 : 0);

    glUniform4f(glGetUniformLocation(programID, "color"), 
        m_renderColor.r, m_renderColor.g, m_renderColor.b, m_renderColor.a);
    
    // Bind texture
    BindTexture(0);
    varID = glGetUniformLocation(programID, "sprite");
    glUniform1i(varID, 0);

    // Draw
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}