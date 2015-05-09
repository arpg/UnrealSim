// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealSim.h"
#include "CameraNode.h"

// Sets default values
ACameraNode::ACameraNode() :
    ElapsedTime(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACameraNode::BeginPlay()
{
	Super::BeginPlay();
	
  NodeNameString = TCHAR_TO_ANSI(*NodeName);
  NodeTopicString = TCHAR_TO_ANSI(*NodeTopic);
  
  Node.init(NodeNameString);
  Node.advertise(NodeTopicString);

  int w = RenderTarget->SizeX;
  int h = RenderTarget->SizeY;
  ImageData = new unsigned char[w * h * 3];
  
  Message.set_width(w);
  Message.set_height(h);
}

// Called every frame
void ACameraNode::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
  ElapsedTime += DeltaTime;

  if (ElapsedTime > 0.05f)
  {
    PublishImage();
    ElapsedTime = 0.0f;
  }
}

void ACameraNode::PublishImage()
{
  FTextureRenderTarget2DResource* textureResource;
  textureResource = (FTextureRenderTarget2DResource*)RenderTarget->Resource;
  int index = 0;

  if (textureResource->ReadPixels(ColorBuffer))
  {
    for (int i = 0; i < ColorBuffer.Num(); ++i)
    {
      ImageData[index++] = ColorBuffer[i].R;
      ImageData[index++] = ColorBuffer[i].G;
      ImageData[index++] = ColorBuffer[i].B;
    }
  }
  
  void* data = (void*)ImageData;
  Message.set_data(data, index);
  Node.publish(NodeTopicString, Message);
}
