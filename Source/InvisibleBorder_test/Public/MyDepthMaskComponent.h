// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "DynamicPainter.h"
#include "MyDepthMaskComponent.generated.h"

UENUM(BlueprintType)
enum class EFunctionStateEnum : uint8
{
	FS_SUCCESSFUL      UMETA(DisplayName = "Successful"),
	FS_FAILURE   	       UMETA(DisplayName = "Failure"),
	FS_DEPENDENCY_FAILURE UMETA(DisplayName = "Dependency_failure")
};

USTRUCT(BlueprintType)
struct FFourChannelImpactRatio
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dynamic Mask")
	int32 channel_1_impactRatio;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dynamic Mask")
	int32 channel_2_impactRatio;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dynamic Mask")
	int32 channel_3_impactRatio;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dynamic Mask")
	int32 channel_4_impactRatio;
	FFourChannelImpactRatio(int32 channel1, int32 channel2, int32 channel3, int32 channel4) :
		channel_1_impactRatio(channel1), channel_2_impactRatio(channel2), channel_3_impactRatio(channel3), channel_4_impactRatio(channel4) {}
	FFourChannelImpactRatio() :
		channel_1_impactRatio(0), channel_2_impactRatio(0), channel_3_impactRatio(0), channel_4_impactRatio(0) {}
};

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
    EFunctionStateEnum Initialize(FVector2D CanvasSize, UTexture2D*& Texture, int32 AgingStep, FFourChannelImpactRatio fourChannelImpactRatio, float ImpactRatio = 3, bool withCompression = false);
	UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
		void RegisterTexture(UTexture2D*& Texture);
	UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
		void Age();
	UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
		void AgeStepByOne();
	UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
		void UpdateMaskTexture();
	UFUNCTION(BlueprintCallable, Category = "Dynamic Mask")
		void ImpactInPosition(FVector2D ImpactPosition);
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

private:
	DynamicPainter* dynamicPainter;
	MaskBrush* brush;
};