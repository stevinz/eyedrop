/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/
#include "3rd_party/sokol/sokol_app.h"

static const char* eventtype_to_str(sapp_event_type event_type) {
    switch (event_type) {
        case SAPP_EVENTTYPE_INVALID:            return "INVALID";
        case SAPP_EVENTTYPE_KEY_DOWN:           return "KEY_DOWN";
        case SAPP_EVENTTYPE_KEY_UP:             return "KEY_UP";
        case SAPP_EVENTTYPE_CHAR:               return "CHAR";
        case SAPP_EVENTTYPE_MOUSE_DOWN:         return "MOUSE_DOWN";
        case SAPP_EVENTTYPE_MOUSE_UP:           return "MOUSE_UP";
        case SAPP_EVENTTYPE_MOUSE_SCROLL:       return "MOUSE_SCROLL";
        case SAPP_EVENTTYPE_MOUSE_MOVE:         return "MOUSE_MOVE";
        case SAPP_EVENTTYPE_MOUSE_ENTER:        return "MOUSE_ENTER";
        case SAPP_EVENTTYPE_MOUSE_LEAVE:        return "MOUSE_LEAVE";
        case SAPP_EVENTTYPE_TOUCHES_BEGAN:      return "TOUCHES_BEGAN";
        case SAPP_EVENTTYPE_TOUCHES_MOVED:      return "TOUCHES_MOVED";
        case SAPP_EVENTTYPE_TOUCHES_ENDED:      return "TOUCHES_ENDED";
        case SAPP_EVENTTYPE_TOUCHES_CANCELLED:  return "TOUCHES_CANCELLED";
        case SAPP_EVENTTYPE_RESIZED:            return "RESIZED";
        case SAPP_EVENTTYPE_ICONIFIED:          return "ICONIFIED";
        case SAPP_EVENTTYPE_RESTORED:           return "RESTORED";
        case SAPP_EVENTTYPE_SUSPENDED:          return "SUSPENDED";
        case SAPP_EVENTTYPE_RESUMED:            return "RESUMED";
        case SAPP_EVENTTYPE_UPDATE_CURSOR:      return "UPDATE_CURSOR";
        case SAPP_EVENTTYPE_QUIT_REQUESTED:     return "QUIT_REQUESTED";
        case SAPP_EVENTTYPE_CLIPBOARD_PASTED:   return "CLIPBOARD_PASTED";
        case SAPP_EVENTTYPE_FILES_DROPPED:      return "FILES_DROPPED";
        default:                                return "EVENT_UNKNOWN";
    }
}

