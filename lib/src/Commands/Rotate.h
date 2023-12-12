/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include "Command.h"
#include <Spix/Data/ItemPath.h>


namespace spix {
namespace cmd {

class Rotate : public Command {
public:
    Rotate(ItemPath targetItemPath, int degree);

    void execute(CommandEnvironment&) override;
    bool canExecuteNow() override;

private:
    ItemPath m_itemPath;
    int degree;
};

} // namespace cmd
} // namespace spix
