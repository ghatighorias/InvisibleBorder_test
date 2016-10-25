#ifndef DATATYPES_H
#define DATATYPES_H

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

#endif