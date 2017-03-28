// Copyright (c) Microsoft. All rights reserved.

#pragma once

#include "OpenCloseConnectionScenario.g.h"
#include "MainPage.xaml.h"

namespace SDKTemplate
{
    /// <summary>
    /// Display animation.
    /// </summary>
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class OpenCloseConnectionScenario sealed
    {
    public:
        OpenCloseConnectionScenario();
    private:
		MainPage^ rootPage;
		void GenerateRandomNumber_Click(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    };
}
