/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include "Command.h"
#include <Spix/Data/ItemPosition.h>
#include <Spix/Events/SwipeDirection.h>

namespace spix {
namespace cmd {

class Swipe : public Command {
public:
    Swipe(ItemPosition targetPosition, SwipeDirection direction);

    void execute(CommandEnvironment&) override;
    bool canExecuteNow() override;

private:
    ItemPosition m_itemPosition;
    SwipeDirection direction;
};

} // namespace cmd
} // namespace spix
