/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Scene/Item.h>

class QQuickItem;

namespace spix {

class QtItem : public Item {
public:
    QtItem() = delete;
    QtItem(QQuickItem* item);

    Size size() const override;
    Point position() const override;
    Rect bounds() const override;
    std::string stringProperty(const std::string& name) const override;
    void setStringProperty(const std::string& name, const std::string& value) override;
    bool invokeMethod(const std::string& method, const std::vector<Variant>& args, Variant& ret) override;
    bool visible() const override;
    Point mapToScene(Point* point);

    QQuickItem* qquickitem();

private:
    QQuickItem* m_item;
};

} // namespace spix
