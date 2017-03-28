#include "pch.h"
#include "RandomNumberGeneratorTask.h"

using namespace Windows::ApplicationModel::AppService;
using namespace Windows::ApplicationModel::Background;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::System;
using namespace Windows::Storage;
using namespace Windows::Security::Cryptography;

using namespace RandomNumberService;


void RandomNumberGeneratorTask::Run(IBackgroundTaskInstance^ taskInstance)
{
	//
	// Query BackgroundWorkCost
	// Guidance: If BackgroundWorkCost is high, then perform only the minimum amount
	// of work in the background task and return immediately.
	//
	auto cost = BackgroundWorkCost::CurrentBackgroundWorkCost;
	auto settings = ApplicationData::Current->LocalSettings;
	settings->Values->Insert("BackgroundWorkCost", cost.ToString());


	//
	// Get the deferral object from the task instance, and take a reference to the taskInstance.
	//
	TaskDeferral = taskInstance->GetDeferral();

	//
	// Associate a cancellation handler with the background task.
	//
	taskInstance->Canceled += ref new BackgroundTaskCanceledEventHandler(this, &RandomNumberGeneratorTask::OnCanceled);

	AppServiceTriggerDetails^ details = (AppServiceTriggerDetails^)taskInstance->TriggerDetails;
	connection = details->AppServiceConnection;

	//Listen for incoming app service requests
	connection->RequestReceived += ref new TypedEventHandler<AppServiceConnection ^, AppServiceRequestReceivedEventArgs ^>(this, &RandomNumberGeneratorTask::OnRequestReceived);

}

//
// Handles background task cancellation.
//
void RandomNumberGeneratorTask::OnCanceled(IBackgroundTaskInstance^ taskInstance, BackgroundTaskCancellationReason reason)
{

	if (TaskDeferral != nullptr)
	{
		//Complete the service deferral
		TaskDeferral->Complete();
		TaskDeferral = nullptr;
	}
}


void RandomNumberGeneratorTask::OnRequestReceived(AppServiceConnection^ sender, AppServiceRequestReceivedEventArgs^ args)
{
	//Get a deferral so we can use an awaitable API to respond to the message
	AppServiceDeferral^ messageDeferral = args->GetDeferral();

		ValueSet^ input = args->Request->Message;
		auto MinValue = input->Lookup("minvalue")->ToString();

		//Parse user input
		long minValueInput = 0;
		const wchar_t* begin = MinValue->Begin();
		const wchar_t*end = begin + std::wcslen(MinValue->Data());
		wchar_t* last_interpreted{ nullptr };
		minValueInput = std::wcstol(begin, &last_interpreted, 10);
		if (last_interpreted != end)
		{
		}

		auto MaxValue = input->Lookup("maxvalue")->ToString();
		long maxValueInput = 0;
		begin = MaxValue->Data();
		end = begin + std::wcslen(MaxValue->Data());
		maxValueInput = std::wcstol(begin, &last_interpreted, 10);
		if (last_interpreted != end)
		{

		}
		//Create the response

		ValueSet^ result = ref new ValueSet();
		result->Insert("result", (int)(CryptographicBuffer::GenerateRandomNumber()% (maxValueInput - minValueInput) + minValueInput));
		//result->Insert("result", 1);

		//Send the response
		args->Request->SendResponseAsync(result);


		//Complete the message deferral so the platform knows we're done responding
		messageDeferral->Complete();
}
