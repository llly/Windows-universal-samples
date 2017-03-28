// Copyright (c) Microsoft. All rights reserved.

#pragma once

#include "KeepConnectionOpenScenario.g.h"
#include "MainPage.xaml.h"

namespace SDKTemplate
{
    /// <summary>
    /// Display animation.
    /// </summary>
    [Windows::Foundation::Metadata::WebHostHidden]
	public ref class KeepConnectionOpenScenario sealed
    {
    public:
        KeepConnectionOpenScenario();
    private:
        MainPage^ rootPage;
		Windows::ApplicationModel::AppService::AppServiceConnection^ connection;
		void OpenConnection_Click(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Connection_ServiceClosed(Windows::ApplicationModel::AppService::AppServiceConnection^ sender, Windows::ApplicationModel::AppService::AppServiceClosedEventArgs^ args);
		void CloseConnection_Click(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void GenerateRandomNumber_Click(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    };
}
