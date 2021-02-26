// Copyright 2021 x.Lin. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Slate/SlateBrushAsset.h"
#include "ChartBase.h"
#include "BarChart.generated.h"


//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadEnd);
/**
 *
 */

USTRUCT(BlueprintType)
struct FBarSeries
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Series")
		FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Series")
		TArray<float> data;
	//If the two data are to be stacked together, set the same stack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Series")
		FString stack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Series")
		bool showBackground = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Series")
		FLinearColor backgroundColor = FLinearColor(0.0f,0.0f,0.0f,0.3f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Series|DataLabel")
		FSeriesLabel  DataLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|LinearGradient")
		bool UseGradientColor = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|LinearGradient")
		FLinearGradient LinearGradient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|ShadowColor")
		FShadowColor ShadowColor;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|ShadowColor")
	//	bool UseShadowColor = false;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|ShadowColor")
		
};

UCLASS()
class MECHARTS_API UBarChart : public UChartBase
{
	GENERATED_UCLASS_BODY()

public:
	virtual void OnLoadCompleted() override;
	virtual void SynchronizeProperties() override;
#if WITH_EDITOR
	//~ Begin UWidget Interface
	virtual const FText GetPaletteCategory() override;
	//~ End UWidget Interface
#endif

public:
	UFUNCTION(BlueprintCallable, Category = "Chart")
		void SetLegend(bool show, Position position, Orient orient, FLinearColor color, int32 fontSize);
	UFUNCTION(BlueprintCallable, Category = "Chart")
		void SetSeries(TArray<FBarSeries> SeriesData);

	//Series
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart|Series")
		TArray<FBarSeries> Series;

private:
	//Chart Data
	TArray<TSharedPtr<FJsonValue>> series_json;

};
