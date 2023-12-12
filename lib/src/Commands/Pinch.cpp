/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <QTest>
#include "Pinch.h"
#include <Spix/Data/Geometry.h>
#include <Spix/Data/GeometryUtils.h>
#include <Scene/Scene.h>
#include <tuple>

namespace spix {
namespace cmd {

Pinch::Pinch(ItemPath targetItemPath, std::vector<std::tuple<Point, Point>> touchpoints):
    m_itemPath {std::move(targetItemPath)},
    touchpoints(touchpoints)
{}

void Pinch::execute(CommandEnvironment& env)
{
    GeometryUtils geos;
    if(touchpoints.size() != 2){
        // Pinch Requires 2 Touchpoints
        return;
    }
    auto fromTo1 =  touchpoints[0];
    auto tp1 = geos.calculatePathBetweenPoints(fromTo1,  Strategy::LINEAR);
    auto fromTo2 = touchpoints[1];
    auto tp2 = geos.calculatePathBetweenPoints(fromTo2, Strategy::LINEAR);
    std::vector<std::vector<Point>> touchpoints;

    touchpoints.push_back(tp1);
    touchpoints.push_back(tp2);
    auto item = env.scene().itemAtPath(m_itemPath);
    env.scene().events().pinch(item.get(), touchpoints);
}

bool Pinch::canExecuteNow()
{return true;}

} // namespace cmd
} // namespace spix
