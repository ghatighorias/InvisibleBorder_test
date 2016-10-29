// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "DataTypes.h"
#include "MaskBrush.h"
#include "DynamicPainter.h"
#include "BlueprintDataTypes.h"
#include "MyDepthMaskComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVISIBLEBORDER_TEST_API UMyDepthMaskComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMyDepthMaskComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;


	UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
    EFunctionStateEnum Initialize(FIntPoint CanvasSize, UTexture2D*& Texture, int32 AgingStep, FFourChannelImpactRatio fourChannelImpactRatio, float ImpactRatio = 3, bool withCompression = false);
	UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
		void RegisterTexture(UTexture2D*& Texture);
	UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
		void Age();
	UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
		void AgeStepByOne();
	UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
		void UpdateMaskTexture();
	UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
		void ImpactInPosition(FIntPoint ImpactPosition);
	UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
		void SetImpactRatio(float impactRatio);
	UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
		void SetAgingStep(int32 AgingStep);
	UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
		void resetDepthMap();
	UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
	  bool IsInitializedSuccessfully();
	UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
		void CreateRectangleBrush(int32 Width, int32 Height);
	UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
		void CreateCircleBrush(int32 circleRadius, float Axx = 1, float Ayy = 1);
	UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
		EFunctionStateEnum RotateBrush(float angle, FIntPoint pivotPoint);
  UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
    void reSetBrush();

private:
	DynamicPainter* dynamicPainter;
	MaskBrush* brush;
};