// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 *
 */


#include "DataTypes.h"
#include "MaskBrush.h"

class INVISIBLEBORDER_TEST_API DynamicPainter
{

public:
	DynamicPainter(FrameSize CanvasSize);
	~DynamicPainter();
	void Initialize(UTexture2D*& Texture, MaskBrush* Brush, int32 AgingStep,
		float ImpactRatio, ChanneledImpactRatio* channeledImpactRatio, bool withCompression=false);
	// it is better that odd numbers are given
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
private:
	MaskBrush* brush;
	FrameSize brushSize;
	uint32* bufferedDynamicMask;
	int32   bufferSize;
	UTexture2D* dynamicMask;

	int32 agingStep;
	float impactRatio;
	FrameSize canvasSize;
	ImpactRange impactRange;
	ChanneledImpactRatio* channeledImpactRatio;

	void freeCanvas();
	void setUpCanvas(FrameSize CanvasSize);
	void set_mask_impact_index(int32 X, int32 Y);

	inline bool is_position_in_range(int32 X, int32 Y);
	int Add_with_cap_255(int currentValue, int& increment, int multiplier);
	uint32 Add_to_quad(uint32* currentValue, int incrementStep, ChanneledImpactRatio* channeledImpactRatio);
	inline uint32 Sub_from_quad(uint32* currentValue, int decreamentStep);
	inline int calculate_impact_ratio_in_position(FIntPoint brushSize, float impactRatio, int X, int Y, int currentIndexX, int currentIndexY);

};