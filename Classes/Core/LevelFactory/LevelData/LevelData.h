//
//  LevelData.h
//  Projectios1
//
//  Created by Kamil Konecko on 6/16/13.
//
//

#ifndef Projectios1_LevelData_h
#define Projectios1_LevelData_h
#include "Core/LevelFactory/Level.h"

class LevelData{
    public:
        virtual ~LevelData(){};
        virtual void initLevel(Level &level) = 0;
    
    
};

#endif
