#ifndef MASKBRUSH_H
#define MASKBRUSH_H

class MaskBrush
{
protected:
  void resetBuffer(int size);
  void fillHoles(int* buffer);

public:
  FIntPoint brushSize;
  int bufferSize;
  int* brushBuffer;
  int bufferFrameWidth;
  int bufferFrameHeight;

  MaskBrush();
  ~MaskBrush();
  void rotate(float angle, FIntPoint pivotPoint);
};

class EllipseBrush : public MaskBrush
{
public:
  EllipseBrush();
  ~EllipseBrush();
  void Initialize(int circleRadius, float Axx = 1, float Ayy = 1);
};

class RectangleBrush : public MaskBrush
{
public:
  RectangleBrush();
  ~RectangleBrush();
  void Initialize(int width, int height);
};

#endif