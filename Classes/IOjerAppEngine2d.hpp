// Physical orientation of a handheld device, equivalent to UIDeviceOrientation
#include "Interface.hpp"
#include "Core/OpenSource/Vector.hpp"
#include "Core/Net/IRestConnector.h"
#include <Core/Keyboard/KeyboardManager.h>
#include <Core/General/IDeviceUtil.h>

enum DeviceOrientation {
    DeviceOrientationUnknown,
    DeviceOrientationPortrait,
    DeviceOrientationPortraitUpsideDown,
    DeviceOrientationLandscapeLeft,
    DeviceOrientationLandscapeRight,
    DeviceOrientationFaceUp,
    DeviceOrientationFaceDown,
};

// Create an instance of the renderer and set up various OpenGL state.
struct IOjerAppEngine2d* CreateRenderer();

// Interface to the OpenGL ES renderer; consumed by GLView.
struct IOjerAppEngine2d {
    virtual void Initialize(int width, int height, IResourceLoader *resourceLoader, float scaleFactor) = 0;
    virtual void Render() = 0;
    virtual void OnDeltaTime(float timeStep) = 0;
    virtual void OnRotate(DeviceOrientation newOrientation) = 0;
    virtual void OnFingerUp(ivec2 oldLocation, ivec2 location) = 0;
    virtual void OnFingerDown(ivec2 location) = 0;
    virtual void OnFingerMove(ivec2 oldLocation, ivec2 newLocation) = 0;
    
    virtual void SetRestConnector(IRestConnector* restConnector) = 0;
    virtual void SetDeviceUtil(IDeviceUtil* deviceUtil) = 0;
    virtual KeyboardManager* GetKeyboardManager() = 0;
    virtual ~IOjerAppEngine2d() {}
};
