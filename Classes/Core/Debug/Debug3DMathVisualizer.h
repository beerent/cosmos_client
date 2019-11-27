//
//  Debug3DMathVisualizer.h
//  Projectios2
//
//  Created by Kamil Konecko on 6/25/15.
//
//

#ifndef __Projectios2__Debug3DMathVisualizer__
#define __Projectios2__Debug3DMathVisualizer__

#include <stdio.h>
#include "Core/Generic/Patterns/Singleton.h"
#include <map>
#include "Core/StringManager/StringManager.h"
#include "glm/gtc/quaternion.hpp"

class Entity;
class Level;

class Debug3DVisual
{
public:
    friend class Debug3DMathVisualizer;
    Debug3DVisual(STRING_ID type);
    ~Debug3DVisual();
    
    void SetRotation(const glm::quat &rotation);
    void SetPosition(const glm::vec3 &position);
    void SetPosition(const glm::vec4 &position);
    
    void SetScale(float scale);

    glm::vec3 const &getPosition()const;
    glm::quat const &getRotation()const;

private:
    Entity* mEntity;
};

class Debug3DMathVisualizer : public Singleton< Debug3DMathVisualizer >
{

public:
    Debug3DMathVisualizer();
    
    void reset();
    void init(Level* activeLevel);
    Debug3DVisual* getVisual(int handle);
    int getOrCreateVisual(STRING_ID name, STRING_ID type);
    int createVisual(STRING_ID name, STRING_ID type);
    int getIDFromName(STRING_ID name) const;

    //destroy
    static void rotateVisualAlongLine(Debug3DVisual* visual, const glm::vec3& lineStart, const glm::vec3& lineEnd);
private:
    
    typedef std::map<int, Debug3DVisual*> IntToDebug3DVisualMap;
    typedef std::map<STRING_ID, int> StringIDToIntMap;
    
    StringIDToIntMap      mNameToIDMap;
    IntToDebug3DVisualMap mVisuals;
    int mUnusedIDIndex;
    Level* mActiveLevel;
    
};
#endif /* defined(__Projectios2__Debug3DMathVisualizer__) */
