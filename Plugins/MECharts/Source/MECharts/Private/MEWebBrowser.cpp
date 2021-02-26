// Copyright 2021 x.Lin. All Rights Reserved

#include "MEWebBrowser.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Async/TaskGraphInterfaces.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Slate/Public/Framework/Application/SlateApplication.h"

#if WITH_EDITOR
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialExpressionMaterialFunctionCall.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "Materials/MaterialExpressionTextureSampleParameter2D.h"
#include "Materials/MaterialFunction.h"
#include "Factories/MaterialFactoryNew.h"
#include "AssetRegistryModule.h"
#include "PackageHelperFunctions.h"
#endif

#define LOCTEXT_NAMESPACE "MEWebBrowser"


UMEWebBrowser::UMEWebBrowser(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = true;
}


void UMEWebBrowser::LoadURL(FString NewURL)
{
	if (WebBrowserWidget.IsValid())
	{
		return WebBrowserWidget->LoadURL(NewURL);
	}
}

void UMEWebBrowser::LoadString(FString Contents, FString DummyURL)
{
	if (WebBrowserWidget.IsValid())
	{
		return WebBrowserWidget->LoadString(Contents, DummyURL);
	}
}

void UMEWebBrowser::ExecuteJavascript(const FString& ScriptText)
{
	if (WebBrowserWidget.IsValid())
	{
		return WebBrowserWidget->ExecuteJavascript(ScriptText);
	}
}

FText UMEWebBrowser::GetTitleText() const
{
	if (WebBrowserWidget.IsValid())
	{
		return WebBrowserWidget->GetTitleText();
	}

	return FText::GetEmpty();
}

FString UMEWebBrowser::GetUrl() const
{
	if (WebBrowserWidget.IsValid())
	{
		return WebBrowserWidget->GetUrl();
	}

	return FString();
}

void UMEWebBrowser::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	WebBrowserWidget.Reset();
}

void UMEWebBrowser::Call(const FString& Function, const FJsonLibraryValue& Data)
{

	if (!WebBrowserWidget.IsValid())
		return;
	FString CurString = FString::Printf(TEXT("ue.interface[%s](%s)"),
		*FJsonLibraryValue(Function).Stringify(),
		*Data.Stringify());
	if (Data.GetType() != EJsonLibraryType::Invalid)
		WebBrowserWidget->ExecuteJavascript(FString::Printf(TEXT("ue.interface[%s](%s)"),
			*FJsonLibraryValue(Function).Stringify(),
			*Data.Stringify()));
	else
		WebBrowserWidget->ExecuteJavascript(FString::Printf(TEXT("ue.interface[%s]()"),
			*FJsonLibraryValue(Function).Stringify()));

}

TSharedRef<SWidget> UMEWebBrowser::RebuildWidget()
{
	if (IsDesignTime())
	{
		return SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("MEWeb Browser", "MEWeb Browser"))
			];
	}
	else
	{
		WebBrowserWidget = SNew(SMEWebBrowser)
			.InitialURL(InitialURL)
			//.ShowControls(false)
			.SupportsTransparency(bSupportsTransparency)
			.OnUrlChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnUrlChanged))
			.OnBeforePopup(BIND_UOBJECT_DELEGATE(FOnBeforePopupDelegate, HandleOnBeforePopup));

		return WebBrowserWidget.ToSharedRef();
	}
}

void UMEWebBrowser::Bind(const FString& Name, UObject* Object)
{
	if (!Object)
		return;
	if (WebBrowserWidget.IsValid())
		WebBrowserWidget->BindUObject(Name, Object);
}

void UMEWebBrowser::Unbind(const FString& Name, UObject* Object)
{
	if (!Object)
		return;
	if (WebBrowserWidget.IsValid())
		WebBrowserWidget->UnbindUObject(Name, Object);
}

void UMEWebBrowser::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (WebBrowserWidget.IsValid())
	{

	}
}

void UMEWebBrowser::HandleOnUrlChanged(const FText& InText)
{
	OnUrlChanged.Broadcast(InText);
}

bool UMEWebBrowser::HandleOnBeforePopup(FString URL, FString Frame)
{
	if (OnBeforePopup.IsBound())
	{
		if (IsInGameThread())
		{
			OnBeforePopup.Broadcast(URL, Frame);
		}
		else
		{
			// Retry on the GameThread.
			TWeakObjectPtr<UMEWebBrowser> WeakThis = this;
			FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, URL, Frame]()
				{
					if (WeakThis.IsValid())
					{
						WeakThis->HandleOnBeforePopup(URL, Frame);
					}
				}, TStatId(), nullptr, ENamedThreads::GameThread);
		}

		return true;
	}

	return false;
}

#if WITH_EDITOR

const FText UMEWebBrowser::GetPaletteCategory()
{
	return LOCTEXT("MEWeb", "MEWeb");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
