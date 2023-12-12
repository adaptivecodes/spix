/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "Tap.h"
#include <Scene/Scene.h>
#include <chrono>
#include <thread>
#include <iostream>

namespace spix {
namespace cmd {

Tap::Tap(ItemPosition itemPosition, int duration):
    m_position(std::move(itemPosition)),
    duration(duration) {}
/*
void Tap::execute(CommandEnvironment& env){
    auto path = m_position.itemPath();
    auto item = env.scene().itemAtPath(path);
    if (!item) {
        env.state().reportError("Tap: Item not found: " + path.string());
        return;
    }
    auto size = item->size();
    auto touchPoint = m_position.positionForItemSize(size);

    env.scene().events().touchPress(item.get(), touchPoint, 0);
    if(duration > 0){
        env.scene().events().touchStationary(item.get(), 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(duration));
    }
    env.scene().events().touchRelease(item.get(), touchPoint, 0);
}*/

void Tap::execute(CommandEnvironment& env){
    auto path = m_position.itemPath();
    auto item = env.scene().itemAtPath(path);
    std::cout << "Search on Path: " << path.string() << std::endl;
    if (!item) {
        std::cout << "No Item Found" << std::endl;
        env.state().reportError("Tap: Item not found: " + path.string());
        return;
    }
    std::cout << "Item Found" << std::endl;

    auto size = item->size();
    auto touchPoint = m_position.positionForItemSize(size);
    std::cout << "Point: " << touchPoint.x << ", " << touchPoint.y << std::endl;
    env.scene().events().tap(item.get(), touchPoint, duration);
}


bool Tap::canExecuteNow()
{
    return true;
}

} // namespace cmd
} // namespace spix
