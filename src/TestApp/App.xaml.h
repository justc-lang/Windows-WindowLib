#pragma once
#include "App.g.h"

namespace winrt::TestApp::implementation {
struct App : AppT<App> {
    App() = default;
    void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);
};
}

namespace winrt::TestApp::factory_implementation {
struct App : AppT<App, implementation::App> {};
}
