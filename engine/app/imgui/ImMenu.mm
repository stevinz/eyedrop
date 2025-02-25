//####################################################################################
//
// Description:     ImMenu, ImGui to MacOS Main Menu Bar Wrapper
// Author:          Stephens Nunnally and Scidian Studios
// License:         Distributed under the MIT License
// Source(s):       https://github.com/stevinz/immenu
// Original Idea:   https://github.com/JamesBoer/ImFrame
//
// Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// Copyright (c) 2021 James Boer
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//####################################################################################
// Imports
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

// Includes
#include <vector>
#include "engine/app/image/Image.h"
#include "ImMenu.h"

// Constants
#define MENU_TAG_START          1                                                   // Starting value of unique menu tags

// Free Function
#if __has_feature(objc_arc)
    #define _IMMENU_OBJC_RELEASE(obj) { obj = nil; }
#else
    #define _IMMENU_OBJC_RELEASE(obj) { [obj release]; obj = nil; }
#endif


//####################################################################################
//##    Click Handler
//####################################################################################
namespace ImMenu {
    static NSInteger s_selected_tag_id = -1;
}

@interface MenuItemHandler : NSObject
-(void) OnClick: (id) sender;
@end

@implementation MenuItemHandler
-(void) OnClick: (id) sender {
    NSMenuItem* menu_item = sender;
    ImMenu::s_selected_tag_id = menu_item.tag;
}
@end


//####################################################################################
//##    ImMenu
//##        ImGui wrapper for OS specific functions
//####################################################################################
namespace ImMenu {


//####################################################################################
//##    Local Static Variables
//####################################################################################
// Menu Variables
static std::vector<NSMenu*>     s_menu_stack = { };
static NSInteger                s_item_index = 0;

// Click Handler
static MenuItemHandler*         s_menu_handler;
static NSInteger                s_current_tag_id = MENU_TAG_START;

// Flags
static bool                     s_build_menu = false;
static bool                     s_need_clear = false;


//####################################################################################
//##    Init
//####################################################################################
// Init Mac Main Menu Bar, add "Quit" item
void osxMenuInitialize(const char* app_name) {
    // Text to use for "Quit" menu item
    NSString* quit_title = [@"Quit " stringByAppendingString:[NSString stringWithUTF8String:app_name]];

    // Create new MacOS main menu bar
    NSMenu* menu_bar = [[NSMenu alloc] init];
        // First item to be added to menu bar, "App Name"
        NSMenuItem* app_menu_item = [[NSMenuItem alloc] init];
            // Sub Menu to be attached to "App Name" menu item
            NSMenu* app_menu = [[NSMenu alloc] init];
                // "Quit" menu item to be added to "App Name" sub menu
                NSMenuItem* quit_menu_item = [[NSMenuItem alloc]
                    initWithTitle:quit_title
                    action:@selector(terminate:)
                    keyEquivalent:@"q"];
            [app_menu addItem:quit_menu_item];
        app_menu_item.submenu = app_menu;
    [menu_bar addItem:app_menu_item];

    // Init Menu
    NSApp.mainMenu = menu_bar;                                                      // Apply menu bar to our Application
    s_menu_stack.push_back(menu_bar);                                               // Save reference to menu bar
    s_menu_handler = [[MenuItemHandler alloc] init];                                // Initialize click handler
}

// Called when App is closing to clean up menu
void osxMenuShutDown() {
    _IMMENU_OBJC_RELEASE(s_menu_handler);
    _IMMENU_OBJC_RELEASE(s_menu_stack.front());
}


//####################################################################################
//##    Begin / End Menu Bar
//####################################################################################
// Call before adding menus to Main Menu Bar
bool osxBeginMainMenuBar(bool first_call) {
    // Clear menus if flag was set, preserve the first menu ("AppName")
    if (first_call && s_need_clear) {
        while (s_menu_stack.front().itemArray.count > 1) {
            [s_menu_stack.front() removeItemAtIndex:1];
        }
        s_current_tag_id = MENU_TAG_START;                                          // Reset unique tag ids
        s_need_clear = false;                                                       // Mark menu as being cleared
    }
    return true;
}

void osxEndMainMenuBar() {
}


//####################################################################################
//##    Begin / End Menu
//####################################################################################
bool osxBeginMenu(const char* label, bool enabled) {
    // Label as NSString
    NSString* title = [NSString stringWithUTF8String:label];

    // Check if menu exists
    NSInteger s_menu_index = [s_menu_stack.back() indexOfItemWithTitle:title];

    // Does exist, do not build
    if (s_menu_index != -1) {
        s_build_menu = false;

    // Does not exist, build now
    } else {
        s_build_menu = true;

        // New submenus consist of both a new menu and a new menu_item to trigger it
        NSMenu* new_menu = [[NSMenu alloc] init];
            new_menu.autoenablesItems = false;
            new_menu.title = title;
        NSMenuItem* menu_item = [[NSMenuItem alloc] init];
            menu_item.title = title;
        [menu_item setSubmenu:new_menu];

        // Add item to menu bar
        s_menu_index = [s_menu_stack.back() numberOfItems];
        [s_menu_stack.back() insertItem:menu_item atIndex:s_menu_index];
    }

    // Update 'enabled' property
    NSMenuItem* menu_bar_item = [s_menu_stack.back() itemAtIndex:s_menu_index];
    if (menu_bar_item) {
        menu_bar_item.enabled = enabled;

        // Push new menu onto stack
        s_menu_stack.push_back(menu_bar_item.submenu);
    }

    // Reset item index
    s_item_index = 0;

    // Return true so clicks can be handled by any clicked NSMenuItem
    return true;
}

void osxEndMenu() {
    s_menu_stack.pop_back();                                                        // Go back to previous menu
    s_item_index = [s_menu_stack.back() numberOfItems] - 1;                         // Reset next item index to count of previous menu

    // If back to Main Menu Bar, mark menus built for now
    if (s_menu_stack.size() == 1) {
        s_build_menu = false;
    }
}


//####################################################################################
//##    Menu Items
//####################################################################################
bool osxMenuItem(const char* label, const char* shortcut, bool selected, bool enabled, DrImage* image) {
    // Label as NSString
    NSString* title = [NSString stringWithUTF8String:label];

    // Build if in building pass
    if (s_build_menu) {
        // Create new menu item
        NSMenuItem* menu_item = [[NSMenuItem alloc] init];
            menu_item.title = title;
            menu_item.action = @selector(OnClick:);
            menu_item.target = s_menu_handler;
        [menu_item setTag:s_current_tag_id];
        ++s_current_tag_id;
        if (shortcut) menu_item.keyEquivalent = [NSString stringWithUTF8String:shortcut];

        // Set menu item icon
        if (image != nullptr) {
            ImageDescriptor icon_desc { };
                icon_desc.width =   image->bitmap().width;
                icon_desc.height =  image->bitmap().height;
                icon_desc.ptr =    &image->bitmap().data[0];
                icon_desc.size =    image->bitmap().data.size();
            NSImage* nsimage = (NSImage*)osxCreateImage(&icon_desc, s_menu_stack.back().size.height, s_menu_stack.back().size.height);
            [ menu_item setImage:nsimage ];
        }

        // Add Item to Menu
        [s_menu_stack.back() addItem:menu_item];
    }

    // Check for title change, set 'enable' and 'selected' properties
    NSMenuItem* menu_item = [s_menu_stack.back() itemAtIndex:s_item_index];
    if (menu_item) {
        // Check if title has changed and menus need to be rebuilt
        if (s_build_menu == false) {
            if (![title isEqualToString:menu_item.title]) {
                s_need_clear = true;
            }
        }

        // Update 'enable' and 'selected' properties
        menu_item.enabled = enabled;
        menu_item.state = selected ? NSControlStateValueOn : NSControlStateValueOff;
    }

    // If enabled, see if it was clicked
    if (enabled) {
        // Compare handler tag to current item tag to see if has been clicked
        if ([menu_item tag] == s_selected_tag_id) {
            // Reset handler tag and return true to signify menu item has been clicked
            s_selected_tag_id = -1;
            return true;
        }
    }

    s_item_index++;
    return false;
}

bool osxMenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled, DrImage* image) {
    if (osxMenuItem(label, shortcut, p_selected ? *p_selected : false, enabled, image)) {
        if (p_selected) *p_selected = !(*p_selected);
        return true;
    }
    return false;
}

