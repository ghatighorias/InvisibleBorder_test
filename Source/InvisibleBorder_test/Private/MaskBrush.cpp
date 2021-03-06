#include "InvisibleBorder_test.h"
#ifndef DATATYPES_H
#include "DataTypes.h"
#endif
#include "MaskBrush.h"

void MaskBrush::resetBuffer(int size)
{
  if (brushBuffer)
    delete[] brushBuffer;
  bufferSize = size;
  brushBuffer = new int[size];
  for (int index = 0; index< bufferSize; index++)
    brushBuffer[index] = 0;
}

void MaskBrush::fillHoles(int* buffer)
{
  if (!buffer)
    return;

  for (int y = 1; y< bufferFrameHeight - 1; y++)
    for (int x = 1; x< bufferFrameWidth - 1; x++)
    {
      if (buffer[y * bufferFrameWidth + x] == 0 &&
        buffer[(y + 1) * bufferFrameWidth + (x + 0)] == 1 &&
        buffer[(y - 1) * bufferFrameWidth + (x + 0)] == 1 &&
        buffer[(y + 0) * bufferFrameWidth + (x + 1)] == 1 &&
        buffer[(y + 0) * bufferFrameWidth + (x - 1)] == 1)
        buffer[y * bufferFrameWidth + x] = 1;
    }
}


MaskBrush::MaskBrush() : bufferFrameWidth(0), bufferFrameHeight(0), bufferSize(0), brushBuffer(nullptr) 
{
}

MaskBrush::~MaskBrush()
{
  bufferSize = 0;
  if (brushBuffer)
    delete[] brushBuffer;
  brushBuffer = nullptr;
}

void MaskBrush::rotate(float angle, FIntPoint pivotPoint)
{
  int* rotatedBuffer = new int[bufferSize];
  for (int index = 0; index< bufferSize; index++)
    rotatedBuffer[index] = 0;

  float s = sin(angle * PI / 180);
  float c = cos(angle * PI / 180);

  for (int y = 0; y < bufferFrameHeight; y++)
    for (int x = 0; x < bufferFrameWidth; x++)
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

      if (oX >= 0 && oX <bufferFrameWidth && oY >= 0 && oY <bufferFrameHeight)
        rotatedBuffer[oY * bufferFrameWidth + oX] = brushBuffer[y * bufferFrameWidth + x];
    }

  fillHoles(rotatedBuffer);

  delete[] brushBuffer;
  brushBuffer = rotatedBuffer;
}


EllipseBrush::EllipseBrush()
{
}

EllipseBrush::~EllipseBrush()
{
}

void EllipseBrush::Initialize(int circleRadius, float Axx, float Ayy)
{
  bufferFrameWidth = 2 * circleRadius + 1;
  bufferFrameHeight = 2 * circleRadius + 1;

  brushSize = FIntPoint(bufferFrameWidth, bufferFrameHeight);
  resetBuffer(brushSize.X * brushSize.Y);

  for (int y = -circleRadius; y <= circleRadius; y++)
    for (int x = -circleRadius; x <= circleRadius; x++)
      if (Axx*x*x + Ayy*y*y <= circleRadius*circleRadius)
        brushBuffer[(circleRadius + y) * brushSize.X + (circleRadius + x)] = 1;
}

RectangleBrush::RectangleBrush()
{
}

RectangleBrush::~RectangleBrush()
{
}

void RectangleBrush::Initialize(int width, int height)
{
  bufferFrameWidth = width;
  bufferFrameHeight = height;

  brushSize = FIntPoint(width, height);
  resetBuffer(width * height);
  for (int index = 0; index < width * height; index++)
    brushBuffer[index] = 1;
}