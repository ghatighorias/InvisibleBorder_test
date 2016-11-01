// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 *
 */


#include "DataTypes.h"
#include "MaskBrush.h"

#ifndef DYNAMICPAINTER_H
#define DYNAMICPAINTER_H

class INVISIBLEBORDER_TEST_API DynamicPainter
{

public:
	DynamicPainter(FIntPoint CanvasSize);
	~DynamicPainter();
	void Initialize(UTexture2D*& Texture, MaskBrush* Brush, int32 AgingStep,
		float ImpactRatio, ChanneledImpactRatio* channeledImpactRatio, bool withCompression=false);
	void setBrush(MaskBrush* Brush);
	void registerTexture(UTexture2D*& Texture, bool withCompression=false);
	void setAgingStep(int32 AgingStep);
	void setImpactRatio(float ImpactRatio);
	void addImpactInPosition(int X, int Y);
	void age();
	void ageStepByOne();
	void updateMaskBuffer();
  void resetBufferedDynamicMask();
  bool isInitializedSuccessfully();
  void setChanneledImpactRatio(ChanneledImpactRatio* ChanneledImpactRatio);

private:
	MaskBrush* brush;
  FIntPoint brushSize;
	uint32* bufferedDynamicMask;
	int32   bufferSize;
	UTexture2D* dynamicMask;

	int32 agingStep;
	float impactRatio;
  FIntPoint canvasSize;
	ChanneledImpactRatio* channeledImpactRatio;

	void freeCanvas();
	void setUpCanvas(FIntPoint CanvasSize);

	int Add_with_cap_255(int currentValue, int& increment, int multiplier);
	uint32 Add_to_quad(uint32* currentValue, int incrementStep, ChanneledImpactRatio* channeledImpactRatio);
	inline uint32 Sub_from_quad(uint32* currentValue, int decreamentStep);
	inline int calculate_impact_ratio_in_position(FIntPoint brushSize, float impactRatio, int X, int Y, int currentIndexX, int currentIndexY);

};

#endif