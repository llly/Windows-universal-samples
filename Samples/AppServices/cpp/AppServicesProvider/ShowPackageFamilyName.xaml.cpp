// Copyright (c) Microsoft. All rights reserved.

#include "pch.h"
#include "ShowPackageFamilyName.xaml.h"
#include <sstream>

using namespace SDKTemplate;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::AppService;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Navigation;

ShowPackageFamilyName::ShowPackageFamilyName() : rootPage(MainPage::Current)
{
    InitializeComponent();
}

void ShowPackageFamilyName::OnNavigatedTo(NavigationEventArgs^ e)
{
	PackageFamilyName->Text = Package::Current->Id->FamilyName;
}