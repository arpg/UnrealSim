// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include "node/Node.h"
#include "Messages/ImageMessage.pb.h"
#include "GameFramework/Actor.h"
#include "CameraNode.generated.h"

UCLASS()
class UNREALSIM_API ACameraNode : public AActor
{
  GENERATED_BODY()
  
public:  
  // Sets default values for this actor's properties
  ACameraNode();

  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
  
  // Called every frame
  virtual void Tick( float DeltaSeconds ) override;
  
  virtual void PublishImage();

public:
  UPROPERTY(Category = Target, EditAnywhere)
  UTextureRenderTarget2D* RenderTarget;

  TArray<FColor> ColorBuffer;

  UPROPERTY(Category = Node, EditAnywhere)
  FString NodeName;

  UPROPERTY(Category = Node, EditAnywhere)
  FString NodeTopic;

protected:
  node::node Node;
  ImageMessage Message;
  unsigned char* ImageData;
  std::string NodeNameString;
  std::string NodeTopicString;
  float ElapsedTime;
  
};
