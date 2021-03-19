//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#include "core/geometry/Matrix.h"
#include "core/geometry/Rect.h"
#include "core/geometry/Vec2.h"
#include "core/imaging/Bitmap.h"
#include "core/imaging/Color.h"
#include "core/imaging/Filter.h"
#include "core/Math.h"
#include "core/Random.h"
#include "engine/app/sokol/Event__strings.h"
#include "engine/app/App.h"
#include "engine/app/Image.h"
#include "engine/app/RenderContext.h"
#include "engine/data/Reflect.h"
#include "engine/scene3d/Mesh.h"
#include "ui/Dockspace.h"
#include "ui/Menu.h"
#include "ui/Toolbar.h"
#include "widgets/ThemeSelector.h"
#include "Editor.h"
#include "Types.h"


// !!!!! #TEMP
#include "engine/scene2d/components/Test1.h"
#include "engine/scene2d/components/Transform2D.h"


#include <iostream>




//####################################################################################
//##    Program Start
//####################################################################################
int main(int argc, char* argv[]) {

    // Turn on reflection
    InitializeReflection();

    // Create app and run
    DrEditor* editor = new DrEditor("Test Editor", DrColor(28, 30, 29), 1750, 1000);
    editor->run();

}


//####################################################################################
//##    On Create
//####################################################################################
void DrEditor::onCreate() { 

    // #################### TESTING ####################
    // ECS Construction
    ecs()->registerComponent<Test1>();
    ecs()->registerComponent<Transform2D>();

    // Create entity
    int entity = ecs()->createEntity();
    Transform2D et { };
        et.position =   std::vector<double>({1.0, 2.0, 3.0});
        et.rotation =   DrVec3(4.0, 5.0, 6.0);
        et.scale_xyz =  std::vector<double>({7.0, 8.0, 9.0});
    ecs()->addComponent(entity, et);

    // Component Iterate
    Archetype entity_type = ecs()->getEntityType(entity);
    std::cout << "Entity #" << entity << " has the following components:" << std::endl;
    for (ComponentID id = 0; id < MAX_COMPONENTS; ++id) {
        std::cout << "Component #" << std::to_string(id) << "? ";
        if (entity_type.test(id)) {
            // Get Test
            HashID component_hash_id = ecs()->getComponentHashID(id);
            void*  component = ecs()->getData(id, entity);
            std::vector<double> pos = GetProperty<std::vector<double>>(component, component_hash_id, "position");

            std::cout << "YES" << ", Component Name: " << GetComponentData(ecs()->getComponentHashID(id)).name << std::endl;
            std::cout << "     First Variable: " << GetPropertyData(ecs()->getComponentHashID(id), 0).name;
            std::cout << ", Value - X: " << pos[0] << ", Y: " << pos[1] << ", Z: " << pos[2];
            std::cout << std::endl;

            // Set Test
            pos = { 23.0, 43.2, 99.0 };
            SetProperty(component, component_hash_id, "position", pos);

            // Check Set
            pos = GetProperty<std::vector<double>>(component, component_hash_id, 0);
            std::cout << "After setting - X: " << pos[0] << ", Y: " << pos[1] << ", Z: " << pos[2] << std::endl;
        } else {
            std::cout << "---" << std::endl;
        }
    }

    // Meta Data
    std::cout << GetComponentData<Transform2D>().name << std::endl;
    std::cout << GetComponentData<Transform2D>().description << std::endl;
    std::cout << GetComponentData(et).name << std::endl;
    std::cout << GetComponentData(et).description << std::endl;

    std::cout << "Prop Name:   " << GetPropertyData<Transform2D>(0).name << std::endl;
    std::cout << "Prop Title:  " << GetPropertyData<Transform2D>(0).title << std::endl;
    std::cout << "Prop About:  " << GetPropertyData<Transform2D>(0).description << std::endl;
    std::cout << "Prop Offset: " << GetPropertyData(et, 0).offset << std::endl;
    
    std::cout << "Prop Name:   " << GetPropertyData<Transform2D>("rotation").name << std::endl;
    std::cout << "Prop Title:  " << GetPropertyData<Transform2D>(1).title << std::endl;
    std::cout << "Prop About:  " << GetPropertyData<Transform2D>(1).description << std::endl;
    std::cout << "Prop Offset: " << GetPropertyData(et, "rotation").offset << std::endl;

    std::cout << "Prop Name:   " << GetPropertyData<Transform2D>("scale_xyz").name << std::endl;
    std::cout << "Prop Title:  " << GetPropertyData<Transform2D>(2).title << std::endl;
    std::cout << "Prop About:  " << GetPropertyData(et, 2).description << std::endl;
    std::cout << "Prop Offset: " << GetPropertyData(et, "scale_xyz").offset << std::endl;

    // Test GetProperty by Index
    DrVec3 rotation = GetProperty<DrVec3>(et, 1);
    std::cout << "Rotation X: " << rotation.x << ", Rotation Y: " << rotation.y << ", Rotation Z: " << rotation.z << std::endl;
    
    // Test GetProperty by Name
    std::vector<double> position = GetProperty<std::vector<double>>(et, "position");
    std::cout << "Position X: " << position[0] << ", Position Y: " << position[1] << ", Position Z: " << position[2] << std::endl;

    // Test SetProperty by Index
    Test1 t{};
        t.test1 = 5;
    std::cout << "Test1 variable test1 is currently: " << t.test1 << std::endl;
    std::cout << "Setting Now..." << std::endl;
    SetProperty(t, 0, int(189));
    std::cout << "Test1 variable test1 is now: " << t.test1 << std::endl;

    // Test SetProperty by Name
    position = { 56.0, 58.5, 60.2 };
    SetProperty(et, "position", position);
    std::cout << "Transform2D instance - Position X: " << et.position[0] << ", Position Y: " << et.position[1] << ", Position Z: " << et.position[2] << std::endl;


    // #############################################

    for (int i = 0; i < EDITOR_IMAGE_TOTAL; i++) {
        images[i] = nullptr;
    }

    // For Reference:
    // (ImTextureID)(uintptr_t) sg_make_image(&img_desc).id;
    // std::string image_file = m_app_directory + "assets/blob.png";
    
    sfetch_request_t sokol_fetch_image { };
        sokol_fetch_image.path = (appDirectory() + "assets/toolbar_icons/toolbar_world_graph.png").c_str();
        sokol_fetch_image.buffer_ptr = m_file_buffer2;
        sokol_fetch_image.buffer_size = sizeof(m_file_buffer2);
        sokol_fetch_image.callback = +[](const sfetch_response_t* response) {
            int png_width, png_height, num_channels;
            const int desired_channels = 4;
            stbi_uc* pixels = stbi_load_from_memory((stbi_uc *)response->buffer_ptr, (int)response->fetched_size,
                                                    &png_width, &png_height, &num_channels, desired_channels);
            // Stb Load Succeeded
            if (pixels) {
                sg_image_desc img_desc { };
                    img_desc.width =        png_width;
                    img_desc.height =       png_height;
                    img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
                    img_desc.wrap_u =       SG_WRAP_CLAMP_TO_EDGE;
                    img_desc.wrap_v =       SG_WRAP_CLAMP_TO_EDGE;
                    img_desc.min_filter =   SG_FILTER_LINEAR;
                    img_desc.mag_filter =   SG_FILTER_LINEAR;
                    img_desc.data.subimage[0][0].ptr = pixels;
                    img_desc.data.subimage[0][0].size = static_cast<size_t>(png_width * png_height * num_channels);
                DrEditor* editor = dynamic_cast<DrEditor*>(g_app);
                editor->images[EDITOR_IMAGE_WORLD_GRAPH] = (ImTextureID)(uintptr_t) sg_make_image(&img_desc).id;
                free(pixels);
            }
        };
    sfetch_send(&sokol_fetch_image);    
}


