// Copyright 2021 x.Lin. All Rights Reserved

#include "BarChart.h"

#define LOCTEXT_NAMESPACE "UMG"

UBarChart::UBarChart(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FBarSeries Default;
	Default.data.Add(3);
	Default.data.Add(2);
	Default.data.Add(5);
	Default.name = "default";
	Series.Add(Default);
	options_json = MakeShareable(new FJsonObject);
	title_json = MakeShareable(new FJsonObject);
	legend_json = MakeShareable(new FJsonObject);
	Axis_json = MakeShareable(new FJsonObject);
	Tooltip_json = MakeShareable(new FJsonObject);
	
}

void UBarChart::OnLoadCompleted()
{
	Super::OnLoadCompleted();
	SetSeries(Series);
	SetXAxisStyle(XaxisLabel, XaxisLine, XsplitLine);
	SetYAxisStyle(YaxisLabel, YaxisLine, YsplitLine);
	//SetAxis(IsXAxis, Axis);
	SetTitle(title_text, title_show, title_position, title_fontSize, title_color);
	//SetLegend(legend_show, legend_position, legend_orient, legend_color, legend_fontSize);
	SetTooltipType(tooltip_trigger, tooltip_type);
	SetBackgroundColor(BGColor);
	UpdateChart();
}

void UBarChart::SynchronizeProperties()
{
	Super::SynchronizeProperties();

		SetSeries(Series);
		SetXAxisStyle(XaxisLabel, XaxisLine, XsplitLine);
		SetYAxisStyle(YaxisLabel, YaxisLine, YsplitLine);
		//SetAxis(IsXAxis, Axis);
		SetTitle(title_text, title_show, title_position, title_fontSize, title_color);
		//SetLegend(legend_show, legend_position, legend_orient, legend_color, legend_fontSize);
		SetTooltipType(tooltip_trigger,tooltip_type);
		SetBackgroundColor(BGColor);
		UpdateChart();
	
}

#if WITH_EDITOR
const FText UBarChart::GetPaletteCategory()
{
	return LOCTEXT("LineChart", "SimpleChart");
}
#endif

//void UBarChart::UpdateChart()
//{
//	if (WebObject)
//	{
//		//json to string and send to web
//		FString JsonStr;
//		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
//		FJsonSerializer::Serialize(options_json.ToSharedRef(), Writer);
//		WebObject->ExecuteJS("vue.SetMyOptions(" + JsonStr + ")");
//		//UE_LOG(LogTemp, Warning, TEXT("%s"), *JsonStr);
//	}
//}

void UBarChart::SetLegend(bool show, Position position, Orient orient, FLinearColor color, int32 fontSize)
{
	legend_show = show;
	legend_position = position;
	legend_orient = orient;
	legend_color = color.ToFColor(true);
	legend_fontSize = fontSize;
	if (!legend_json.IsValid())
	{
		legend_json = MakeShareable(new FJsonObject);
	}
	//legend setting
	legend_json->SetBoolField(TEXT("show"), legend_show);
	if (legend_show)
	{
		switch (legend_position)
		{
		case Position::center:
			legend_json->SetStringField(TEXT("x"), "center");
			break;
		case Position::left:
			legend_json->SetStringField(TEXT("x"), "left");
			break;
		case Position::right:
			legend_json->SetStringField(TEXT("x"), "right");
			break;
		default:
			break;
		}
		switch (legend_orient)
		{
		case Orient::horizontal:
			legend_json->SetStringField(TEXT("orient"), "horizontal");
			break;
		case Orient::vertical:
			legend_json->SetStringField(TEXT("orient"), "vertical");
			break;
		default:
			break;
		}
		TArray<TSharedPtr<FJsonValue>> legend_data;
		for (auto Serie : Series)
		{
			legend_data.Add(MakeShareable(new FJsonValueString(Serie.name)));
		}
		legend_json->SetArrayField(TEXT("data"), legend_data);
		TSharedPtr<FJsonObject> legend_textStyle = MakeShareable(new FJsonObject);;
		legend_textStyle->SetNumberField(TEXT("fontSize"), legend_fontSize);
		legend_textStyle->SetStringField(TEXT("color"), ColorToRGBA(legend_color));
		legend_json->SetObjectField(TEXT("textStyle"), legend_textStyle);
	}
	if (!options_json.IsValid())
	{
		options_json = MakeShareable(new FJsonObject);
	}
	options_json->SetObjectField(TEXT("legend"), legend_json);
	//UpdateChart();
}

void UBarChart::SetSeries(TArray<FBarSeries> SeriesData)
{
	Series = SeriesData;
	//Series setting
	series_json.Reset();
	for (auto Serie : Series)
	{
		TSharedPtr<FJsonObject> temp = MakeShareable(new FJsonObject);
	
 		temp->SetStringField(TEXT("name"), Serie.name);
		TArray<TSharedPtr<FJsonValue>> tempdata;
		for (auto d : Serie.data)
		{
			tempdata.Add(MakeShareable(new FJsonValueNumber(d)));
		}

		//Set Serie Label
		temp->SetObjectField(TEXT("label"), SetDataLabel(Serie.DataLabel));

		//Set itemStyle
		if (Serie.UseGradientColor)
		{
			temp->SetObjectField(TEXT("itemStyle"), SetItemStyle(Serie.LinearGradient,Serie.ShadowColor));
		}
	
		temp->SetBoolField(TEXT("showBackground"), Serie.showBackground);
		TSharedPtr<FJsonObject> backgroundStyle = MakeShareable(new FJsonObject);
		FString Color = ColorToRGBA(Serie.backgroundColor.ToFColor(false));
		backgroundStyle->SetStringField(TEXT("color"), Color);
		temp->SetObjectField(TEXT("backgroundStyle"), backgroundStyle);
		temp->SetArrayField(TEXT("data"), tempdata);
		temp->SetStringField(TEXT("type"), TEXT("bar"));	
		temp->SetStringField(TEXT("stack"), Serie.stack);
		series_json.Add(MakeShareable(new FJsonValueObject(temp)));
	}
	if (!options_json.IsValid())
	{
		options_json = MakeShareable(new FJsonObject);
	}
	options_json->SetArrayField(TEXT("series"), series_json);
	SetLegend(legend_show, legend_position, legend_orient, legend_color, legend_fontSize);
	//UpdateChart();
}

#undef LOCTEXT_NAMESPACE
