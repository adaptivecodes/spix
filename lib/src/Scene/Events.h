/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Scene/Item.h>
#include <Spix/Data/Geometry.h>
#include <Spix/Data/PasteboardContent.h>
#include <Spix/Events/Identifiers.h>

#include <string>

namespace spix {

class Events {
public:
    virtual ~Events() = default;

    // TODO ADE: Rework
    virtual void swipe(Item* item, Point from, Point to, std::vector<Point> mouseOvers) = 0;
    virtual void pinch(Item* item, std::vector<std::vector<Point>> touchpoints) = 0;
    virtual void rotate(Item* item, int degree) = 0;

    virtual void tap(Item* item, Point touchPoint, int duration) = 0;

    // TODO ADE : REWORK
    /*
    virtual void touchPress(TouchSequencer* touchSequencer, Item* item, Point loc, int touchId) = 0;
    virtual void touchMove(TouchSequencer* touchSequencer, Item* item, Point loc, int touchId) = 0;
    virtual void touchRelease(TouchSequencer* touchSequencer, Item* item, Point loc, int touchId) = 0;
    virtual void touchStationary(TouchSequencer* touchSequencer, Item* item, int touchId) = 0;
*/


    virtual void mouseDown(Item* item, Point loc, MouseButton button) = 0;
    virtual void mouseUp(Item* item, Point loc, MouseButton button) = 0;
    virtual void mouseMove(Item* item, Point loc) = 0;
    virtual void stringInput(Item* item, const std::string& text) = 0;
    virtual void keyPress(Item* item, int keyCode, KeyModifier mod) = 0;
    virtual void keyRelease(Item* item, int keyCode, KeyModifier mod) = 0;
    virtual void extMouseDrop(Item* item, Point loc, PasteboardContent& content) = 0;
    virtual void quit() = 0;
};

} // namespace spix
