// Fill out your copyright notice in the Description page of Project Settings.

#include "InvisibleBorder_test.h"
#include "DynamicPainter.h"
#include <cmath>

using namespace std;

DynamicPainter::DynamicPainter(FIntPoint CanvasSize):bufferedDynamicMask(nullptr), dynamicMask(nullptr), bufferSize(0)
{
  setUpCanvas(CanvasSize);
}

DynamicPainter::~DynamicPainter()
{
  freeCanvas();
}

bool DynamicPainter::isInitializedSuccessfully()
{
  return bufferedDynamicMask && dynamicMask && brush;
}

void DynamicPainter::setUpCanvas(FIntPoint CanvasSize)
{
  canvasSize = CanvasSize;
  bufferSize = canvasSize.X * canvasSize.Y;
  bufferedDynamicMask = new uint32[bufferSize];
  resetBufferedDynamicMask();
}

void DynamicPainter::resetBufferedDynamicMask()
{
	if (!bufferedDynamicMask)
		return;
	for (int32 index = 0; index < bufferSize; index++)
		bufferedDynamicMask[index] = 0;
}

void DynamicPainter::freeCanvas()
{
	if (bufferedDynamicMask)
		delete[] bufferedDynamicMask;
  bufferSize = 0;
	bufferedDynamicMask = nullptr;
	dynamicMask = nullptr;
	canvasSize = FIntPoint();
}

void DynamicPainter::Initialize(UTexture2D*& Texture, MaskBrush* Brush, int32 AgingStep,
	float ImpactRatio, ChanneledImpactRatio* Input_channeledImpactRatio, bool withCompression)
{
	channeledImpactRatio = Input_channeledImpactRatio;
	registerTexture(Texture, withCompression);
	setBrush(Brush);
	setAgingStep(AgingStep);
	setImpactRatio(ImpactRatio);
}

void DynamicPainter::setBrush(MaskBrush* Brush)
{
	brush = Brush;
}

void DynamicPainter::registerTexture(UTexture2D*& Texture, bool withCompression)
{
  if (Texture)
    delete Texture;
  Texture = UTexture2D::CreateTransient(canvasSize.X, canvasSize.Y);
  if(!withCompression)
  {
    Texture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
    Texture->Filter = TextureFilter::TF_Nearest;
    Texture->SRGB = false;
  }
  Texture->AddToRoot();
  
  dynamicMask = Texture;
}

void DynamicPainter::setAgingStep(int32 AgingStep)
{
  agingStep = abs(AgingStep);
}

void DynamicPainter::setImpactRatio(float ImpactRatio)
{
  impactRatio = ImpactRatio;
}

void DynamicPainter::addImpactInPosition(int X, int Y)
{
  for (int j = -brush->brushSize.Y / 2; j <= brush->brushSize.Y / 2; j++)
	  for (int i = -brush->brushSize.X / 2; i <= brush->brushSize.X / 2; i++)
	  {
		  int index_in_mask_buffer = (j + Y)*canvasSize.X + (i + X);
		  if (index_in_mask_buffer < bufferSize && index_in_mask_buffer >= 0 && brush->brushBuffer[(int)((j + brush->brushSize.Y / 2)*brush->brushSize.X + (i + brush->brushSize.X / 2))])
			  bufferedDynamicMask[index_in_mask_buffer] = fmin((int64)bufferedDynamicMask[index_in_mask_buffer] + 20, pow(2, 32));//Add_to_quad(&bufferedDynamicMask[index_in_mask_buffer], 20/*calculate_impact_ratio_in_position(brush->brushSize, impactRatio, X, Y, i + brush->brushSize.X / 2, j + brush->brushSize.Y / 2)*/, channeledImpactRatio);
	  }
}

void DynamicPainter::age()
{
  for (int index = 0; index < bufferSize; index++)
	bufferedDynamicMask[index] = Sub_from_quad(&bufferedDynamicMask[index], agingStep);
}

