//
//  MaterialManager.h
//  Projectios2
//
//  Created by Kamil Konecko on 10/14/15.
//
//

#ifndef __Projectios2__MaterialManager__
#define __Projectios2__MaterialManager__

#include <stdio.h>
#include <iostream>
#include <map>
#include "Material.h"
#include "Core/StringManager/StringManager.h"
#include "Interface.hpp"

class MaterialManager
{
public:
    void init(IResourceLoader* resourceLoader);
    Material* getMaterial(STRING_ID name);
    
    void release();
    static MaterialManager* getInstance();
    
private:
    IResourceLoader* m_resourceLoader;
    typedef std::map<STRING_ID, Material*> StrIDToMaterialPtrMap;
    StrIDToMaterialPtrMap m_materials;
    static MaterialManager* m_instance;
    Material* parseMaterial(STRING_ID name);
    
    
};

#endif /* defined(__Projectios2__MaterialManager__) */
