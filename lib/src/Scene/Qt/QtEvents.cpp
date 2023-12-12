/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "QtEvents.h"

#include <Scene/Qt/QtItem.h>
#include <Scene/Qt/QtItemTools.h>

#include <Spix/Data/PasteboardContent.h>

#include <QGuiApplication>
#include <QObject>
#include <QQuickItem>
#include <QQuickWindow>
#include <QTest>
#include <QTouchDevice>

#include <iostream>
#include <QtTest/QtTest>

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QMimeData>
#include <cmath>
#include <unistd.h>
#include <chrono>
#include <thread>


#define PI 3.14159265358979323846

namespace spix {

namespace {

QQuickWindow* getWindowAndPositionForItem(Item* item, Point relToItemPos, QPointF& windowPos)
{
    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem)
        return nullptr;
    auto window = qtitem->qquickitem()->window();

    QPointF qtlocalPoint(relToItemPos.x, relToItemPos.y);
    windowPos = qtitem->qquickitem()->mapToScene(qtlocalPoint);

    return window;
}

Qt::MouseButton getQtMouseButtonValue(MouseButton button)
{
    unsigned qtButton = Qt::MouseButton::NoButton;

    if (button & MouseButtons::Left) {
        qtButton |= Qt::MouseButton::LeftButton;
    }

    if (button & MouseButtons::Right) {
        qtButton |= Qt::MouseButton::RightButton;
    }

    if (button & MouseButtons::Middle) {
        qtButton |= Qt::MouseButton::MiddleButton;
    }

    return static_cast<Qt::MouseButton>(qtButton);
}

Qt::KeyboardModifiers getQtKeyboardModifiers(KeyModifier mod)
{
    Qt::KeyboardModifiers qtmod = Qt::NoModifier;

    if (mod & KeyModifiers::Shift) {
        qtmod = qtmod | Qt::ShiftModifier;
    }
    if (mod & KeyModifiers::Control) {
        qtmod = qtmod | Qt::ControlModifier;
    }
    if (mod & KeyModifiers::Alt) {
        qtmod = qtmod | Qt::AltModifier;
    }
    if (mod & KeyModifiers::Meta) {
        qtmod = qtmod | Qt::MetaModifier;
    }

    return qtmod;
}

void sendQtKeyEvent(Item* item, bool press, int keyCode, KeyModifier mod)
{
    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem)
        return;

    auto window = qtitem->qquickitem()->window();

    auto qtmod = getQtKeyboardModifiers(mod);
    auto keyEvent = new QKeyEvent(press ? QEvent::KeyPress : QEvent::KeyRelease, keyCode, qtmod);
    QGuiApplication::postEvent(window, keyEvent);
}

} // namespace

void QtEvents::mouseDown(Item* item, Point loc, MouseButton button)
{
    QPointF windowLoc;
    auto window = getWindowAndPositionForItem(item, loc, windowLoc);
    if (!window)
        return;

    m_pressedMouseButtons |= button;
    Qt::MouseButton eventCausingButton = getQtMouseButtonValue(button);
    Qt::MouseButtons activeButtons = getQtMouseButtonValue(m_pressedMouseButtons);

    QMouseEvent* event
        = new QMouseEvent(QEvent::MouseButtonPress, windowLoc, eventCausingButton, activeButtons, Qt::NoModifier);
    QGuiApplication::postEvent(window, event);
}

void QtEvents::mouseUp(Item* item, Point loc, MouseButton button)
{
    QPointF windowLoc;
    auto window = getWindowAndPositionForItem(item, loc, windowLoc);
    if (!window)
        return;

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    // Qt6 expects the mouse to be down during the event
    Qt::MouseButton eventCausingButton = getQtMouseButtonValue(button);
    Qt::MouseButtons activeButtons = getQtMouseButtonValue(m_pressedMouseButtons);
    m_pressedMouseButtons ^= button;
#else
    // Qt5 expects the mouse to be up during the event
    m_pressedMouseButtons ^= button;
    Qt::MouseButton eventCausingButton = getQtMouseButtonValue(button);
    Qt::MouseButtons activeButtons = getQtMouseButtonValue(m_pressedMouseButtons);
#endif

    QMouseEvent* event
        = new QMouseEvent(QEvent::MouseButtonRelease, windowLoc, eventCausingButton, activeButtons, Qt::NoModifier);
    QGuiApplication::postEvent(window, event);
}

