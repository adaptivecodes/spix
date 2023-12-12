/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "Rotate.h"
#include <Scene/Scene.h>


namespace spix {
namespace cmd {

Rotate::Rotate(ItemPath targetItemPath, int degree):
    m_itemPath {std::move(targetItemPath)}, degree(degree) {}

void Rotate::execute(CommandEnvironment& env)
{
    auto item = env.scene().itemAtPath(m_itemPath);
    env.scene().events().rotate(item.get(), degree);
}

bool Rotate::canExecuteNow()
{return true;}

} // namespace cmd
} // namespace spix
