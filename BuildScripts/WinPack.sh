#!/bin/bash          
echo Updating Bundle
des=../Solutions/Debug/OpenGLESApp2.WindowsUniversal.Application/AppX/Resources_staged/
echo $des
 rsync -duv --perms  ../CollisionData/ $des
 rsync -duv --perms  ../CollisionData/BasicCollisionTypes/ $des
 rsync -duv --perms  ../CollisionData/CollisionGroups/ $des
 rsync -duv --perms  ../Textures/ $des
 rsync -duv --perms  ../UIComponentArchetypes/ $des
 rsync -duv --perms  ../UIRenderables/ $des
 rsync -duv --perms  ../Renderables/ $des
 rsync -duv --perms  ../Renderables/CollisionRenderables/ $des
 rsync -duv --perms  ../Meshes/ $des
 rsync -duv --perms  ../FontArchetypes/ $des
 rsync -duv --perms  ../EntityGroupData/ $des
 rsync -duv --perms  ../EntityArchetypes/ $des
 rsync -duv --perms  ../Materials/ $des
 rsync -duv --perms  ../Shaders/ $des
 rsync -duv --perms  ../Shaders/Fragment/ $des
 rsync -duv --perms  ../Shaders/Programs/ $des
 rsync -duv --perms  ../Shaders/Vertex/ $des
 cd ../Solutions/Debug/OpenGLESApp2.WindowsUniversal.Application/AppX/
 ./swap.bat
echo Update Complete
