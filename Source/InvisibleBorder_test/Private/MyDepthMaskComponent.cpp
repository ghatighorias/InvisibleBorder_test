// Fill out your copyright notice in the Description page of Project Settings.

#include "InvisibleBorder_test.h"
#include "MyDepthMaskComponent.h"


// Sets default values for this component's properties
UMyDepthMaskComponent::UMyDepthMaskComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyDepthMaskComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UMyDepthMaskComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

bool UMyDepthMaskComponent::IsInitializedSuccessfully()
{
	if (!dynamicPainter)
	  return dynamicPainter->isInitializedSuccessfully();
  return false;
}

EFunctionStateEnum UMyDepthMaskComponent::Initialize(FIntPoint CanvasSize, UTexture2D*& Texture, int32 AgingStep, FFourChannelImpactRatio fourChannelImpactRatio, float ImpactRatio, bool withCompression)
{
	if (!brush)
		return EFunctionStateEnum::FS_DEPENDENCY_FAILURE;
	
	if (!dynamicPainter)
		dynamicPainter = new DynamicPainter(CanvasSize);

	ChanneledImpactRatio* TempChanneledImpactRatio = new ChanneledImpactRatio(fourChannelImpactRatio.channel_1_impactRatio,
																			                                      fourChannelImpactRatio.channel_2_impactRatio,
																			                                      fourChannelImpactRatio.channel_3_impactRatio,
																			                                      fourChannelImpactRatio.channel_4_impactRatio);
  try
  {
    dynamicPainter->Initialize(Texture, brush, AgingStep, ImpactRatio, TempChanneledImpactRatio, withCompression);
    return EFunctionStateEnum::FS_SUCCESSFUL;

  }
  catch (...)
  {
    return EFunctionStateEnum::FS_FAILURE;
  }
	
}
void UMyDepthMaskComponent::RegisterTexture(UTexture2D*& Texture)
{
	dynamicPainter->registerTexture(Texture);
}
void UMyDepthMaskComponent::Age()
{
	dynamicPainter->age();
}
void UMyDepthMaskComponent::AgeStepByOne()
{
	dynamicPainter->ageStepByOne();
}
void UMyDepthMaskComponent::UpdateMaskTexture()
{
	dynamicPainter->updateMaskBuffer();
}
void UMyDepthMaskComponent::ImpactInPosition(FIntPoint ImpactPosition)
{
	dynamicPainter->addImpactInPosition(ImpactPosition.X, ImpactPosition.Y);
}
void UMyDepthMaskComponent::SetImpactRatio(float impactRatio)
{
	dynamicPainter->setImpactRatio(impactRatio);
}
void UMyDepthMaskComponent::SetAgingStep(int32 AgingStep)
{
	dynamicPainter->setAgingStep(AgingStep);
}
void UMyDepthMaskComponent::resetDepthMap()
{
	dynamicPainter->resetBufferedDynamicMask();
}

// reassignment of the brush to the dynamic painter is excessive since in the first place it is assigned through pointers
void UMyDepthMaskComponent::CreateRectangleBrush(int32 Width, int32 Height)
{
	if (brush)
		delete brush;

	RectangleBrush* rBrush = new RectangleBrush;
	rBrush->Initialize(Width, Height);
	brush = rBrush;
}
void UMyDepthMaskComponent::CreateCircleBrush(int circleRadius, float Axx, float Ayy)
{
	if (brush)
		delete brush;

	EllipseBrush* eBrush = new EllipseBrush;
	eBrush->Initialize(circleRadius, Axx, Ayy);
	brush = eBrush;
}

EFunctionStateEnum UMyDepthMaskComponent::RotateBrush(float angle, FIntPoint pivotPoint)
{
  if (brush)
  {
    try
    {
      brush->rotate(angle, pivotPoint);
      return EFunctionStateEnum::FS_SUCCESSFUL;
    }
    catch (...)
    {
      return EFunctionStateEnum::FS_FAILURE;
    }
  }
  else
    return EFunctionStateEnum::FS_DEPENDENCY_FAILURE;
}
