// Copyright 2021 x.Lin. All Rights Reserved



#include "ChartBase.h"
#include "Widgets/Images/SThrobber.h"
#include "Misc/FileHelper.h"

#define LOCTEXT_NAMESPACE "UMG"

UChartBase::UChartBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// default Set
	XsplitLine = FLineStyle(false, FColor(255.0f, 255.0f, 255.0f, 130.0f), 1, AxisLineType::dashed);
	XaxisLine = FLineStyle(true, FColor(255.0f, 255.0f, 255.0f, 255.0f), 1, AxisLineType::solid);
	XaxisLabel = FaxisLabel(0, 0, FColor(255.0f, 255.0f, 255.0f, 255.0f), 15, true);
	YsplitLine = FLineStyle(true, FColor(255.0f, 255.0f, 255.0f, 130.0f), 1, AxisLineType::dashed);
	YaxisLine = FLineStyle(true, FColor(255.0f, 255.0f, 255.0f, 255.0f), 1, AxisLineType::solid);
	YaxisLabel = FaxisLabel(0, 0, FColor(255.0f, 255.0f, 255.0f, 255.0f), 15, true);
	BGColor = FColor(0.0f, 0.0f, 0.0f, 0.0f);

	// HtmlPath
	FilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir() + "MECharts/Source/ThirdParty/html/");
	DefaultURL = "file:///" + FilePath + "index.html#/Chart";

	//OptionConfigure
	options_json = MakeShareable(new FJsonObject);
	YaxisLabel_json = MakeShareable(new FJsonObject);
	YaxisLine_json = MakeShareable(new FJsonObject);
	YsplitLine_json = MakeShareable(new FJsonObject);
	XaxisLabel_json = MakeShareable(new FJsonObject);
	XaxisLine_json = MakeShareable(new FJsonObject);
	XsplitLine_json = MakeShareable(new FJsonObject);
	title_json = MakeShareable(new FJsonObject);
	legend_json = MakeShareable(new FJsonObject);
	Axis_json = MakeShareable(new FJsonObject);
	Tooltip_json = MakeShareable(new FJsonObject);
}

void UChartBase::Call(const FString& Function, const FJsonLibraryValue& Data)
{
	if (!MEWebBrowser.IsValid())
		return;

	if (Data.GetType() != EJsonLibraryType::Invalid)
	{
		FString CurFunction = FString::Printf(TEXT("ue.interface[%s](%s)"), *FJsonLibraryValue(Function).Stringify(), *Data.Stringify());
		MEWebBrowser->ExecuteJavascript(FString::Printf(TEXT("ue.interface[%s](%s)"),
			*FJsonLibraryValue(Function).Stringify(),
			*Data.Stringify()));
	}
		//MEWebBrowser->ExecuteJavascript(CurFunction);
	else
		MEWebBrowser->ExecuteJavascript(FString::Printf(TEXT("ue.interface[%s]()"),
			*FJsonLibraryValue(Function).Stringify()));
}

void UChartBase::JsFunction(TArray<FString> TextArray)
{
	FilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir() + "MECharts/Source/ThirdParty/html/static/js/JsFunction.js");
	FFileHelper::SaveStringArrayToFile(TextArray, *FilePath);
	MEWebBrowser->LoadURL(DefaultURL);
	
}

void UChartBase::Bind(const FString& Name, UObject* Object)
{
	if (!Object)
		return;
	if (MEWebBrowser.IsValid())
		MEWebBrowser->BindUObject(Name, Object);
}

void UChartBase::Unbind(const FString& Name, UObject* Object)
{
	if (!Object)
		return;
	if (MEWebBrowser.IsValid())
		MEWebBrowser->UnbindUObject(Name, Object);
}

void UChartBase::ExecuteJavascript(const FString& ScriptText)
{
	if (MEWebBrowser.IsValid())
	{
		MEWebBrowser->ExecuteJavascript(ScriptText);
	}
}

void UChartBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

