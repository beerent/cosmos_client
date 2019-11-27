//
//  Primative.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 6/29/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Core/Entity/Entities/Primative.h"

// Define the positions and colors of two triangles.
const StringManager::StringID Primative::CLASS_ID_PRIMATIVE = StringManager::getIDForString("CLASS_ID_PRIMATIVE");

Primative::Primative():Entity(CLASS_ID_PRIMATIVE)
{
}

void Primative::Render()
{
}

void Primative::SetPosition(const Vector3<double> &position)
{
}

