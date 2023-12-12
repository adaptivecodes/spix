/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "MockEvents.h"

namespace spix {

void MockEvents::mouseDown(Item* item, Point loc, MouseButton /*button*/)
{
    if (onMouseClickEvent) {
        onMouseClickEvent(item, loc, true, false);
    }
}

void MockEvents::mouseUp(Item* item, Point loc, MouseButton /*button*/)
{
    if (onMouseClickEvent) {
        onMouseClickEvent(item, loc, false, true);
    }
}

void MockEvents::mouseMove(Item* item, Point loc)
{
    if (onMouseClickEvent) {
        onMouseClickEvent(item, loc, false, false);
    }
}

void MockEvents::stringInput(Item* item, const std::string& text)
{
    if (onStringInputEvent) {
        onStringInputEvent(item, text);
    }
}

void MockEvents::keyPress(Item* item, int keyCode, KeyModifier mod)
{
}

void MockEvents::keyRelease(Item* item, int keyCode, KeyModifier mod)
{
}

void MockEvents::extMouseDrop(Item* item, Point loc, PasteboardContent& content)
{
    if (onMouseDropEvent) {
        onMouseDropEvent(item, loc, content);
    }
}

void MockEvents::quit()
{
}

void MockEvents::swipe(Item* item, Point from, Point to, std::vector<Point> mouseMoves)
{
}

void MockEvents::pinch(Item* item, std::vector<std::vector<Point>> touchpoints)
{
}

void MockEvents::rotate(Item* item, int degree)
{
}
/*
void MockEvents::touchPress(Item* item, Point loc, int touchId)
{}

void MockEvents::touchMove(Item* item, Point loc, int touchId)
{}

void MockEvents::touchRelease(Item* item, Point loc, int touchId)
{}

void MockEvents::touchStationary(Item* item, int touchId)
{}
*/
} // namespace spix