TSharedPtr<FJsonObject> UChartBase::SetDataLabel(FSeriesLabel Label)
{
	//Set Serie Label
	TSharedPtr<FJsonObject> DataLabel_json = MakeShareable(new FJsonObject);
	if (Label.show)
	{
		DataLabel_json->SetBoolField(TEXT("show"), Label.show);
		DataLabel_json->SetNumberField(TEXT("rotate"), Label.rotate);
		switch (Label.Position) {
		default:
			break;
		case SeriesLabelPosition::inside:
			DataLabel_json->SetStringField(TEXT("position"), TEXT("inside"));
			break;
		case SeriesLabelPosition::bottom:
			DataLabel_json->SetStringField(TEXT("position"), TEXT("bottom"));
			break;
		case SeriesLabelPosition::top:
			DataLabel_json->SetStringField(TEXT("position"), TEXT("top"));
			break;
		case SeriesLabelPosition::left:
			DataLabel_json->SetStringField(TEXT("position"), TEXT("left"));
			break;
		case SeriesLabelPosition::right:
			DataLabel_json->SetStringField(TEXT("position"), TEXT("right"));
			break;
		}
		TSharedPtr<FJsonObject> textStyle_json = MakeShareable(new FJsonObject);
		textStyle_json->SetNumberField(TEXT("fontSize"), Label.fontSize);
		textStyle_json->SetStringField(TEXT("color"), ColorToRGBA(Label.Textcolor));
		DataLabel_json->SetObjectField(TEXT("textStyle"), textStyle_json);
	}
	return DataLabel_json;
}

TSharedPtr<FJsonObject> UChartBase::SetItemStyle(FLinearGradient LinearGradient, FShadowColor ShadowColor)
{
	TSharedPtr<FJsonObject> LinearGradient_json = MakeShareable(new FJsonObject);
	TSharedPtr<FJsonObject> Color_json = MakeShareable(new FJsonObject);
	
	Color_json->SetNumberField(TEXT("x"), LinearGradient.ColorPosition.X);
	Color_json->SetNumberField(TEXT("x2"), LinearGradient.ColorPosition.Y);
	Color_json->SetNumberField(TEXT("y"), LinearGradient.ColorPosition.Z);
	Color_json->SetNumberField(TEXT("y2"), LinearGradient.ColorPosition.W);
	TArray<TSharedPtr<FJsonValue>> colorStops_json;
	for (auto c : LinearGradient.colorStops)
	{
		TSharedPtr<FJsonObject> temp = MakeShareable(new FJsonObject);
		temp->SetNumberField(TEXT("offset"), c.offset);
		temp->SetStringField(TEXT("color"), ColorToRGBA(c.color));
		colorStops_json.Add(MakeShareable(new FJsonValueObject(temp)));
	}
	Color_json->SetArrayField(TEXT("colorStops"), colorStops_json);
	LinearGradient_json->SetObjectField(TEXT("color"), Color_json);
	LinearGradient_json->SetStringField(TEXT("shadowColor"), ColorToRGBA(ShadowColor.ShadowColor));
	LinearGradient_json->SetNumberField(TEXT("shadowBlur"), ShadowColor.ShadowBlur);
	return  LinearGradient_json;
}

FString UChartBase::ColorToRGBA(FColor color)
{
	FString SColor;
	float A = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 255.0f), FVector2D(0.0f, 1.0f), color.A);
	SColor = "rgba(" + FString::FromInt(color.R) + "," + FString::FromInt(color.G) + "," + FString::FromInt(color.B) + "," + FString::SanitizeFloat(A) + ")";
	return SColor;
}

void UChartBase::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyOverlay.Reset();
	MEWebBrowser.Reset();

	options_json.Reset();
	YaxisLabel_json.Reset();
	YaxisLine_json.Reset();
	YsplitLine_json.Reset();
	XaxisLabel_json.Reset();
	XaxisLine_json.Reset();
	XsplitLine_json.Reset();
	title_json.Reset();
	legend_json.Reset();
	Axis_json.Reset();
	Tooltip_json.Reset();
}

void UChartBase::SetTheme(ChartTheme ChartTheme, bool Axis_Style_With_Theme)
{
	Theme = ChartTheme;
	AxisStyleWithTheme = Axis_Style_With_Theme;
	SetAxis(IsXAxis, AxisData);
}

#if WITH_EDITORONLY_DATA
void UChartBase::PostLoad()
{
	Super::PostLoad();
}
#endif

void UChartBase::OnLoadCompleted()
{
#if PLATFORM_ANDROID || PLATFORM_IOS

#else 
	if (MEWebBrowser.IsValid() && MyOverlay.IsValid() && MyOverlay->GetChildren())
	{
		//MyOverlay->GetChildren()->GetChildAt(0).Get().SetVisibility(EVisibility::Hidden);
		//loadend.Broadcast();
	}
#endif
}

