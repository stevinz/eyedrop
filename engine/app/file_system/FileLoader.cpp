//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Fri Mar 26 2021
//
//
#include "3rd_party/stb/stb_rect_pack.h"
#include "engine/app/image/Image.h"
#include "engine/app/image/Bitmap.h"
#include "engine/app/App.h"
#include "FileLoader.h"


//####################################################################################
//##    Constructor / Destructor
//####################################################################################
DrFileLoader::DrFileLoader() {
    addAtlas();
}


//####################################################################################
//##    Sokol Helper
//####################################################################################
void DrFileLoader::initializeSgImageDesc(const int& width, const int& height, sg_image_desc& image_desc) {
    image_desc.width =        width;
    image_desc.height =       height;
    image_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
    image_desc.wrap_u =       SG_WRAP_CLAMP_TO_EDGE;        // also available: SG_WRAP_MIRRORED_REPEAT
    image_desc.wrap_v =       SG_WRAP_CLAMP_TO_EDGE;        // also available: SG_WRAP_MIRRORED_REPEAT
    image_desc.min_filter =   SG_FILTER_LINEAR;
    image_desc.mag_filter =   SG_FILTER_LINEAR;
}


//####################################################################################
//##    Image Fetching
//####################################################################################
// Adds a new atlas into the App, inits onto GPU
void DrFileLoader::addAtlas() {
    m_rect_packs.push_back(std::make_shared<stbrp_context>());
    m_atlases.push_back(std::make_shared<DrBitmap>(MAX_TEXTURE_SIZE, MAX_TEXTURE_SIZE, DROP_BITMAP_FORMAT_ARGB));

}

// Adds image to stack of images to be loaded
void DrFileLoader::addImageToFetch(std::shared_ptr<DrImage>& load_to, std::string image_file, ImageFunction callback, bool outline) {
    ImageData img_data = { load_to, image_file, callback, outline };
    m_load_image_stack.push_back(img_data);
}

// Initiates fetch of next image on the load stack
void DrFileLoader::fetchNextImage() {
    if (m_loading_image || m_load_image_stack.size() < 1) return;
    m_loading_image = true;

    sfetch_request_t sokol_fetch_image { };
        sokol_fetch_image.path = m_load_image_stack[0].image_file.c_str();
        sokol_fetch_image.buffer_ptr = m_load_image_buffer;
        sokol_fetch_image.buffer_size = sizeof(m_load_image_buffer);
        sokol_fetch_image.callback = +[](const sfetch_response_t* response) {
            // Load Data from response
            DrBitmap bmp((unsigned char*)response->buffer_ptr, (int)response->fetched_size);
            assert((bmp.width <= MAX_TEXTURE_SIZE && bmp.height <= MAX_TEXTURE_SIZE) && "Image dimensions too large! Max width and height are MAX_TEXTURE_SIZE!");

            // If valid, create image on gpu
            if (response->error_code == SFETCH_ERROR_FILE_NOT_FOUND     /* '1' */) { }
            if (response->error_code == SFETCH_ERROR_BUFFER_TOO_SMALL   /* '3' */) { }

            // Attempt to create image
            g_app->fileLoader()->createImage(bmp);
        };
    sfetch_send(&sokol_fetch_image);    
}

// Creates DrImage from DrBitmap on top of image loading stack
void DrFileLoader::createImage(DrBitmap& bmp) {
    if (bmp.isValid()) {
        sg_image_desc img_desc { };
            initializeSgImageDesc(bmp.width, bmp.height, img_desc);
            img_desc.data.subimage[0][0].ptr = &bmp.data[0];
            img_desc.data.subimage[0][0].size = static_cast<size_t>(bmp.width * bmp.height * bmp.channels);
        m_load_image_stack[0].image = std::make_shared<DrImage>(m_load_image_stack[0].image_file, bmp, m_load_image_stack[0].outline);
        m_load_image_stack[0].image->setID(sg_make_image(&img_desc).id);

        // Call callback function
        if (m_load_image_stack[0].callback != NULL) {
            m_load_image_stack[0].callback(m_load_image_stack[0].image);
        }
    }

    // Remove image of list to be fetched
    m_load_image_stack.pop_front();
    m_loading_image = false;
}



