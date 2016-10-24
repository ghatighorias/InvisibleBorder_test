// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 *
 */

#include <math.h> 

#define PI 3.14159265

struct ImpactRange
{
	int32 HorizentalLowerBoundary;
	int32 HorizentalUpperBoundary;
	int32 VerticalLowerBoundary;
	int32 VerticalUpperBoundary;
	ImpactRange() :HorizentalLowerBoundary(0),
		HorizentalUpperBoundary(0),
		VerticalLowerBoundary(0),
		VerticalUpperBoundary(0)
	{

	}
};

struct FrameSize
{
	int32 X;
	int32 Y;

	FrameSize():X(0),Y(0){}
	FrameSize(int x, int y):X(x),Y(y){}
	FrameSize(FVector2D size)
	{
		X = (int)size.X;
		Y = (int)size.Y;
	}
};



class MaskBrush
{
protected:
	void resetBuffer(int size)
	{
		if (brushBuffer)
			delete[] brushBuffer;
		bufferSize = size;
		brushBuffer = new int[size];
		for (int index = 0; index< bufferSize; index++)
			brushBuffer[index] = 0;
	}

	void fillHoles(int* buffer, int cubeSize)
	{
		if (!buffer)
			return;

		for (int y = 1; y< cubeSize - 1; y++)
			for (int x = 1; x< cubeSize - 1; x++)
			{
				if (buffer[y * cubeSize + x] == 0 &&
					buffer[(y + 1) * cubeSize + (x + 0)] == 1 &&
					buffer[(y - 1) * cubeSize + (x + 0)] == 1 &&
					buffer[(y + 0) * cubeSize + (x + 1)] == 1 &&
					buffer[(y + 0) * cubeSize + (x - 1)] == 1)
					buffer[y * cubeSize + x] = 1;
			}
	}

public:
	FIntPoint brushSize;
	int bufferSize;
	int* brushBuffer;
	int cubeCols;

	MaskBrush() : cubeCols(0), bufferSize(0), brushBuffer(nullptr) {}
	~MaskBrush()
	{
		bufferSize = 0;
		if (brushBuffer)
			delete[] brushBuffer;
		brushBuffer = nullptr;
	}

	/*
	void CreateCircleBrush(int circleRadius)
	{
		int cubeSide = 2 * circleRadius + 1;
		brushSize = FIntPoint(cubeSide, cubeSide);
		resetBuffer(cubeSide * cubeSide);

		for (int y = -circleRadius; y <= circleRadius; y++)
			for (int x = -circleRadius; x <= circleRadius; x++)
				if (x * x + y * y <= circleRadius * circleRadius)
					brushBuffer[(circleRadius + y) * cubeSide + (circleRadius + x)] = 1;
	}

	void CreateRectangleBrush(int width, int height)
	{
		brushSize = FIntPoint(width, height);
		resetBuffer(width * height);
		for (int index = 0; index< width * height; index++)
			brushBuffer[index] = 1;
	}
	*/

	void rotate(int cubeSize, float angle, FIntPoint pivotPoint)
	{
		int* rotatedBuffer = new int[bufferSize];
		for (int index = 0; index< bufferSize; index++)
			rotatedBuffer[index] = 0;

		float s = sin(angle * PI / 180);
		float c = cos(angle * PI / 180);

		for (int y = 0; y < cubeSize; y++)
			for (int x = 0; x < cubeSize; x++)
			{
				// translate point back to origin:
				int oX = x - pivotPoint.X;
				int oY = y - pivotPoint.Y;


				// rotate point
				float xnew = oX * c - oY * s;
				float ynew = oY * c + oX * s;

				// translate point back:
				oX = xnew + pivotPoint.X;
				oY = ynew + pivotPoint.Y;

				if (oX>=0 && oX <cubeSize && oY>=0 && oY <cubeSize)
					rotatedBuffer[oY * cubeSize + oX] = brushBuffer[y * cubeSize + x];
			}

		fillHoles(rotatedBuffer, cubeSize);

		delete[] brushBuffer;
		brushBuffer = rotatedBuffer;
	}
};

class EllipseBrush : public MaskBrush
{

public:
	EllipseBrush() {}
	~EllipseBrush() {}

	void Initialize(int circleRadius, float Axx = 1, float Ayy = 1)
	{
		cubeCols = 2 * circleRadius + 1;
		brushSize = FIntPoint(cubeCols, cubeCols);
		resetBuffer(brushSize.X * brushSize.Y);

		for (int y = -circleRadius; y <= circleRadius; y++)
			for (int x = -circleRadius; x <= circleRadius; x++)
				if (Axx*x*x + Ayy*y*y <= circleRadius*circleRadius)
					brushBuffer[(circleRadius + y) * cubeCols + (circleRadius + x)] = 1;
	}
};

class RectangleBrush : public MaskBrush
{
public:
	RectangleBrush() {}
	~RectangleBrush() {}
	void Initialize(int width, int height)
	{
		cubeCols = width;
		brushSize = FIntPoint(width, height);
		resetBuffer(width * height);
		for (int index = 0; index < width * height; index++)
			brushBuffer[index] = 1;
	}
};

struct ChanneledImpactRatio {
public:
	int channel_1_impactRatio;
	int channel_2_impactRatio;
	int channel_3_impactRatio;
	int channel_4_impactRatio;
	~ChanneledImpactRatio() {}
	ChanneledImpactRatio(int channel1, int channel2, int channel3, int channel4)
	{
		channel_1_impactRatio = channel1;
		channel_2_impactRatio = channel2;
		channel_3_impactRatio = channel3;
		channel_4_impactRatio = channel4;
	}

};

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
	/*
	uint32 maxMaskValue;
	uint32 minMaskValue;
	uint32 secondChannelMinValue;
	uint32 thirdChannelMinValue;
	uint32 forthChannelMinValue;
	int64  fifthChannelMinValue;
  */

	void freeCanvas();
	void setUpCanvas(FrameSize CanvasSize);
	void set_mask_impact_index(int32 X, int32 Y);

	inline bool is_position_in_range(int32 X, int32 Y);
	int Add_with_cap_255(int currentValue, int& increment, int multiplier);
	uint32 Add_to_quad(uint32* currentValue, int incrementStep, ChanneledImpactRatio* channeledImpactRatio);
	inline uint32 Sub_from_quad(uint32* currentValue, int decreamentStep);
	inline int calculate_impact_ratio_in_position(FIntPoint brushSize, float impactRatio, int X, int Y, int currentIndexX, int currentIndexY);

};




//make access to brush
//create and update brush
//update the initialize function in BP
//make the channeled impactratio and brush class BP accessible
//make use of add_to_quad