void QtEvents::rotate(Item* item, int degree)
{
    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem)
        return;
    auto window = qtitem->qquickitem()->window();
    QTouchDevice* touchDevice = QTest::createTouchDevice();
    auto size = item->size();
    auto posPoint = item->position();

    auto middlePointWidth = size.width * 0.5;
    auto middlePointHeight = size.height * 0.5;

    int radius = middlePointWidth;
    int rotationDirection = degree > 0 ? -1 : 1;

    QPoint middlePoint = QPoint(posPoint.x + middlePointWidth, posPoint.y  + middlePointHeight);
    auto x = (radius * sin(degree*PI/180)) + middlePoint.x();
    auto y = (radius * cos(degree*PI/180)) + middlePoint.y();
    QTest::QTouchEventSequence touch = QTest::touchEvent(window, touchDevice);
    touch.press(0, middlePoint).commit();
    touch.stationary(0);
    touch.press(1, QPoint(x, y));
    touch.commit();
    /*x = (radius * sin((degree-rotationDirection)*PI/180)) + middlePoint.x();
    y = (radius * cos((degree-rotationDirection)*PI/180)) + middlePoint.y();
    touch.stationary(0);
    touch.press(1, QPoint(x, y));
    touch.commit();
    x = (radius * sin((degree-rotationDirection-rotationDirection)*PI/180)) + middlePoint.x();
    y = (radius * cos((degree-rotationDirection-rotationDirection)*PI/180)) + middlePoint.y();
    touch.stationary(0);
    touch.press(1, QPoint(x, y));
    touch.commit();
    x = (radius * sin((degree-rotationDirection-rotationDirection-rotationDirection)*PI/180)) + middlePoint.x();
    y = (radius * cos((degree-rotationDirection-rotationDirection-rotationDirection)*PI/180)) + middlePoint.y();
    touch.stationary(0);
    touch.press(1, QPoint(x, y));
    touch.commit();*/

    for (int p_degree = degree; p_degree != rotationDirection; p_degree = p_degree + rotationDirection) {
        x = (radius * sin(p_degree*PI/180)) + middlePoint.x();
        y = (radius * cos(p_degree*PI/180)) + middlePoint.y();
        touch.stationary(0);
        touch.move(1, QPoint(x, y));
        touch.commit();
 }
    touch.release(0, middlePoint);
    touch.release(1, QPoint(x, y));
    touch.commit();
}

/*
void QtEvents::touchPress(TouchSequencer* touchSequencer, Item* item, Point loc, int touchId){
    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem)
        return;
    auto qtouchSequencer = dynamic_cast<TouchSequencer*>(touchSequencer);
    if (!qtouchSequencer)
        return;
    qtouchSequencer->press(touchId, qtitem->qquickitem()->mapToScene(QPointF(loc.x , loc.y)).toPoint());
    qtouchSequencer->commit();
}

void QtEvents::touchMove(TouchSequencer* touchSequencer, Item* item, Point loc, int touchId){
    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem)
        return;
    auto qtouchSequencer = dynamic_cast<TouchSequencer*>(touchSequencer);
    if (!qtouchSequencer)
        return;
    qtouchSequencer->move(touchId, qtitem->qquickitem()->mapToScene(QPointF(loc.x , loc.y)).toPoint());
    qtouchSequencer->commit();
}

void QtEvents::touchStationary(TouchSequencer* touchSequencer, int touchId){
    touchEventSequence.stationary(touchId);
    touchEventSequence.commit();
    //delete touchDevice;
}

void QtEvents::touchRelease(TouchSequencer* touchSequencer,  Item* item, Point loc, int touchId) {
    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem)
        return;
    auto qtouchSequencer = dynamic_cast<TouchSequencer*>(touchSequencer);
    if (!qtouchSequencer)
        return;
    qtouchSequencer->release(touchId, qtitem->qquickitem()->mapToScene(QPointF(loc.x , loc.y)).toPoint());
    qtouchSequencer->commit();
}

*/

