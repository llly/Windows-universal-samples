#pragma once

namespace RandomNumberService
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class RandomNumberGeneratorTask sealed: public Windows::ApplicationModel::Background::IBackgroundTask
    {
	private:
		Windows::ApplicationModel::AppService::AppServiceConnection^ connection = nullptr;
		Platform::Agile<Windows::ApplicationModel::Background::BackgroundTaskDeferral> TaskDeferral = nullptr;
    public:
		virtual void Run(Windows::ApplicationModel::Background::IBackgroundTaskInstance^ taskInstance);
	protected:
		virtual void OnCanceled(Windows::ApplicationModel::Background::IBackgroundTaskInstance^ taskInstance, Windows::ApplicationModel::Background::BackgroundTaskCancellationReason reason);
		virtual void OnRequestReceived(Windows::ApplicationModel::AppService::AppServiceConnection^ sender, Windows::ApplicationModel::AppService::AppServiceRequestReceivedEventArgs^ args);
    };
}
