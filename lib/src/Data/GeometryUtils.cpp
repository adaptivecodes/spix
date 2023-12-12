//
// Created by dev1 on 14.08.23.
//

#include "../../include/Spix/Data/GeometryUtils.h"
#include "Spix/Events/SwipeDirection.h"
#include <tuple>

namespace spix {

std::vector<Point> GeometryUtils::calculatePathBetweenPoints(
    std::tuple<Point, Point> fromTo, spix::Strategy strategy)
{
    Point from = std::get<0>(fromTo);
    Point to = std::get<1>(fromTo);
    auto swipeDirection = GeometryUtils::getSwipeDirection(fromTo);

    double m, b;
    std::vector<Point> pointsOnLine;
    switch (swipeDirection) {
    case SwipeDirection::LEFT:
        m = (to.y - from.y) / (to.x - from.x);
        b = from.y - m * from.x;
        for (int x = std::max(from.x, to.x); x >= std::min(from.x, to.x); x=x-1) {
            double y = m * x + b;
            pointsOnLine.push_back(Point(x, y));
        }
        break;
    case SwipeDirection::RIGHT:
        m = (to.y - from.y) / (to.x - from.x);
        b = from.y - m * from.x;

        for (int x = std::min(from.x, to.x); x <= std::max(from.x, to.x); x=x+1) {
            double y = m * x + b;
            pointsOnLine.push_back(Point(x, y));
        }
        break;
    }
    return pointsOnLine;
}


SwipeDirection GeometryUtils::getSwipeDirection(std::tuple<Point, Point> fromTo){
    Point from = std::get<0>(fromTo);
    Point to = std::get<1>(fromTo);
    return from.x > to.x ? SwipeDirection::LEFT : SwipeDirection::RIGHT;
}

Point GeometryUtils::getPositionForPositionSizeAndSwipeDirecton(ItemPosition itemPosition, Size itemSize, SwipeDirection direction){
    switch (direction) {
    case SwipeDirection::LEFT:
        return Point(0, itemPosition.positionForItemSize(itemSize).y);
    case SwipeDirection::RIGHT:
        return Point(itemSize.width,  itemPosition.positionForItemSize(itemSize).y);
    case SwipeDirection::UP:
        return Point(itemPosition.positionForItemSize(itemSize).x, 0);
    case SwipeDirection::DOWN:
        return Point(itemPosition.positionForItemSize(itemSize).x, itemSize.height);
    }
    return Point(0,0);
}


} // namespace spix