#pragma once

#include "GameFramework/Actor.h"
#include "BlueprintDataTypes.generated.h"

UCLASS()
class INVISIBLEBORDER_TEST_API ABlueprintDataTypes : public AActor
{
  GENERATED_BODY()
};

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