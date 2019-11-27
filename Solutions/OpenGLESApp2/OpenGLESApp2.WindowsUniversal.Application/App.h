#pragma once

#include <string>

#include <WindowsRestConnector.h>
#include <WindowsUserMemory.h>
#include "pch.h"

class OjerRenderer;

namespace OpenGLESApp2
{
    ref class App sealed : public Windows::ApplicationModel::Core::IFrameworkView
    {
    public:
        App();

        // IFrameworkView Methods.
        virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
        virtual void SetWindow(Windows::UI::Core::CoreWindow^ window);
        virtual void Load(Platform::String^ entryPoint);
        virtual void Run();
        virtual void Uninitialize();

    private:
        void RecreateRenderer();

        // Application lifecycle event handlers.
        void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args);

        // Window event handlers.
        void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
        void OnWindowClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args);
		void OnPointerPressed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);
		void OnPointerReleased(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);
		std::string VirtualKeyToString(const Windows::System::VirtualKey& virtualKey) const;
		std::string KeyPressToCorrectCapitalization(const std::string& s) const;
		bool ValidKeyPressed(const std::string& s) const;
		void OnKeyDown(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args);
		void OnKeyUp(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args);
	    void InitializeEGL(Windows::UI::Core::CoreWindow^ window);
        void CleanupEGL();
		std::string GetKeyStrFromVirtualKey(Windows::System::VirtualKey key) const;
        bool mWindowClosed;
        bool mWindowVisible;
		bool m_shiftKeyDown;
        
        EGLDisplay mEglDisplay;
        EGLContext mEglContext;
        EGLSurface mEglSurface;
		float mIputScale;

		WindowsRestConnector mRestConnector;
		WindowsUserMemory m_userMemory;

        std::unique_ptr<OjerRenderer> mCubeRenderer;
    };

}