FString UChartBase::ThemeToString()
{
	FString Theme_str;
	switch (Theme)
	{
	default:
		break;
	case ChartTheme::azul:
		Theme_str = "azul";
		break;
	case ChartTheme::beeinspired:
		Theme_str = "bee-inspired";
		break;
	case ChartTheme::blue:
		Theme_str = "blue";
		break;
	case ChartTheme::caravan:
		Theme_str = "caravan";
		break;
	case ChartTheme::carp:
		Theme_str = "carp";
		break;
	case ChartTheme::cool:
		Theme_str = "cool";
		break;
	case ChartTheme::dark:
		Theme_str = "dark";
		break;
	case ChartTheme::darkblue:
		Theme_str = "dark-blue";
		break;
	case ChartTheme::darkbold:
		Theme_str = "dark-bold";
		break;
	case ChartTheme::darkdigerati:
		Theme_str = "dark-digerati";
		break;
	case ChartTheme::darkfreshcut:
		Theme_str = "dark-fresh-cut";
		break;
	case ChartTheme::darkmushroom:
		Theme_str = "dark-mushroom";
		break;
	case ChartTheme::eduardo:
		Theme_str = "eduardo";
		break;
	case ChartTheme::forest:
		Theme_str = "forest";
		break;
	case ChartTheme::freshcut:
		Theme_str = "fresh-cut";
		break;
	case ChartTheme::fruit:
		Theme_str = "fruit";
		break;
	case ChartTheme::gray:
		Theme_str = "gray";
		break;
	case ChartTheme::green:
		Theme_str = "green";
		break;
	case ChartTheme::helianthus:
		Theme_str = "helianthus";
		break;
	case ChartTheme::infographic:
		Theme_str = "infographic";
		break;
	case ChartTheme::inspired:
		Theme_str = "inspired";
		break;
	case ChartTheme::jazz:
		Theme_str = "jazz";
		break;
	case ChartTheme::london:
		Theme_str = "london";
		break;
	case ChartTheme::macarons:
		Theme_str = "macarons";
		break;
	case ChartTheme::macarons2:
		Theme_str = "macarons2";
		break;
	case ChartTheme::mint:
		Theme_str = "mint";
		break;
	case ChartTheme::red:
		Theme_str = "red";
		break;
	case ChartTheme::redvelvet:
		Theme_str = "red-velvet";
		break;
	case ChartTheme::roma:
		Theme_str = "roma";
		break;
	case ChartTheme::royal:
		Theme_str = "royal";
		break;
	case ChartTheme::sakura:
		Theme_str = "sakura";
		break;
	case ChartTheme::shine:
		Theme_str = "shine";
		break;
	case ChartTheme::techblue:
		Theme_str = "tech-blue";
		break;
	case ChartTheme::vintage:
		Theme_str = "vintage";
		break;
	}
	return Theme_str;
}

void UChartBase::UpdateChart()
{
	if (options_json.IsValid() && MEWebBrowser.IsValid())
	{
		//json to string and send to web
		FString JsonStr;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
		FJsonSerializer::Serialize(options_json.ToSharedRef(), Writer);
		FString t = "\"";

		if (!MEWebBrowser.IsValid())
		{
			return;
		}
		MEWebBrowser->ExecuteJavascript("myChart.setOption(" + JsonStr + "," + t + ThemeToString() + t + ")");
		//UE_LOG(LogTemp, Warning, TEXT("Option =   %s"), *JsonStr);
	}
}

TSharedRef<SWidget> UChartBase::RebuildWidget()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	MEWebBrowser = SNew(SMEWebBrowser)
		.InitialURL(DefaultURL)
		.ShowControls(false)
		.SupportsTransparency(true)
		.OnLoadCompleted(BIND_UOBJECT_DELEGATE(FSimpleDelegate, OnLoadCompleted));

	MyOverlay = SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SBox)
			.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			MEWebBrowser.ToSharedRef()
		]
		];
