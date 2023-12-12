/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include "Command.h"
#include <Spix/Data/ItemPath.h>
#include <Spix/Data/Geometry.h>
#include <Spix/Events/PinchDirection.h>


namespace spix {
namespace cmd {

class Pinch : public Command {
public:
    Pinch(ItemPath targetItemPath, std::vector<std::tuple<Point, Point>> touchpoints);

    void execute(CommandEnvironment&) override;
    bool canExecuteNow() override;

private:
    ItemPath m_itemPath;
    std::vector<std::tuple<Point, Point>> touchpoints;
};

} // namespace cmd
} // namespace spix
