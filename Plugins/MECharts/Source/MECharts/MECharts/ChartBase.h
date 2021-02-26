// Copyright 2021 x.Lin. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "SMEWebBrowser.h"
#include "Components/Widget.h"
#include "JsonLibrary/Public/JsonLibrary.h"
#include "ChartBase.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class ChartTheme :uint8
{
	azul,
	beeinspired,
	blue,
	caravan,
	carp,
	cool,
	dark,
	darkblue,
	darkbold,
	darkdigerati,
	darkfreshcut,
	darkmushroom,
	eduardo,
	forest,
	freshcut,
	fruit,
	gray,
	green,
	helianthus,
	infographic,
	inspired,
	jazz,
	london,
	macarons,
	macarons2,
	mint,
	red,
	redvelvet,
	roma,
	royal,
	sakura,
	shine,
	techblue,
	vintage
};

UENUM(BlueprintType)
enum class Position :uint8
{
	center,
	left,
	right
};

UENUM(BlueprintType)
enum class Orient :uint8
{
	horizontal,
	vertical
};

UENUM(BlueprintType)
enum class TooltipType :uint8
{
	line,
	shadow
};

UENUM(BlueprintType)
enum class TriggerType :uint8
{
	item,
	axis
};

USTRUCT(BlueprintType)
struct FaxisLabel
{
	GENERATED_USTRUCT_BODY()
public:
	//Set to 0 to force all labels to be displayed.The larger the label, the less display.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|axisLabel")
		int32 interval = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|axisLabel")
		int32 rotate = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|axisLabel")
		FColor Textcolor = FColor(255.0f, 255.0f, 255.0f, 130.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|axisLabel")
		int32 fontSize = 15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|axisLabel")
		bool show = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|axisLabel")
		FString Unit;
public:
	FaxisLabel()
	{}

	FaxisLabel(int32 value1, int32 value2, FColor value3, int32 value4, bool value5)
		:interval(value1)
		, rotate(value2)
		, Textcolor(value3)
		, fontSize(value4)
		, show(value5)
	{}
};

UENUM(BlueprintType)
enum class SeriesLabelPosition :uint8
{
	inside,
	top,
	bottom,
	left,
	right
};

USTRUCT(BlueprintType)
struct FSeriesLabel
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|SeriesLabel")
		int32 rotate = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|SeriesLabel")
		FColor Textcolor = FColor(255.0f, 255.0f, 255.0f, 130.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|SeriesLabel")
		int32 fontSize = 15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|SeriesLabel")
		bool show = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|SeriesLabel")
		SeriesLabelPosition Position = SeriesLabelPosition::top;
};

UENUM(BlueprintType)
enum class AxisLineType :uint8
{
	solid,
	dashed,
	dotted
};

USTRUCT(BlueprintType)
struct FLineStyle
{
	GENERATED_USTRUCT_BODY()
public:
	//Set to 0 to force all labels to be displayed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|axisLine")
		bool show = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|axisLine")
		FColor color = FColor(255.0f, 255.0f, 255.0f, 255.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|axisLine")
		int32 width = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|axisLine")
		AxisLineType type = AxisLineType::solid;

public:
	FLineStyle()
	{}

	FLineStyle(bool value1, FColor value2, int32 value3, AxisLineType value4)
		:show(value1)
		, color(value2)
		, width(value3)
		, type(value4)
	{}
};

USTRUCT(BlueprintType)
struct FColorStop
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 1), Category = "Chart|LinearGradient")
		float offset = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|LinearGradient")
		FColor color = FColor(255.0f, 255.0f, 255.0f, 255.0f);
public:
	FColorStop()
	{}

	FColorStop(float value1, FColor value2)
		:offset(value1)
		, color(value2)
	{}
};

USTRUCT(BlueprintType)
struct FLinearGradient
{
	GENERATED_USTRUCT_BODY()
public:
	//This is a weight value for the color direction
	//x: 1-0 from left to right
	//y : 1 - 0 from right to left
	//z : 1 - 0 from top to bottom
	//w : 1 - 0 from bottom to top
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 1), Category = "Chart|LinearGradient")
		FVector4 ColorPosition = FVector4(0, 0, 0, 1);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|LinearGradient")
		TArray<FColorStop> colorStops = { FColorStop(0.0f,FColor(0,0,255,255)),FColorStop(1.0f,FColor(20,125,210,170)) };
};

USTRUCT(BlueprintType)
struct FShadowColor
{
	GENERATED_USTRUCT_BODY()
public:
	//Blur size of graphics shadows
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|ShadowColor")
		float ShadowBlur = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|ShadowColor")
		FColor ShadowColor = FColor(255.0f, 255.0f, 255.0f, 255.0f);
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|ShadowColor")




};

UCLASS()
class MECHARTS_API UChartBase : public UWidget
{