void QtEvents::swipe(Item* item, Point from, Point to, std::vector<Point> touchpoints)
{
    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem){
        return;
    }

    auto window = qtitem->qquickitem()->window();
    QTouchDevice* touchDevice = QTest::createTouchDevice();
    QTest::QTouchEventSequence touch = QTest::touchEvent(window, touchDevice);

    QPointF windowLoc = qtitem->qquickitem()->mapToScene(QPointF(from.x, from.y));

    touch.press(0, QPoint(windowLoc.rx(), windowLoc.ry()));
  //  touch.press(1,  QPoint(windowLoc.rx(), windowLoc.ry()+5));
    touch.commit();

    for (Point element : touchpoints) {
         windowLoc = qtitem->qquickitem()->mapToScene(QPointF(element.x, element.y));
         QPoint(windowLoc.rx(), windowLoc.ry());
         QPoint(windowLoc.rx(), windowLoc.ry()+5);
         touch.move(0, QPoint(windowLoc.rx(), windowLoc.ry()));
   //     touch.move(1,  QPoint(windowLoc.rx(), windowLoc.ry()+5));
        touch.commit();
    }

     windowLoc = qtitem->qquickitem()->mapToScene(QPointF(to.x, to.y));
     QPoint windowLocInt(windowLoc.rx(), windowLoc.ry());
   //  QPoint windowLocInt2(windowLoc.rx(), windowLoc.ry()+5);

    touch.release(0, windowLocInt);
    //touch.release(1, windowLocInt2);
    touch.commit();
}

void QtEvents::tap(Item* item, Point touchPoint, int duration)
{
    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem){
        return;
    }
    QPointF qtlocalPoint(touchPoint.x, touchPoint.y);
    QPointF windowLoc = qtitem->qquickitem()->mapToScene(qtlocalPoint);
    QPoint windowLocInt(windowLoc.rx(), windowLoc.ry());

    auto window = qtitem->qquickitem()->window();
    QTouchDevice* touchDevice = QTest::createTouchDevice();
    QTest::QTouchEventSequence touch = QTest::touchEvent(window, touchDevice);

    touch.press(0, QPoint(windowLocInt.x(), windowLocInt.y()), 0);
    touch.commit();
    if(duration > 0){
        touch.stationary( 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(duration));
        touch.commit();
    }
    touch.release(0, QPoint(windowLocInt.x(), windowLocInt.y()));
    touch.commit();
}

void QtEvents::pinch(Item* item, std::vector<std::vector<Point>> touchpoints)
{
    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem){
        return;
    }

    auto window = qtitem->qquickitem()->window();

    QTouchDevice* touchDevice = QTest::createTouchDevice();
    QTest::QTouchEventSequence touch = QTest::touchEvent(window, touchDevice);
    std::vector<Point> tp1 = touchpoints[0];
    std::vector<Point> tp2 = touchpoints[1];

    std::vector<Point>::iterator it1 = tp1.begin();
    std::vector<Point>::iterator it2 = tp2.begin();

    touch.press(0, QPoint((*tp1.begin()).x, (*tp1.begin()).y));
    touch.press(1, QPoint((*tp2.begin()).x, (*tp2.begin()).y));
    touch.commit();

    while (it1 != tp1.end() || it2 != tp2.end()) {
        if (it1 == tp1.end()) {
            touch.stationary(0);
        } else {
            Point point1 = *it1;
            touch.move(0, QPoint(point1.x, point1.y));
            it1 = std::next(it1, 1);
        }
        if (it2 == tp2.end()) {
            touch.stationary(1);
        } else {
            Point point2 = *it2;
            touch.move(1, QPoint(point2.x, point2.y));
            it2 = std::next(it2, 1);
        }
        touch.commit();
    }
    Point lastPoint1 = *std::prev(tp1.end(), 1);
    Point lastPoint2 = *std::prev(tp2.end(), 1);

    touch.release(0, QPoint(lastPoint1.x, lastPoint1.y));
    touch.release(1, QPoint(lastPoint2.x, lastPoint2.y));


    sleep(10);
    touch.commit();
    delete touchDevice;
    //QGuiApplication::sync();
}

