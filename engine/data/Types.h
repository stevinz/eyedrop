//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 04 2021
//
//
#ifndef DR_ENGINE_DATA_TYPES_H
#define DR_ENGINE_DATA_TYPES_H

#include <bitset>
#include <cstdint>

// Global Enum Constants
#define DROP_VERSION         2000                                   // Drop Creator version number, important for file loading / saving

#define MAX_FILE_SIZE      (512 * 512)                              // Used for filebuffers with sokol_fetch
#define INVALID_IMAGE          -1                                   // Used to identify images that have been initialized, but not loaded yet

#define KEY_NONE               -1                                   // Value that represents no item selected
#define KEY_START               1                                   // Starting value for key generators

#define MAX_ENTITIES        10000                                   // Total of number of entities allowed for now
#define MAX_COMPONENTS         32                                   // Current maximum number of compoenents (uint_8), used for sizing Signature


// ECS System
using EntityID =            std::uint32_t;                          // Unique Entity ID Key
using ComponentID =         std::uint8_t;                           // Identifier for Components
using Archetype =           std::bitset<MAX_COMPONENTS>;            // A bitset to represent a collection of Components (formerly Signature)
using ArrayIndex = 		    size_t;                                 // For referencing Array subscript values
using HashID =              size_t;                                 // This comes from typeid(OBJECT).hash_code() ... identical to Reflect.h's 'TypeHash'


//####################################################################################
//##    Used to track what types Properties of Components are, stored in DrReflect meta data
//##    !!!!! Numbers are SAVE FILE CRITICAL and CANNOT BE CHANGED once set
//##
//##    !!!!! NEXT AVAILABLE NUMBER:    35
//##
//############################
enum Property_Type {
    PROPERTY_TYPE_UNKNOWN           =  0,   // unknown!!        Uh-oh!

    PROPERTY_TYPE_BOOL              =  1,   // bool             True or False                                           (false == 0, true != 0)
    PROPERTY_TYPE_BOOL_INT          =  2,   // vector<int>      True or False, when True shows int spinbox              sizeof 6 - bool, value, min, max, step size
    PROPERTY_TYPE_BOOL_DOUBLE       =  3,   // vector<double>   True or False, when True shows double spinbox           sizeof 6 - bool, value, min, max, step size
    PROPERTY_TYPE_BOOL_COLOR        =  4,   // vector<uint>     True or False, when True shows color                    sizeof 2 - bool, color (as unsigned int)

    PROPERTY_TYPE_INT               =  5,   // int              Integer
    PROPERTY_TYPE_INT_POSITIVE      =  6,   // int              Integer >= 0
    PROPERTY_TYPE_INT_RANGED        =  7,   // vector<int>      Integer >= min and <= max                               sizeof 4 - value, min, max, step size
    PROPERTY_TYPE_INT_SLIDER        =  8,   // vector<int>      Decimal >= min and <= max, with a slider                sizeof 4 - value, min, max, step size

    PROPERTY_TYPE_DOUBLE            =  9,   // double           Decimal
    PROPERTY_TYPE_DOUBLE_POSITIVE   = 10,   // double           Decimal >= 0.0
    PROPERTY_TYPE_DOUBLE_RANGED     = 11,   // vector<double>   Decimal >= min and <= max                               sizeof 4 - value, min, max, step size
    PROPERTY_TYPE_DOUBLE_SLIDER     = 12,   // vector<double>   Decimal >= min and <= max, with a slider                sizeof 4 - value, min, max, step size
    PROPERTY_TYPE_DOUBLE_ANGLE      = 13,   // double           Decimal for showing degrees, degree symbol in spinbox
    PROPERTY_TYPE_DOUBLE_PERCENT    = 14,   // double           Decimal 0.0 to 100.0, percent symbol with a slider

    PROPERTY_TYPE_POINT             = 15,   // vector<int>      Integer pair x and y                                    sizeof 2 - x, y
    PROPERTY_TYPE_POINTF            = 16,   // vector<double>   Decimal pair x and y                                    sizeof 2 - x, y
    PROPERTY_TYPE_POINT3D           = 17,   // vector<double>   Decimal point trio, x, y, z                             sizeof 3 - doubles: x, y, z
    PROPERTY_TYPE_VARIABLE2D        = 18,   // vector<double>   Decimal point pair, number followed by a +/- number     sizeof 3 - doubles: x, y, variation
    PROPERTY_TYPE_VARIABLE3D        = 19,   // vector<double>   Decimal point trio, number followed by a +/- number     sizeof 4 - doubles: x, y, z, variation

    PROPERTY_TYPE_POSITIONF         = 20,   // vector<double>   Decimal pair x and y, used for object positions         sizeof 2 - x, y
    PROPERTY_TYPE_SCALEF            = 21,   // vector<double>   Decimal pair x and y, has smaller increment step        sizeof 2 - x, y
    PROPERTY_TYPE_POSITIVE_SCALEF   = 22,   // vector<double>   Decimal pair x and y, both numbers are >= 0.0           sizeof 2 - x, y
    PROPERTY_TYPE_SIZEF             = 23,   // vector<double>   Decimal pair w and h                                    sizeof 2 - w, h
    PROPERTY_TYPE_POSITIVE_SIZEF    = 24,   // vector<double>   Decimal pair w and h, both numbers are >= 0.0           sizeof 2 - w, h   

    PROPERTY_TYPE_STRING            = 25,   // string           Uses LineEdit
    PROPERTY_TYPE_TEXTBOX           = 26,   // string           Uses TextEdit for multi-line text input

    PROPERTY_TYPE_LIST              = 27,   // int              Index of a particular enum list
    PROPERTY_TYPE_ARRAY_INT         = 28,   // vector<int>      Array of ints
    PROPERTY_TYPE_ARRAY_DOUBLE      = 29,   // vector<double>   Array of double
    PROPERTY_TYPE_ARRAY_STRING      = 30,   // vector<string>   Array of strings

    PROPERTY_TYPE_COLOR             = 31,   // unsigned int     For colors (compatible with DrColor.rgba())
    PROPERTY_TYPE_IMAGE             = 32,   // int              Index of a particular DrAnimation Asset within the Project

    // Future: 
    PROPERTY_TYPE_COLLISION         = 33,   // ?????            Maybe vector<points> (for Collision Shapes)
    PROPERTY_TYPE_ICON              = 34,   // ?????            Maybe string
};


//####################################################################################
//##
//##    Main Types of Entities possible in Project
//##        - All Entities inherit DrEntity which contain DrComponents, which are displayed in the Inspector
//##
//############################
enum Asset_Type {    
    // Misc Types
    ASSET_TYPE_NOT_FOUND = 0,               // For passing a value in functions that represents No Type Selected / Found, !!!!! #NOTE: Keep as zero

    // Shared Types
    ASSET_TYPE_PREFAB,                      // Keeps information about an Object so we can easily make a copy
    ASSET_TYPE_IMAGE,                       // Images for use in Project

    // Game Types
    ASSET_TYPE_WORLD,                       // Describes a World (physics, lighting, etc), also holds a colleciton of Scenes
    ASSET_TYPE_SCENE,                       // Holds a collection of Entities within an entity component system
};


// ################## Project Mapped Types ####################
enum Scene_Type {
    SCENE_TYPE_PHYSICS_2D,
    SCENE_TYPE_PHYSICS_3D,
    SCENE_TYPE_UI,
};


enum Component_Icon {
    COMPONENT_ICON_NONE,
    COMPONENT_ICON_WORLD_GRAPH,
};


#endif  // DR_ENGINE_DATA_TYPES_H