	GENERATED_UCLASS_BODY()
public:
	TSharedPtr<FJsonObject> SetDataLabel(FSeriesLabel Label);
	TSharedPtr<FJsonObject> SetItemStyle(FLinearGradient LinearGradient, FShadowColor ShadowColor);
	FString ColorToRGBA(FColor color);
	FString ThemeToString();
	virtual void OnLoadCompleted();
	//Update chart
	UFUNCTION(BlueprintCallable, Category = "Chart")
		void UpdateChart();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart")
		ChartTheme Theme;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart")
		bool AxisStyleWithTheme = false;
	UFUNCTION(BlueprintCallable, Category = "Chart")
		void SetTheme(ChartTheme ChartTheme, bool Axis_Style_With_Theme);

	UFUNCTION(BlueprintCallable, Category = "Chart")
		void SetTitle(FString text, bool show, Position position, int32 fontSize, FLinearColor color);
	UFUNCTION(BlueprintCallable, Category = "Chart")
		void SetAxis(bool XAxis, TArray<FString> Axis_Data);
	UFUNCTION(BlueprintCallable, Category = "Chart")
		void SetXAxisStyle(FaxisLabel  Xaxis_Label, FLineStyle  Xaxis_Line, FLineStyle  Xsplit_Line);
	UFUNCTION(BlueprintCallable, Category = "Chart")
		void SetYAxisStyle(FaxisLabel  Yaxis_Label, FLineStyle  Yaxis_Line, FLineStyle  Ysplit_Line);
	UFUNCTION(BlueprintCallable, Category = "Chart")
		void SetTooltipType(TriggerType trigger, TooltipType tooltip);

	UFUNCTION(BlueprintCallable, Category = "Chart")
		void SetBackgroundColor(FColor InBGColor);

	//title
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Title")
		FString title_text = "title";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Title")
		bool title_show = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Title")
		Position title_position = Position::center;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Title")
		int32 title_fontSize = 12;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Title")
		FColor title_color = FColor(255.0f, 255.0f, 255.0f, 255.0f);

	//legend
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Legend")
		bool legend_show = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Legend")
		Position legend_position = Position::left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Legend")
		Orient legend_orient = Orient::horizontal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Legend")
		FColor legend_color = FColor(255.0f, 255.0f, 255.0f, 255.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Legend")
		int32 legend_fontSize = 12;

	//Axis
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Axis")
		TArray<FString> AxisData = { "1","2","3" };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Axis")
		bool IsXAxis = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Axis")
		bool  boundaryGap = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Axis|XAxisStyle", meta = (EditCondition = "!AxisStyleWithTheme"))
		FaxisLabel  XaxisLabel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Axis|XAxisStyle", meta = (EditCondition = "!AxisStyleWithTheme"))
		FLineStyle  XaxisLine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Axis|XAxisStyle", meta = (EditCondition = "!AxisStyleWithTheme"))
		FLineStyle  XsplitLine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Axis|YAxisStyle", meta = (EditCondition = "!AxisStyleWithTheme"))
		FaxisLabel  YaxisLabel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Axis|YAxisStyle", meta = (EditCondition = "!AxisStyleWithTheme"))
		FLineStyle  YaxisLine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Axis|YAxisStyle", meta = (EditCondition = "!AxisStyleWithTheme"))
		FLineStyle  YsplitLine;

	//tooltip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Tooltip")
		TooltipType tooltip_type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Tooltip")
		TriggerType tooltip_trigger;

	//backgroundColor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|BackgroundColor")
		FColor BGColor;


	//Communication
	UFUNCTION(BlueprintCallable, Category = "Chart|Communication")
		void Call(const FString& Function, const FJsonLibraryValue& Data);

	UFUNCTION(BlueprintCallable, Category = "Chart|Communication")
		void JsFunction(TArray<FString> TextArray);

	// Bind an object to ue.name in the browser context.
	UFUNCTION(BlueprintCallable, Category = "Chart|Communication")
		void Bind(const FString& Name, UObject* Object);

	// Unbind an object from ue.name in the browser context.
	UFUNCTION(BlueprintCallable, Category = "Chart|Communication")
		void Unbind(const FString& Name, UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Chart|Communication")
		void ExecuteJavascript(const FString& ScriptText);

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

	//Chart
	TSharedPtr<FJsonObject> options_json;
#if WITH_EDITORONLY_DATA
	//~ Begin UObject Interface
	virtual void PostLoad() override;
	//~ End UObject Interface
#endif

protected:
	FString DefaultURL;
	FString FilePath;
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

	TSharedPtr<class SMEWebBrowser> MEWebBrowser;
	TSharedPtr<SOverlay> MyOverlay;

	//Chart Data
	TSharedPtr<FJsonObject> YaxisLabel_json;
	TSharedPtr<FJsonObject> YaxisLine_json;
	TSharedPtr<FJsonObject> YsplitLine_json;
	TSharedPtr<FJsonObject> XaxisLabel_json;
	TSharedPtr<FJsonObject> XaxisLine_json;
	TSharedPtr<FJsonObject> XsplitLine_json;
	TSharedPtr<FJsonObject> title_json;
	TSharedPtr<FJsonObject> legend_json;
	TSharedPtr<FJsonObject> Axis_json;
	TSharedPtr<FJsonObject> Tooltip_json;
};

