//
// Created by dev1 on 14.08.23.
//

#ifndef SPIX_GEOMETRYUTILS_H
#define SPIX_GEOMETRYUTILS_H

#include <Spix/Events/SwipeDirection.h>
#include <Spix/Data/Geometry.h>
#include <Spix/Data/ItemPosition.h>
#include <vector>


namespace spix {

enum SPIX_EXPORT Strategy{LINEAR, RADIAL, BEZIER};
/**
enum SPIX_EXPORT PinchCorner{
    UPPER_LEFT = 0b1,
    UPPER_RIGHT= 0b1<<1,
    LOWER_LEFT = 0b1<<2,
    LOWER_RIGHT= 0b1<<3
};
**/
class SPIX_EXPORT GeometryUtils {
public:
     std::vector<Point> calculatePathBetweenPoints(std::tuple<Point,Point> fromTo, Strategy strategy=Strategy::LINEAR);
     Point getPositionForPositionSizeAndSwipeDirecton(ItemPosition itemPosition, Size itemSize, SwipeDirection direction);
     SwipeDirection getSwipeDirection(std::tuple<Point,Point> fromTo);
    // std::vector<std::tuple<Point, Point>> calculatePointsAfterScale(Rect item, double scale, int  pinchCorner);

};

} // namespace spix

#endif // SPIX_GEOMETRYUTILS_H
