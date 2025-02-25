/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#ifndef DR_RENDER_CONTEXT_H
#define DR_RENDER_CONTEXT_H

// Includes
#include "3rd_party/sokol/sokol_gfx.h"
#include "engine/app/image/Color.h"

// Shaders
#include "engine/scene3d/shaders/BasicShader.glsl.h"


//####################################################################################
//##    DrRenderContext
//##        Holds rendering / pipeline / bindings info
//############################
class DrRenderContext
{
public:
    // Constructor / Destructor
    DrRenderContext(DrColor color);
    ~DrRenderContext() { }

    // #################### VARIABLES ####################
public:
    // Render Context Variables
    sg_pass_action      pass_action     {};
    sg_pipeline         pipeline        {};         // Shader... Pipeline holds shader, vertex shader attribute type, primitive type, index type, cull mode, depth info, blend mode
    sg_bindings         bindings        {};         // Mesh...   Bindings hold vertex buffers, index buffers, and fragment shader images


    // #################### INTERNAL FUNCTIONS ####################
public:
    // Local Variable Functions


};

#endif  // DR_RENDER_CONTEXT_H

