/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/spix_export.h>

#include <Scene/Events.h>
#include <Scene/Mock/MockItem.h>

#include <functional>

namespace spix {

class SPIX_EXPORT MockEvents : public Events {
public:
    /*
    void touchPress(Item* item, Point loc, int touchId) override;
    void touchMove(Item* item, Point loc, int touchId) override;
    void touchRelease(Item* item, Point loc, int touchId) override;
    void touchStationary(Item* item, int touchId) override;
*/
    void pinch(Item* item, std::vector<std::vector<Point>> touchpoints) override;
    void rotate(Item* item, int degree) override;
    void swipe(Item* item, Point from, Point to,  std::vector<Point> mouseMoves) override;
    void tap(Item* item, Point touchPoint, int duration) override;

    void mouseDown(Item* item, Point loc, MouseButton button) override;
    void mouseUp(Item* item, Point loc, MouseButton button) override;
    void mouseMove(Item* item, Point loc) override;
    void stringInput(Item* item, const std::string& text) override;
    void keyPress(Item* item, int keyCode, KeyModifier mod) override;
    void keyRelease(Item* item, int keyCode, KeyModifier mod) override;
    void extMouseDrop(Item* item, Point loc, PasteboardContent& content) override;
    void quit() override;

    // Mock stuff
    std::function<void(Item*, Point, bool, bool)> onMouseClickEvent;
    std::function<void(Item*, const std::string&)> onStringInputEvent;
    std::function<void(Item*, Point, PasteboardContent&)> onMouseDropEvent;
};

} // namespace spix
