#pragma once
#include "EngineTypes.h"
#include "Math/ESTransform.h"

struct ESAddSpriteConfig {
    ESAddSpriteConfig(const ESTransform2D _transform, const EUi32 _renderOrder) {
        transform = _transform;
        renderOrder = _renderOrder;
    }

    ESAddSpriteConfig(const EString _texturePath, const ESTransform2D _transform, const EUi32 _renderOrder) {
        texturePath = _texturePath;
        transform = _transform;
        renderOrder = _renderOrder;
    }

    ESAddSpriteConfig SetRenderColor(glm::vec4 _renderColor) { renderColor = _renderColor; return *this; }
    ESAddSpriteConfig SetIsText(bool _isText) { isText = _isText; return *this; }

    ESTransform2D transform;
    EUi32 renderOrder;
    EString texturePath = "";
    bool isText = false;
    glm::vec4 renderColor = glm::vec4(1.0f);
};