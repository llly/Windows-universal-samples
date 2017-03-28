// Copyright (c) Microsoft. All rights reserved.

#include "pch.h"
#include "KeepConnectionOpenScenario.xaml.h"
#include <sstream>

using namespace SDKTemplate;

using namespace concurrency;

using namespace Platform;
using namespace Windows::ApplicationModel::AppService;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

KeepConnectionOpenScenario::KeepConnectionOpenScenario() : rootPage(MainPage::Current)
{
    InitializeComponent();
}


void KeepConnectionOpenScenario::OpenConnection_Click(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	//Is a connection already open?
	if (connection != nullptr)
	{
		rootPage->NotifyUser("A connection already exists", NotifyType::ErrorMessage);
		return;
	}

	//Set up a new app service connection
	connection = ref new AppServiceConnection();
	connection->AppServiceName = "com.microsoft.randomnumbergenerator";
	connection->PackageFamilyName = "Microsoft.SDKSamples.AppServicesProvider.CPP_8wekyb3d8bbwe";
	connection->ServiceClosed += ref new TypedEventHandler < AppServiceConnection ^, AppServiceClosedEventArgs ^ > (this, &KeepConnectionOpenScenario::Connection_ServiceClosed);

	create_task(connection->OpenAsync()).then([this](AppServiceConnectionStatus status)
	{	//If the new connection opened successfully we're done here
		if (status == AppServiceConnectionStatus::Success)
		{
			rootPage->NotifyUser("Connection is open", NotifyType::StatusMessage);
		}
		else
		{
			//Something went wrong. Lets figure out what it was and show the 
			//user a meaningful message
			switch (status)
			{
			case AppServiceConnectionStatus::AppNotInstalled:
				rootPage->NotifyUser("The app AppServicesProvider is not installed. Deploy AppServicesProvider to this device and try again.", NotifyType::ErrorMessage);
				break;

			case AppServiceConnectionStatus::AppUnavailable:
				rootPage->NotifyUser("The app AppServicesProvider is not available. This could be because it is currently being updated or was installed to a removable device that is no longer available.", NotifyType::ErrorMessage);
				break;

			case AppServiceConnectionStatus::AppServiceUnavailable:
				rootPage->NotifyUser(String::Concat("The app AppServicesProvider is installed but it does not provide the app service.", connection->AppServiceName), NotifyType::ErrorMessage);
				break;

			case AppServiceConnectionStatus::Unknown:
				rootPage->NotifyUser("An unkown error occurred while we were trying to open an AppServiceconnection->", NotifyType::ErrorMessage);
				break;
			}

			//Clean up before we go
			//connection->Dispose();
			connection = nullptr;
		}
	});
}
void KeepConnectionOpenScenario::Connection_ServiceClosed(Windows::ApplicationModel::AppService::AppServiceConnection^ sender, Windows::ApplicationModel::AppService::AppServiceClosedEventArgs^ args)
{
//	Dispatcher::RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () = >
	{
		//Dispose the connection reference we're holding
		if (connection != nullptr)
		{
			//connection->Close();
			connection = nullptr;
		}
	}

}
void KeepConnectionOpenScenario::CloseConnection_Click(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//Is there an open connection?
	if (connection == nullptr)
	{
		rootPage->NotifyUser("There's no open connection to close", NotifyType::ErrorMessage);
		return;
	}

	//Close the open connection
	//connection->Close();
	connection = nullptr;

	//Let the user know we closed the connection
	rootPage->NotifyUser("Connection is closed", NotifyType::StatusMessage);

}
void KeepConnectionOpenScenario::GenerateRandomNumber_Click(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	//Is there an open connection?
	if (connection == nullptr)
	{
		rootPage->NotifyUser("You need to open a connection before trying to generate a random number.", NotifyType::ErrorMessage);
		return;
	}

	//Parse user input
	long minValueInput = 0;
	const wchar_t* begin = MinValue->Text->Begin();
	const wchar_t* end = begin + std::wcslen(begin);;
	wchar_t* last_interpreted{ nullptr };
	minValueInput = std::wcstol(begin, &last_interpreted, 10);
	if (last_interpreted != end)
	{
		rootPage->NotifyUser("The Minimum Value should be a valid integer", NotifyType::ErrorMessage);
		return;
	}

	long maxValueInput = 0;
	begin = MaxValue->Text->Data();
	size_t length = std::wcslen(begin);
	end = begin + length;
	maxValueInput = std::wcstol(begin, &last_interpreted, 10);
	if (last_interpreted != end)
	{
		rootPage->NotifyUser("The Maximum Value should be a valid integer", NotifyType::ErrorMessage);
		return;
	}
	if (maxValueInput <= minValueInput)
	{
		rootPage->NotifyUser("Maximum Value must be larger than Minimum Value", NotifyType::ErrorMessage);
		return;
	}

	//Send a message to the app service
	ValueSet^ inputs = ref new ValueSet();
	std::wstringstream min;
	min <<minValueInput;
	String^ minString = ref new String(min.str().c_str());
	inputs->Insert("minvalue", minString);

	std::wstringstream max;
	max << maxValueInput;
	String^ maxString = ref new String(max.str().c_str());
	inputs->Insert("maxvalue", (int)maxValueInput);
	create_task(connection->SendMessageAsync(inputs)).then([this](AppServiceResponse ^response)
	{
		//If the service responded display the Message. We're done!
		if (response->Status == AppServiceResponseStatus::Success)
		{
			if (!response->Message->HasKey("result"))
			{
				rootPage->NotifyUser("The app service response message does not contain a key called \"result\"", NotifyType::StatusMessage);
				return;
			}

			String^ resultText = response->Message->Lookup("result")->ToString();
			if (!resultText->IsEmpty())
			{
				Result->Text = resultText;
				rootPage->NotifyUser("App service responded with a result", NotifyType::StatusMessage);
			}
			else
			{
				rootPage->NotifyUser("App service did not respond with a result", NotifyType::ErrorMessage);
			}
		}
		else
		{
			//Something went wrong. Show the user a meaningful
			//message depending upon the status
			switch (response->Status)
			{
			case AppServiceResponseStatus::Failure:
				rootPage->NotifyUser("The service failed to acknowledge the message we sent it. It may have been terminated because the client was suspended.", NotifyType::ErrorMessage);
				break;

			case AppServiceResponseStatus::ResourceLimitsExceeded:
				rootPage->NotifyUser("The service exceeded the resources allocated to it and had to be terminated.", NotifyType::ErrorMessage);
				break;

			case AppServiceResponseStatus::Unknown:
			default:
				rootPage->NotifyUser("An unkown error occurred while we were trying to send a message to the service.", NotifyType::ErrorMessage);
				break;
			}
		}
	});
}