//####################################################################################
//##    Render Update
//####################################################################################
void DrEditor::onUpdateScene() { 
        // ***** Compute model-view-projection matrix for vertex shader
    hmm_mat4 proj = HMM_Perspective(52.5f, (float)sapp_width()/(float)sapp_height(), 5.f, 20000.0f);
    hmm_mat4 view = HMM_LookAt(HMM_Vec3(0.0f, 1.5f, m_mesh->image_size * m_zoom), HMM_Vec3(0.0f, 0.0f, 0.0f), HMM_Vec3(0.0f, 1.0f, 0.0f));
    hmm_mat4 view_proj = HMM_MultiplyMat4(proj, view);

    hmm_mat4 rxm = HMM_Rotate(m_add_rotation.x, HMM_Vec3(1.0f, 0.0f, 0.0f));
    hmm_mat4 rym = HMM_Rotate(m_add_rotation.y, HMM_Vec3(0.0f, 1.0f, 0.0f));
    hmm_mat4 rotate = HMM_MultiplyMat4(rxm, rym); 
    m_model =  HMM_MultiplyMat4(rotate, m_model);
    m_total_rotation.x = EqualizeAngle0to360(m_total_rotation.x + m_add_rotation.x);
    m_total_rotation.y = EqualizeAngle0to360(m_total_rotation.y + m_add_rotation.y);
    m_add_rotation.set(0.f, 0.f);

    // Uniforms for vertex shader
    vs_params_t vs_params;
        vs_params.m =   m_model;
        vs_params.mvp = HMM_MultiplyMat4(view_proj, m_model);
    
    // Uniforms for fragment shader
    fs_params_t fs_params;
        fs_params.u_wireframe = (m_mesh->wireframe) ? 1.0f : 0.0f;

    // Check if user requested new model quality, if so recalculate
    if (m_mesh_quality != m_before_keys) {
        calculateMesh(false);
        m_before_keys = m_mesh_quality;
    }

    sg_apply_pipeline(renderContext()->pipeline);
    sg_apply_bindings(&renderContext()->bindings);
    sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, SG_RANGE(vs_params));
    sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_fs_params, SG_RANGE(fs_params));
    sg_draw(0, m_mesh->indices.size(), 1);
}

