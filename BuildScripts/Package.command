#!/bin/bash          
echo Updating Bundle   
cd "$( dirname "${BASH_SOURCE[0]}" )"
cd ..
DIR="$( pwd )"
rsync -duv $DIR/Archetypes/AI/AIGoal/ $DIR/Resources.bundle
rsync -duv $DIR/Archetypes/AI/NPCAI/ $DIR/Resources.bundle
rsync -duv $DIR/Archetypes/AI/AIBehavior/ $DIR/Resources.bundle
rsync -duv $DIR/CollisionData/ $DIR/Resources.bundle
rsync -duv $DIR/CollisionData/BasicCollisionTypes/ $DIR/Resources.bundle
rsync -duv $DIR/CollisionData/CustomCollisionTypes/ $DIR/Resources.bundle
rsync -duv $DIR/CollisionData/CollisionGroups/ $DIR/Resources.bundle
rsync -duv $DIR/Textures/ $DIR/Resources.bundle
rsync -duv $DIR/UIComponentArchetypes/ $DIR/Resources.bundle
rsync -duv $DIR/UIRenderables/ $DIR/Resources.bundle
rsync -duv $DIR/Renderables/ $DIR/Resources.bundle
rsync -duv $DIR/Renderables/CollisionRenderables/ $DIR/Resources.bundle
rsync -duv $DIR/Meshes/ $DIR/Resources.bundle
rsync -duv $DIR/FontArchetypes/ $DIR/Resources.bundle
rsync -duv $DIR/EntityGroupData/ $DIR/Resources.bundle
rsync -duv $DIR/EntityArchetypes/ $DIR/Resources.bundle
rsync -duv $DIR/Materials/ $DIR/Resources.bundle
rsync -duv $DIR/Shaders/ $DIR/Resources.bundle
rsync -duv $DIR/Shaders/Fragment/ $DIR/Resources.bundle
rsync -duv $DIR/Shaders/Programs/ $DIR/Resources.bundle
rsync -duv $DIR/Shaders/Vertex/ $DIR/Resources.bundle
cd $DIR/Resources.bundle
for i in $( ls | grep [A-Z] ); do mv -if $i `echo $i | tr 'A-Z' 'a-z'`; done
echo Update Complete