#else
	MEWebBrowser = SNew(SMEWebBrowser)
		.InitialURL(DefaultURL)
		.SupportsTransparency(true)
		.OnLoadCompleted(BIND_UOBJECT_DELEGATE(FSimpleDelegate, OnLoadCompleted));

	MyOverlay = SNew(SOverlay)
		//+ SOverlay::Slot()
		//.HAlign(HAlign_Fill)
		//.VAlign(VAlign_Fill)
		//[
		//	SNew(SBox)
		//	.HAlign(HAlign_Center)
		//.VAlign(VAlign_Center)
		//[
		//	SNew(SCircularThrobber)
		//]
		//]
	+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SBox)
			.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			MEWebBrowser.ToSharedRef()
		]
		];
#endif


	return MyOverlay.ToSharedRef();
}


void UChartBase::SetTitle(FString text, bool show, Position position, int32 fontSize, FLinearColor color)
{
	title_text = text;
	title_show = show;
	title_position = position;
	title_fontSize = fontSize;
	title_color = color.ToFColor(true);
	if (!title_json.IsValid())
	{
		title_json = MakeShareable(new FJsonObject);
	}
	//title setting
	title_json->SetBoolField(TEXT("show"), title_show);
	if (title_show)
	{
		title_json->SetStringField(TEXT("text"), title_text);
		switch (title_position)
		{
		case Position::center:
			title_json->SetStringField(TEXT("x"), "center");
			break;
		case Position::left:
			title_json->SetStringField(TEXT("x"), "left");
			break;
		case Position::right:
			title_json->SetStringField(TEXT("x"), "right");
			break;
		default:
			break;
		}
		TSharedPtr<FJsonObject> title_textStyle = MakeShareable(new FJsonObject);;
		title_textStyle->SetNumberField(TEXT("fontSize"), title_fontSize);
		title_textStyle->SetStringField(TEXT("color"), ColorToRGBA(title_color));
		title_json->SetObjectField(TEXT("textStyle"), title_textStyle);
	}
	if (!options_json.IsValid())
	{
		options_json = MakeShareable(new FJsonObject);
	}
	options_json->SetObjectField(TEXT("title"), title_json);
	//UpdateChart();
}

void UChartBase::SetAxis(bool XAxis, TArray<FString> Axis_Data)
{
	AxisData = Axis_Data;
	IsXAxis = XAxis;
	Axis_json.Reset();
	Axis_json = MakeShareable(new FJsonObject);
	if (!options_json.IsValid())
	{
		options_json = MakeShareable(new FJsonObject);
	}
	//Axis setting
	TArray<TSharedPtr<FJsonValue>> EntriesArray;
	for (auto String : AxisData)
	{
		EntriesArray.Add(MakeShareable(new FJsonValueString(String)));
	}
	Axis_json->SetArrayField(TEXT("data"), EntriesArray);
	Axis_json->SetBoolField(TEXT("boundaryGap"), boundaryGap);
	if (IsXAxis)
	{
		TSharedPtr<FJsonObject> YAxis_json = MakeShareable(new FJsonObject);
		if (!AxisStyleWithTheme)
		{
			Axis_json->SetObjectField(TEXT("axisLabel"), XaxisLabel_json);
			Axis_json->SetObjectField(TEXT("axisLine"), XaxisLine_json);
			Axis_json->SetObjectField(TEXT("splitLine"), XsplitLine_json);
			YAxis_json->SetObjectField(TEXT("axisLabel"), YaxisLabel_json);
			YAxis_json->SetObjectField(TEXT("axisLine"), YaxisLine_json);
			YAxis_json->SetObjectField(TEXT("splitLine"), YsplitLine_json);
		}
		options_json->SetObjectField(TEXT("yAxis"), YAxis_json);
		options_json->SetObjectField(TEXT("xAxis"), Axis_json);
	}
	else {
		TSharedPtr<FJsonObject> XAxis_json = MakeShareable(new FJsonObject);
		if (!AxisStyleWithTheme)
		{
			Axis_json->SetObjectField(TEXT("axisLabel"), YaxisLabel_json);
			Axis_json->SetObjectField(TEXT("axisLine"), YaxisLine_json);
			Axis_json->SetObjectField(TEXT("splitLine"), YsplitLine_json);
			XAxis_json->SetObjectField(TEXT("axisLabel"), XaxisLabel_json);
			XAxis_json->SetObjectField(TEXT("axisLine"), XaxisLine_json);
			XAxis_json->SetObjectField(TEXT("splitLine"), XsplitLine_json);
		}
		options_json->SetObjectField(TEXT("xAxis"), XAxis_json);
		options_json->SetObjectField(TEXT("yAxis"), Axis_json);
	}
	//UpdateChart();
}

