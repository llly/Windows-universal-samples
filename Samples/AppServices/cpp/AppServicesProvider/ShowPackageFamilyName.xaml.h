// Copyright (c) Microsoft. All rights reserved.

#pragma once

#include "ShowPackageFamilyName.g.h"
#include "MainPage.xaml.h"

namespace SDKTemplate
{
    /// <summary>
    /// Display animation .
    /// </summary>
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class ShowPackageFamilyName sealed
    {
    public:
		ShowPackageFamilyName();
		 void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
    private:
		MainPage^ rootPage;
    };
}