void QtEvents::mouseMove(Item* item, Point loc)
{
    QPointF windowLoc;
    auto window = getWindowAndPositionForItem(item, loc, windowLoc);
    if (!window)
        return;

    Qt::MouseButton activeButtons = getQtMouseButtonValue(m_pressedMouseButtons);

    // Wiggle the cursor a bit. This is needed to correctly recognize drag events
    windowLoc.rx() -= 1;
    QMouseEvent* mouseMoveEvent
        = new QMouseEvent(QEvent::MouseMove, windowLoc, Qt::MouseButton::NoButton, activeButtons, Qt::NoModifier);
    QGuiApplication::postEvent(window, mouseMoveEvent);

    // Wiggle the cursor a bit. This is needed to correctly recognize drag events
    windowLoc.rx() += 1;
    mouseMoveEvent
        = new QMouseEvent(QEvent::MouseMove, windowLoc, Qt::MouseButton::NoButton, activeButtons, Qt::NoModifier);
    QGuiApplication::postEvent(window, mouseMoveEvent);
}

void QtEvents::stringInput(Item* item, const std::string& text)
{
    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem)
        return;

    auto window = qtitem->qquickitem()->window();

    auto keyDownEvent = new QKeyEvent(QEvent::KeyPress, 0 /* key id */, Qt::NoModifier, QString::fromStdString(text));
    QGuiApplication::postEvent(window, keyDownEvent);
}

void QtEvents::keyPress(Item* item, int keyCode, KeyModifier mod)
{
    sendQtKeyEvent(item, true, keyCode, mod);
}

void QtEvents::keyRelease(Item* item, int keyCode, KeyModifier mod)
{
    sendQtKeyEvent(item, false, keyCode, mod);
}

void QtEvents::extMouseDrop(Item* item, Point loc, PasteboardContent& content)
{
    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem)
        return;

    auto window = qtitem->qquickitem()->window();

    QPointF qtlocalPoint(loc.x, loc.y);
    QPointF windowLoc = qtitem->qquickitem()->mapToScene(qtlocalPoint);
    QPoint windowLocInt(windowLoc.rx(), windowLoc.ry());

    auto mimeData = new QMimeData();
    QList<QUrl> urlList;
    for (const auto& url : content.urls()) {
        QUrl qurl(QString::fromStdString(url));
        urlList.append(qurl);
    }
    mimeData->setUrls(urlList);

    auto enter = new QDragEnterEvent(windowLocInt, Qt::CopyAction | Qt::MoveAction | Qt::LinkAction, mimeData,
        Qt::MouseButton::NoButton, Qt::NoModifier);
    QGuiApplication::postEvent(window, enter);

    auto move = new QDragMoveEvent(windowLocInt, Qt::CopyAction | Qt::MoveAction | Qt::LinkAction, mimeData,
        Qt::MouseButton::NoButton, Qt::NoModifier);
    QGuiApplication::postEvent(window, move);

    auto drop = new QDropEvent(windowLoc, Qt::CopyAction | Qt::MoveAction | Qt::LinkAction, mimeData,
        Qt::MouseButton::NoButton, Qt::NoModifier);
    QGuiApplication::postEvent(window, drop);
}

void QtEvents::quit()
{
    QGuiApplication::quit();
}

} // namespace spix