void UChartBase::SetXAxisStyle(FaxisLabel Xaxis_Label, FLineStyle Xaxis_Line, FLineStyle Xsplit_Line)
{
	XaxisLabel = Xaxis_Label;
	XaxisLine = Xaxis_Line;
	XsplitLine = Xsplit_Line;
	if (!XaxisLabel_json.IsValid())
	{
		XaxisLabel_json = MakeShareable(new FJsonObject);
	}
	//axisLabel
	XaxisLabel_json->SetBoolField(TEXT("show"), XaxisLabel.show);
	XaxisLabel_json->SetNumberField(TEXT("interval"), XaxisLabel.interval);
	XaxisLabel_json->SetNumberField(TEXT("rotate"), XaxisLabel.rotate);
	XaxisLabel_json->SetStringField(TEXT("formatter"), TEXT("{value}") + XaxisLabel.Unit);
	TSharedPtr<FJsonObject> textStyle_json = MakeShareable(new FJsonObject);
	textStyle_json->SetNumberField(TEXT("fontSize"), XaxisLabel.fontSize);
	XaxisLabel_json->SetObjectField(TEXT("textStyle"), textStyle_json);
	textStyle_json->SetStringField(TEXT("color"), ColorToRGBA(XaxisLabel.Textcolor));

	if (!XaxisLine_json.IsValid())
	{
		XaxisLine_json = MakeShareable(new FJsonObject);
	}
	//axisLine
	XaxisLine_json->SetBoolField(TEXT("show"), XaxisLine.show);
	TSharedPtr<FJsonObject> lineStyle_json = MakeShareable(new FJsonObject);
	lineStyle_json->SetNumberField(TEXT("width"), XaxisLine.width);
	lineStyle_json->SetStringField(TEXT("color"), ColorToRGBA(XaxisLine.color));
	switch (XaxisLine.type)
	{
	default:
		break;
	case AxisLineType::solid:
		lineStyle_json->SetStringField(TEXT("type"), TEXT("solid"));
		break;
	case AxisLineType::dashed:
		lineStyle_json->SetStringField(TEXT("type"), TEXT("dashed"));
		break;
	case AxisLineType::dotted:
		lineStyle_json->SetStringField(TEXT("type"), TEXT("dotted"));
		break;
	}
	XaxisLine_json->SetObjectField(TEXT("lineStyle"), lineStyle_json);

	if (!XsplitLine_json.IsValid())
	{
		XsplitLine_json = MakeShareable(new FJsonObject);
	}
	//splitLine
	XsplitLine_json->SetBoolField(TEXT("show"), XsplitLine.show);
	TSharedPtr<FJsonObject> lineStyleSplit_json = MakeShareable(new FJsonObject);
	lineStyleSplit_json->SetStringField(TEXT("color"), ColorToRGBA(XsplitLine.color));
	lineStyleSplit_json->SetNumberField(TEXT("width"), XsplitLine.width);
	switch (XsplitLine.type)
	{
	default:
		break;
	case AxisLineType::solid:
		lineStyleSplit_json->SetStringField(TEXT("type"), TEXT("solid"));
		break;
	case AxisLineType::dashed:
		lineStyleSplit_json->SetStringField(TEXT("type"), TEXT("dashed"));
		break;
	case AxisLineType::dotted:
		lineStyleSplit_json->SetStringField(TEXT("type"), TEXT("dotted"));
		break;
	}
	XsplitLine_json->SetObjectField(TEXT("lineStyle"), lineStyleSplit_json);

	SetAxis(IsXAxis, AxisData);
}

