/***
 * Copyright (C) Andreas Deister. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include "Command.h"
#include <Spix/Data/ItemPosition.h>

namespace spix {
namespace cmd {

class Tap : public Command {
public:
    Tap(ItemPosition itemPosition, int duration=0);

    void execute(CommandEnvironment&) override;
    bool canExecuteNow() override;

private:
    ItemPosition m_position;
    int duration;
};

} // namespace cmd
} // namespace spix