#define HZ_CORE_IMGUI_COMPONENT_VAR(func, label, code) ImGui::TextUnformatted(label); ImGui::NextColumn(); ImGui::SetNextItemWidth(-1); if(func) { code } ImGui::NextColumn();

//####################################################################################
//##    Gui Update
//####################################################################################
void DrEditor::onUpdateGUI() { 
    // Keep track of open windows / widgets
    static bool widgets[EDITOR_WIDGET_TOTAL_NUMBER];
    if (isFirstFrame()) {
        std::fill(widgets, widgets + EDITOR_WIDGET_TOTAL_NUMBER, true);
        widgets[EDITOR_WIDGET_THEME] = false;
        widgets[EDITOR_WIDGET_STYLE] = false;
        widgets[EDITOR_WIDGET_DEMO] =  false;
    }
    
    // Menu
    MainMenuUI(widgets);
    
    // Handle Dockspace
    int menu_height = 0;
    DockspaceUI(widgets, menu_height);

    // Handle Toolbar
    ToolbarUI(widgets, images, menu_height);


    // ##### Widget Windows
    ImGuiWindowFlags child_flags = 0; //ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
    
    // if (widgets[EDITOR_WIDGET_STATUS]) {
    //     ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(100, 100));
    //     ImGui::Begin("Status Bar", &widgets[EDITOR_WIDGET_STATUS], child_flags);
    //         ImGui::Text("Some status text");
    //     ImGui::End();
    //     ImGui::PopStyleVar();
    // }
    
    if (widgets[EDITOR_WIDGET_ASSETS]) {
        ImGui::Begin("Assets", &widgets[EDITOR_WIDGET_ASSETS], child_flags);
            ImGui::Text("Asset 1");
            ImGui::Text("Asset 2");
            ImGui::Text("Asset 3");
            ImGui::Text("Asset 4");
        ImGui::End();
    }
    
    if (widgets[EDITOR_WIDGET_INSPECTOR]) {
        ImGui::Begin("Property Inspector", &widgets[EDITOR_WIDGET_INSPECTOR], child_flags);
            static ImVec4 base  = ImVec4(0.000f, 0.750f, 0.720f, 1.0f);
            static ImVec4 base2 = ImVec4(0.000f, 0.750f, 0.720f, 1.0f);
            static float f1 = 0.123f, f2 = 0.0f;
            static float angle = 0.0f;

            ImGui::SliderFloat("slider float", &f1, 0.0f, 1.0f, "ratio = %.3f");
            ImGui::SliderFloat("slider float (log)", &f2, -10.0f, 10.0f, "%.4f", ImGuiSliderFlags_Logarithmic);
            ImGui::SliderAngle("slider angle", &angle);
            ImGui::Text("Base: "); ImGui::SameLine(); ImGui::ColorEdit3("##Base", (float*) &base, ImGuiColorEditFlags_PickerHueWheel);

            ImGui::BeginColumns("Cols", 2);
            {
                bool flag;
	            HZ_CORE_IMGUI_COMPONENT_VAR(ImGui::ColorEdit3("##Color 2", (float*) &base2), "Color 2", int i;);
                HZ_CORE_IMGUI_COMPONENT_VAR(ImGui::Checkbox("##Fixed Aspect Ratio", &flag), "Fixed Aspect Ratio", int j;);
            }
            ImGui::EndColumns();
        ImGui::End();
    }

    // Demo Window
    if (widgets[EDITOR_WIDGET_DEMO]) {
        ImGui::ShowDemoWindow();
    }

    // Style Selector
    if (widgets[EDITOR_WIDGET_STYLE]) {
        ImGui::ShowStyleEditor();
    }   

    // Theme selector
    if (widgets[EDITOR_WIDGET_THEME] || isFirstFrame()) {
        //ImGui::SetNextWindowPos(ImVec2(100, 200));
        ImGui::SetNextWindowContentSize(ImVec2(250, 250));
        ThemeSelectorUI(widgets[EDITOR_WIDGET_THEME], child_flags, isFirstFrame());
    }
}


