#!/bin/bash          
echo Updating Bundle   
rsync -duv ../CollisionData/ ../Resources.bundle
rsync -duv ../CollisionData/BasicCollisionTypes/ ../Resources.bundle
rsync -duv ../CollisionData/CustomCollisionTypes/ ../Resources.bundle
rsync -duv ../CollisionData/CollisionGroups/ ../Resources.bundle
rsync -duv ../Textures/ ../Resources.bundle
rsync -duv ../UIComponentArchetypes/ ../Resources.bundle
rsync -duv ../UIRenderables/ ../Resources.bundle
rsync -duv ../Renderables/ ../Resources.bundle
rsync -duv ../Renderables/CollisionRenderables/ ../Resources.bundle
rsync -duv ../Meshes/ ../Resources.bundle
rsync -duv ../FontArchetypes/ ../Resources.bundle
rsync -duv ../EntityGroupData/ ../Resources.bundle
rsync -duv ../EntityArchetypes/ ../Resources.bundle
rsync -duv ../Materials/ ../Resources.bundle
rsync -duv ../Shaders/ ../Resources.bundle
rsync -duv ../Shaders/Fragment/ ../Resources.bundle
rsync -duv ../Shaders/Programs/ ../Resources.bundle
rsync -duv ../Shaders/Vertex/ ../Resources.bundle
echo Update Complete