static const char* keycode_to_str(sapp_keycode keycode) {
    switch (keycode) {
        case SAPP_KEYCODE_INVALID:          return "INVALID";
        case SAPP_KEYCODE_SPACE:            return "SPACE";
        case SAPP_KEYCODE_APOSTROPHE:       return "APOSTROPHE";
        case SAPP_KEYCODE_COMMA:            return "COMMA";
        case SAPP_KEYCODE_MINUS:            return "MINUS";
        case SAPP_KEYCODE_PERIOD:           return "PERIOD";
        case SAPP_KEYCODE_SLASH:            return "SLASH";
        case SAPP_KEYCODE_0:                return "0";
        case SAPP_KEYCODE_1:                return "1";
        case SAPP_KEYCODE_2:                return "2";
        case SAPP_KEYCODE_3:                return "3";
        case SAPP_KEYCODE_4:                return "4";
        case SAPP_KEYCODE_5:                return "5";
        case SAPP_KEYCODE_6:                return "6";
        case SAPP_KEYCODE_7:                return "7";
        case SAPP_KEYCODE_8:                return "8";
        case SAPP_KEYCODE_9:                return "9";
        case SAPP_KEYCODE_SEMICOLON:        return "SEMICOLON";
        case SAPP_KEYCODE_EQUAL:            return "EQUAL";
        case SAPP_KEYCODE_A:                return "A";
        case SAPP_KEYCODE_B:                return "B";
        case SAPP_KEYCODE_C:                return "C";
        case SAPP_KEYCODE_D:                return "D";
        case SAPP_KEYCODE_E:                return "E";
        case SAPP_KEYCODE_F:                return "F";
        case SAPP_KEYCODE_G:                return "G";
        case SAPP_KEYCODE_H:                return "H";
        case SAPP_KEYCODE_I:                return "I";
        case SAPP_KEYCODE_J:                return "J";
        case SAPP_KEYCODE_K:                return "K";
        case SAPP_KEYCODE_L:                return "L";
        case SAPP_KEYCODE_M:                return "M";
        case SAPP_KEYCODE_N:                return "N";
        case SAPP_KEYCODE_O:                return "O";
        case SAPP_KEYCODE_P:                return "P";
        case SAPP_KEYCODE_Q:                return "Q";
        case SAPP_KEYCODE_R:                return "R";
        case SAPP_KEYCODE_S:                return "S";
        case SAPP_KEYCODE_T:                return "T";
        case SAPP_KEYCODE_U:                return "U";
        case SAPP_KEYCODE_V:                return "V";
        case SAPP_KEYCODE_W:                return "W";
        case SAPP_KEYCODE_X:                return "X";
        case SAPP_KEYCODE_Y:                return "Y";
        case SAPP_KEYCODE_Z:                return "Z";
        case SAPP_KEYCODE_LEFT_BRACKET:     return "LEFT_BRACKET";
        case SAPP_KEYCODE_BACKSLASH:        return "BACKSLASH";
        case SAPP_KEYCODE_RIGHT_BRACKET:    return "RIGHT_BRACKET";
        case SAPP_KEYCODE_GRAVE_ACCENT:     return "ACCENT";
        case SAPP_KEYCODE_WORLD_1:          return "WORLD_1";
        case SAPP_KEYCODE_WORLD_2:          return "WORLD_2";
        case SAPP_KEYCODE_ESCAPE:           return "ESCAPE";
        case SAPP_KEYCODE_ENTER:            return "ENTER";
        case SAPP_KEYCODE_TAB:              return "TAB";
        case SAPP_KEYCODE_BACKSPACE:        return "BACKSPACE";
        case SAPP_KEYCODE_INSERT:           return "INSERT";
        case SAPP_KEYCODE_DELETE:           return "DELETE";
        case SAPP_KEYCODE_RIGHT:            return "RIGHT";
        case SAPP_KEYCODE_LEFT:             return "LEFT";
        case SAPP_KEYCODE_DOWN:             return "DOWN";
        case SAPP_KEYCODE_UP:               return "UP";
        case SAPP_KEYCODE_PAGE_UP:          return "PAGE_UP";
        case SAPP_KEYCODE_PAGE_DOWN:        return "PAGE_DOWN";
        case SAPP_KEYCODE_HOME:             return "HOME";
        case SAPP_KEYCODE_END:              return "END";
        case SAPP_KEYCODE_CAPS_LOCK:        return "CAPS_LOCK";
        case SAPP_KEYCODE_SCROLL_LOCK:      return "SCROLL_LOCK";
        case SAPP_KEYCODE_NUM_LOCK:         return "NUM_LOCK";
        case SAPP_KEYCODE_PRINT_SCREEN:     return "PRINT_SCREEN";
        case SAPP_KEYCODE_PAUSE:            return "PAUSE";
        case SAPP_KEYCODE_F1:               return "F1";
        case SAPP_KEYCODE_F2:               return "F2";
        case SAPP_KEYCODE_F3:               return "F3";
        case SAPP_KEYCODE_F4:               return "F4";
        case SAPP_KEYCODE_F5:               return "F5";
        case SAPP_KEYCODE_F6:               return "F6";
        case SAPP_KEYCODE_F7:               return "F7";
        case SAPP_KEYCODE_F8:               return "F8";
        case SAPP_KEYCODE_F9:               return "F9";
        case SAPP_KEYCODE_F10:              return "F10";
        case SAPP_KEYCODE_F11:              return "F11";
        case SAPP_KEYCODE_F12:              return "F12";
        case SAPP_KEYCODE_F13:              return "F13";
        case SAPP_KEYCODE_F14:              return "F14";
        case SAPP_KEYCODE_F15:              return "F15";
        case SAPP_KEYCODE_F16:              return "F16";
        case SAPP_KEYCODE_F17:              return "F17";
        case SAPP_KEYCODE_F18:              return "F18";
        case SAPP_KEYCODE_F19:              return "F19";
        case SAPP_KEYCODE_F20:              return "F20";
        case SAPP_KEYCODE_F21:              return "F21";
        case SAPP_KEYCODE_F22:              return "F22";
        case SAPP_KEYCODE_F23:              return "F23";
        case SAPP_KEYCODE_F24:              return "F24";
        case SAPP_KEYCODE_F25:              return "F25";
        case SAPP_KEYCODE_KP_0:             return "KP_0";
        case SAPP_KEYCODE_KP_1:             return "KP_1";
        case SAPP_KEYCODE_KP_2:             return "KP_2";
        case SAPP_KEYCODE_KP_3:             return "KP_3";
        case SAPP_KEYCODE_KP_4:             return "KP_4";
        case SAPP_KEYCODE_KP_5:             return "KP_5";
        case SAPP_KEYCODE_KP_6:             return "KP_6";
        case SAPP_KEYCODE_KP_7:             return "KP_7";
        case SAPP_KEYCODE_KP_8:             return "KP_8";
        case SAPP_KEYCODE_KP_9:             return "KP_9";
        case SAPP_KEYCODE_KP_DECIMAL:       return "KP_DECIMAL";
        case SAPP_KEYCODE_KP_DIVIDE:        return "KP_DIVIDE";
        case SAPP_KEYCODE_KP_MULTIPLY:      return "KP_MULTIPLY";
        case SAPP_KEYCODE_KP_SUBTRACT:      return "KP_SUBTRACT";
        case SAPP_KEYCODE_KP_ADD:           return "KP_ADD";
        case SAPP_KEYCODE_KP_ENTER:         return "KP_ENTER";
        case SAPP_KEYCODE_KP_EQUAL:         return "KP_EQUAL";
        case SAPP_KEYCODE_LEFT_SHIFT:       return "LEFT_SHIFT";
        case SAPP_KEYCODE_LEFT_CONTROL:     return "LEFT_CONTROL";
        case SAPP_KEYCODE_LEFT_ALT:         return "LEFT_ALT";
        case SAPP_KEYCODE_LEFT_SUPER:       return "LEFT_SUPER";
        case SAPP_KEYCODE_RIGHT_SHIFT:      return "RIGHT_SHIFT";
        case SAPP_KEYCODE_RIGHT_CONTROL:    return "RIGHT_CONTROL";
        case SAPP_KEYCODE_RIGHT_ALT:        return "RIGHT_ALT";
        case SAPP_KEYCODE_RIGHT_SUPER:      return "RIGHT_SUPER";
        case SAPP_KEYCODE_MENU:             return "MENU";
        default:                            return "KEYCODE_UNKNOWN?";
    }
}

static const char* button_to_str(sapp_mousebutton button) {
    switch (button) {
        case SAPP_MOUSEBUTTON_INVALID:      return "INVALID";
        case SAPP_MOUSEBUTTON_LEFT:         return "LEFT";
        case SAPP_MOUSEBUTTON_RIGHT:        return "RIGHT";
        case SAPP_MOUSEBUTTON_MIDDLE:       return "MIDDLE";
        default:                            return "MOUSE_UNKNOWN";
    }
}