void UChartBase::SetYAxisStyle(FaxisLabel Yaxis_Label, FLineStyle Yaxis_Line, FLineStyle Ysplit_Line)
{
	YaxisLabel = Yaxis_Label;
	YaxisLine = Yaxis_Line;
	YsplitLine = Ysplit_Line;
	if (!YaxisLabel_json.IsValid())
	{
		YaxisLabel_json = MakeShareable(new FJsonObject);
	}
	//axisLabel
	YaxisLabel_json->SetBoolField(TEXT("show"), YaxisLabel.show);
	YaxisLabel_json->SetNumberField(TEXT("interval"), YaxisLabel.interval);
	YaxisLabel_json->SetNumberField(TEXT("rotate"), YaxisLabel.rotate);
	YaxisLabel_json->SetStringField(TEXT("formatter"), TEXT("{value}") + YaxisLabel.Unit);
	TSharedPtr<FJsonObject> textStyle_json = MakeShareable(new FJsonObject);
	textStyle_json->SetNumberField(TEXT("fontSize"), YaxisLabel.fontSize);
	YaxisLabel_json->SetObjectField(TEXT("textStyle"), textStyle_json);
	textStyle_json->SetStringField(TEXT("color"), ColorToRGBA(YaxisLabel.Textcolor));

	if (!YaxisLine_json.IsValid())
	{
		YaxisLine_json = MakeShareable(new FJsonObject);
	}
	//axisLine
	TSharedPtr<FJsonObject> lineStyle_json = MakeShareable(new FJsonObject);
	lineStyle_json->SetNumberField(TEXT("width"), YaxisLine.width);
	lineStyle_json->SetStringField(TEXT("color"), ColorToRGBA(YaxisLine.color));
	switch (YaxisLine.type)
	{
	default:
		break;
	case AxisLineType::solid:
		lineStyle_json->SetStringField(TEXT("type"), TEXT("solid"));
		break;
	case AxisLineType::dashed:
		lineStyle_json->SetStringField(TEXT("type"), TEXT("dashed"));
		break;
	case AxisLineType::dotted:
		lineStyle_json->SetStringField(TEXT("type"), TEXT("dotted"));
		break;
	}
	YaxisLine_json->SetObjectField(TEXT("lineStyle"), lineStyle_json);

	if (!YsplitLine_json.IsValid())
	{
		YsplitLine_json = MakeShareable(new FJsonObject);
	}
	//splitLine
	YsplitLine_json->SetBoolField(TEXT("show"), YsplitLine.show);
	TSharedPtr<FJsonObject> lineStyleSplit_json = MakeShareable(new FJsonObject);
	lineStyleSplit_json->SetNumberField(TEXT("width"), YsplitLine.width);
	lineStyleSplit_json->SetStringField(TEXT("color"), ColorToRGBA(YsplitLine.color));
	switch (YsplitLine.type)
	{
	default:
		break;
	case AxisLineType::solid:
		lineStyleSplit_json->SetStringField(TEXT("type"), TEXT("solid"));
		break;
	case AxisLineType::dashed:
		lineStyleSplit_json->SetStringField(TEXT("type"), TEXT("dashed"));
		break;
	case AxisLineType::dotted:
		lineStyleSplit_json->SetStringField(TEXT("type"), TEXT("dotted"));
		break;
	}
	YsplitLine_json->SetObjectField(TEXT("lineStyle"), lineStyleSplit_json);

	SetAxis(IsXAxis, AxisData);
}

void UChartBase::SetTooltipType(TriggerType trigger, TooltipType tooltip)
{
	tooltip_type = tooltip;
	tooltip_trigger = trigger;
	TSharedPtr<FJsonObject> axisPointer_json = MakeShareable(new FJsonObject);
	switch (tooltip_type)
	{
	case TooltipType::line:
		axisPointer_json->SetStringField(TEXT("type"), TEXT("line"));
		break;
	case TooltipType::shadow:
		axisPointer_json->SetStringField(TEXT("type"), TEXT("shadow"));
		break;
	default:
		break;
	}
	switch (tooltip_trigger)
	{
	case TriggerType::axis:
		Tooltip_json->SetStringField(TEXT("trigger"), TEXT("axis"));
		break;
	case TriggerType::item:
		Tooltip_json->SetStringField(TEXT("trigger"), TEXT("item"));
		break;
	default:
		break;
	}
	if (!Tooltip_json.IsValid())
	{
		Tooltip_json = MakeShareable(new FJsonObject);
	}
	if (!options_json.IsValid())
	{
		options_json = MakeShareable(new FJsonObject);
	}
	Tooltip_json->SetObjectField(TEXT("axisPointer"), axisPointer_json);
	options_json->SetObjectField(TEXT("tooltip"), Tooltip_json);
}

void UChartBase::SetBackgroundColor(FColor InBGColor)
{
	BGColor = InBGColor;
	options_json->SetStringField(TEXT("backgroundColor"), ColorToRGBA(BGColor));
}

#undef LOCTEXT_NAMESPACE