void osxSeparator() {
    if (s_build_menu) {
        NSMenuItem* seperator_item = [NSMenuItem separatorItem];
        [s_menu_stack.back() addItem:seperator_item];
    }
    s_item_index++;
}


//####################################################################################
//##    Builder Functions
//####################################################################################
void* osxCreateImage(const ImageDescriptor* image_desc, const int width, const int height) {
    CGColorSpaceRef cg_color_space = CGColorSpaceCreateDeviceRGB();
    CFDataRef cf_data = CFDataCreate(kCFAllocatorDefault, (const UInt8*)image_desc->ptr, (CFIndex)image_desc->size);
    CGDataProviderRef cg_data_provider = CGDataProviderCreateWithCFData(cf_data);
    CGImageRef cg_img = CGImageCreate(
        (size_t)image_desc->width,                          // width
        (size_t)image_desc->height,                         // height
        8,                                                  // bitsPerComponent
        32,                                                 // bitsPerPixel
        (size_t)image_desc->width * 4,                      // bytesPerRow
        cg_color_space,                                     // space
        kCGImageAlphaLast | kCGImageByteOrderDefault,       // bitmapInfo
        cg_data_provider,                                   // provider
        NULL,                                               // decode
        false,                                              // shouldInterpolate
        kCGRenderingIntentDefault);
    CFRelease(cf_data);
    CGDataProviderRelease(cg_data_provider);
    CGColorSpaceRelease(cg_color_space);

    NSSize desired_size;
        desired_size.width =  width;
        desired_size.height = height;
    NSImage* ns_image = [[NSImage alloc] initWithCGImage:cg_img size:desired_size];
    CGImageRelease(cg_img);
    return ns_image;
}


}   // End ImMenu
