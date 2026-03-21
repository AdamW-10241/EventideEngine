#include "Graphics/ESprite.h"
#include "Graphics/EShaderProgram.h"
#include "Math/ESTransform.h"
#include "Game/EGameEngine.h"

// External Libs
#include <GLEW/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

bool ESprite::CreateSprite(const EString& texturePath)
{
    // Create sprite
    if (!CreateSprite()) return false;

    // Load texture
    if (!LoadTexture(texturePath, false, false)) {
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

void ESprite::Render(const TShared<EShaderProgram>& shader, ESTransform2D& transform, bool flipVertically)
{
    shader->Activate();
    
    // Get viewport dimensions
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float screenWidth = static_cast<float>(viewport[2]);
    float screenHeight = static_cast<float>(viewport[3]);

    // Screen-space orthographic projection
    glm::mat4 projection;
    if (flipVertically) {
        projection = glm::ortho(0.0f, screenWidth, 0.0f, screenHeight, -1.0f, 1.0f);
    }else {
        // Flip sprite vertically
        projection = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);
    }
    

    // Model matrix
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec2 renderScale = transform.scale * m_renderScale;
    glm::vec2 position = transform.position + m_positionOffset + (transform.scale - renderScale) * 0.5f;
    glm::vec2 center = position + renderScale * 0.5f;

    model = glm::translate(model, glm::vec3(center, 0.0f));
    model = glm::rotate(model, glm::radians(transform.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-renderScale * 0.5f, 0.0f));
    model = glm::scale(model, glm::vec3(renderScale, 1.0f));

    // Set uniforms
    GLuint programID = shader->GetProgramID();

    int varID = glGetUniformLocation(programID, "projection");
    glUniformMatrix4fv(varID, 1, GL_FALSE, glm::value_ptr(projection));

    varID = glGetUniformLocation(programID, "model");
    glUniformMatrix4fv(varID, 1, GL_FALSE, glm::value_ptr(model));

    glUniform1i(glGetUniformLocation(programID, "useTexture"), !m_path.empty() ? 1 : 0);

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

void ESprite::UpdateTransform(glm::vec2 windowSize)
{
    float slateUnit = windowSize.y / SLATE_UNIT_SCALAR;
    glm::vec2 pixelSize = m_doScale ? CalcPixelSize(slateUnit) : CalcPixelSize(1.0f);
    
    glm::vec2 anchorPos = m_anchor * windowSize;
    glm::vec2 pixelPos = anchorPos - pixelSize * m_alignment;

    m_transform.position = pixelPos;
    m_transform.scale = pixelSize;
}

glm::vec2 ESprite::CalcPixelSize(float slateUnit) const
{
    if (m_sizeInUnits.x != 0.0f && m_sizeInUnits.y == 0.0f) {
        float x = m_sizeInUnits.x * slateUnit;
        return { x, x / m_width * m_height };
    }
    if (m_sizeInUnits.y != 0.0f && m_sizeInUnits.x == 0.0f) {
        float y = m_sizeInUnits.y * slateUnit;
        return { y / m_height * m_width, y };
    }
    return {
        m_sizeInUnits.x == 0.0f ? (float)m_width * slateUnit : m_sizeInUnits.x * slateUnit,
        m_sizeInUnits.y == 0.0f ? (float)m_height * slateUnit : m_sizeInUnits.y * slateUnit
    };
}

void ESprite::UpdateTransform()
{
    auto window = EGameEngine::GetGameEngine()->GetWindow().lock();
    if (!window) {
        EDebug::Log("Window could not be locked.\n");
        return;
    }

    glm::vec2 windowSize = window->GetCurrentSize();
    UpdateTransform(windowSize);
}
