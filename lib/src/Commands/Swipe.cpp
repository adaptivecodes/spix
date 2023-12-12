/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "Swipe.h"
#include <Scene/Scene.h>
#include <Spix/Data/Geometry.h>
#include <Spix/Data/GeometryUtils.h>
#include <Spix/Events/SwipeDirection.h>
#include <iostream>
#include <tuple>

namespace spix {
namespace cmd {

Swipe::Swipe(ItemPosition targetPosition, SwipeDirection direction):
    m_itemPosition(std::move(targetPosition)),
    direction(direction)
{}
/*
void Swipe::execute(CommandEnvironment& env){
    GeometryUtils geos;
    auto path = m_itemPosition.itemPath();
    auto item = env.scene().itemAtPath(path);
    if (!item) {
        env.state().reportError("Swipe: Item not found: " + path.string());
        return;
    }
    spix::Size size = item->size();
    Point from = m_itemPosition.positionForItemSize(size);
    Point to = geos.getPositionForPositionSizeAndSwipeDirecton(m_itemPosition, item->size(), direction);

    auto fromTo = std::make_tuple(from, to);
    std::vector<Point> pointsOnLine = geos.calculatePathBetweenPoints(fromTo, Strategy::LINEAR);
    TouchSequencer ts(item.get());
  //  QTouchDevice* touchDevice = QTest::createTouchDevice();
   // QTest::QTouchEventSequence touch = QTest::touchEvent(item->qquickitem()->window(), touchDevice);

    env.scene().events().touchPress(ts, item.get(), from, 0);

    for (Point element : pointsOnLine) {
        env.scene().events().touchMove(ts, item.get(), element, 0);
    }
    env.scene().events().touchRelease(ts, item.get(), to, 0);
}
*/

void Swipe::execute(CommandEnvironment& env)
{
    GeometryUtils geos;
    auto path = m_itemPosition.itemPath();
    auto item = env.scene().itemAtPath(path);
    if (!item) {
        std::cout << "Swipe: No Item Found" << std::endl;
        env.state().reportError("Swipe: Item not found: " + path.string());
        return;
    }
    std::cout << "Swipe: Item Found" << std::endl;
    spix::Size size = item->size();
    Point from = m_itemPosition.positionForItemSize(size);
    Point to = geos.getPositionForPositionSizeAndSwipeDirecton(m_itemPosition, item->size(), direction);

    auto fromTo = std::make_tuple(from, to);
    std::cout <<"Calculate Path from P1("<<from.x << ", " << from.y << ") to P2(" <<to.x << ", " << to.y << ")" << std::endl;

    std::vector<Point> pointsOnLine = geos.calculatePathBetweenPoints(fromTo, Strategy::LINEAR);

    env.scene().events().swipe(item.get(), from, to, pointsOnLine);
}

bool Swipe::canExecuteNow()
{
    return true;
}

} // namespace cmd
} // namespace spix