void DynamicPainter::ageStepByOne()
{
	uint64 temp;
	for (int index = 0; index < bufferSize; index++)
	{
		temp = bufferedDynamicMask[index];
		bufferedDynamicMask[index] = fmax(0, temp - 1);
	}
}


void DynamicPainter::updateMaskBuffer()
{
	if(!isInitializedSuccessfully())
	  return;

	UTexture2D* Texture = dynamicMask;
	const size_t Size = bufferSize * sizeof(RGBQUAD);
	uint8* Src = (uint8*)bufferedDynamicMask;
	uint8* Dest = (uint8*)Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(Dest, Src, Size);
	Texture->PlatformData->Mips[0].BulkData.Unlock();
	Texture->UpdateResource();
}

inline int DynamicPainter::calculate_impact_ratio_in_position(FIntPoint brushSize, float impactRatio, int X, int Y, int currentIndexX, int currentIndexY)
{
	return (abs((1 + trunc(brushSize.X / 2) - abs(X - currentIndexX)) / trunc(brushSize.X / 2) +
		(1 + trunc(brushSize.Y / 2) - abs(Y - currentIndexY)) / trunc(brushSize.Y / 2))) / 2 * impactRatio;
}

inline int DynamicPainter::Add_with_cap_255(int currentValue, int& increment, int multiplier)
{
	int realIncrement = increment * multiplier / 10;
	if (currentValue + realIncrement > 255)
	{
		int temp = increment - ceil((255 - currentValue) / (float)multiplier);
		increment = temp < 0 ? 0 : temp;
		return 255 - currentValue;
	}
	else
	{
		increment = 0;
		return realIncrement;
	}
}

uint32 DynamicPainter::Add_to_quad(uint32* currentValue, int incrementStep, ChanneledImpactRatio* channeledImpactRatio)
{
	RGBQUAD pixelQuad = *reinterpret_cast<RGBQUAD*>(currentValue);

    pixelQuad.rgbBlue =  Add_with_cap_255(pixelQuad.rgbBlue, incrementStep, channeledImpactRatio->channel_1_impactRatio);
	pixelQuad.rgbGreen = Add_with_cap_255(pixelQuad.rgbGreen, incrementStep, channeledImpactRatio->channel_2_impactRatio);
	pixelQuad.rgbRed = Add_with_cap_255(pixelQuad.rgbRed, incrementStep, channeledImpactRatio->channel_3_impactRatio);
	pixelQuad.rgbReserved = Add_with_cap_255(pixelQuad.rgbReserved, incrementStep, channeledImpactRatio->channel_4_impactRatio);

	return *currentValue + *reinterpret_cast<uint32*>(&pixelQuad);
}

inline uint32 DynamicPainter::Sub_from_quad(uint32* currentValue, int decreamentStep)
{
	RGBQUAD pixelQuad = *reinterpret_cast<RGBQUAD*>(currentValue);

	pixelQuad.rgbReserved = pixelQuad.rgbReserved - decreamentStep >= 0 ? decreamentStep : pixelQuad.rgbReserved;
	pixelQuad.rgbRed = pixelQuad.rgbRed - (decreamentStep - pixelQuad.rgbReserved) >= 0 ? decreamentStep - pixelQuad.rgbReserved : pixelQuad.rgbRed;
	pixelQuad.rgbGreen = pixelQuad.rgbGreen - (decreamentStep - pixelQuad.rgbReserved - pixelQuad.rgbRed) >= 0 ? decreamentStep - pixelQuad.rgbReserved - pixelQuad.rgbRed : pixelQuad.rgbGreen;
	pixelQuad.rgbBlue = pixelQuad.rgbBlue - (decreamentStep - pixelQuad.rgbReserved - pixelQuad.rgbRed - pixelQuad.rgbGreen) >= 0 ? decreamentStep - pixelQuad.rgbReserved - pixelQuad.rgbRed - pixelQuad.rgbGreen : pixelQuad.rgbBlue;

	return *currentValue - *reinterpret_cast<uint32*>(&pixelQuad);
}