//####################################################################################
//##    Event Update
//####################################################################################
void DrEditor::onEvent(const sapp_event* event) {
    if ((event->type == SAPP_EVENTTYPE_KEY_DOWN) && !event->key_repeat) {
        switch (event->key_code) {
            case SAPP_KEYCODE_1: 
            case SAPP_KEYCODE_2:
            case SAPP_KEYCODE_3:
            case SAPP_KEYCODE_4:
            case SAPP_KEYCODE_5:
            case SAPP_KEYCODE_6:
            case SAPP_KEYCODE_7:
            case SAPP_KEYCODE_8:
            case SAPP_KEYCODE_9:
                m_mesh_quality = event->key_code - SAPP_KEYCODE_1;
                break;
            case SAPP_KEYCODE_R:
                m_total_rotation.set(0.f, 0.f);
                m_add_rotation.set(25.f, 25.f);
                m_model = DrMatrix::identityMatrix();
                break;
            case SAPP_KEYCODE_W:
                m_mesh->wireframe = !m_mesh->wireframe;
                m_wireframe = m_mesh->wireframe;
                break;
            default: ;
        }
                
    } else if (event->type == SAPP_EVENTTYPE_MOUSE_SCROLL) {
        m_zoom -= (event->scroll_y * 0.1f);
        m_zoom = Clamp(m_zoom, 0.5f, 5.0f);

    } else if (event->type == SAPP_EVENTTYPE_MOUSE_DOWN) {
        if (event->mouse_button == SAPP_MOUSEBUTTON_LEFT) {
            m_mouse_down.set(event->mouse_y, event->mouse_x);
            m_is_mouse_down = true;
        }
        
    } else if (event->type == SAPP_EVENTTYPE_MOUSE_UP) {
        if (event->mouse_button == SAPP_MOUSEBUTTON_LEFT) {
            m_is_mouse_down = false;
        }

    } else if (event->type == SAPP_EVENTTYPE_MOUSE_MOVE) {
        if (m_is_mouse_down) {
            float x_movement = event->mouse_y;
            float y_movement = event->mouse_x;

            if (m_mouse_down.x < x_movement) {
                m_add_rotation.x = m_rotate_speed * (x_movement - m_mouse_down.x);
            } else if (m_mouse_down.x > x_movement) {
                m_add_rotation.x = 360 - (m_rotate_speed * (m_mouse_down.x - x_movement));
            }
            
            if (m_mouse_down.y > y_movement) {
                m_add_rotation.y = 360 - (m_rotate_speed * (m_mouse_down.y - y_movement));
            } else if (m_mouse_down.y < y_movement) {
                m_add_rotation.y = m_rotate_speed * (y_movement - m_mouse_down.y);
            }

            m_mouse_down.x = x_movement;
            m_mouse_down.y = y_movement;
            m_add_rotation.x = EqualizeAngle0to360(m_add_rotation.x);
            m_add_rotation.y = EqualizeAngle0to360(m_add_rotation.y);
        }

    } else if (event->type == SAPP_EVENTTYPE_FILES_DROPPED) {
        #if defined(DROP_TARGET_HTML5)
            // on emscripten need to use the sokol-app helper function to load the file data
            sapp_html5_fetch_request sokol_fetch_request { };
                sokol_fetch_request.dropped_file_index = 0;
                sokol_fetch_request.buffer_ptr = m_file_buffer;
                sokol_fetch_request.buffer_size = sizeof(m_file_buffer);
                sokol_fetch_request.callback = +[](const sapp_html5_fetch_response* response) {
                    if (response->succeeded) {
                        g_app->initImage((stbi_uc *)response->buffer_ptr, (int)response->fetched_size);
                    } else {
                        // File too big if (response->error_code == SAPP_HTML5_FETCH_ERROR_BUFFER_TOO_SMALL), otherwise file failed to load for unknown reason
                    }
                };
            sapp_html5_fetch_dropped_file(&sokol_fetch_request);
        #else
            // native platform: use sokol-fetch to load file content
            sfetch_request_t sokol_fetch_request { };
                sokol_fetch_request.path = sapp_get_dropped_file_path(0);
                sokol_fetch_request.buffer_ptr = m_file_buffer;
                sokol_fetch_request.buffer_size = sizeof(m_file_buffer);
                sokol_fetch_request.callback = +[](const sfetch_response_t* response) {
                    if (response->fetched) {
                        g_app->initImage((stbi_uc *)response->buffer_ptr, (int)response->fetched_size);
                    } else {
                        // File too big if (response->error_code == SFETCH_ERROR_BUFFER_TOO_SMALL), otherwise file failed to load for unknown reason
                    }                   
                };
            sfetch_send(&sokol_fetch_request);
        #endif
    }
}
