#include "UnrealSim.h"
#include "InputNode.h"

InputNode::InputNode(const std::string& name, const std::string& topic) :
  name(name),
  topic(topic),
  running(false)
{
  thread = FRunnableThread::Create(this, TEXT("InputNode"));
}

InputNode::~InputNode()
{
  // delete thread;
  // thread = nullptr;
}

bool InputNode::Init()
{
  running = true;
  return true;
}

uint32 InputNode::Run()
{
  InputMessage message;
 
  // node.init(name);
  node.init("Node3");
   
  while (running)
  {
    // node.subscribe(topic);
    node.subscribe("node_demo/CarInput");
    
    // bool success = node.receive(topic, message);
    bool success = node.receive("node_demo/CarInput", message);
    
    if (success) {
      UE_LOG(LogTemp, Log, TEXT("Input Received"));
      AlertMessage(message);
    }
    FPlatformProcess::Sleep(0.001f);
  }
  
  return 0;
}

void InputNode::Stop()
{
  running = false;
  thread->WaitForCompletion();
}

void InputNode::AddListener(InputListener* listener)
{
  listeners.push_back(listener);
}

void InputNode::RemoveListener(InputListener* listener)
{
  auto iterator = std::remove(listeners.begin(), listeners.end(), listener);
  listeners.erase(iterator, listeners.end());
}

void InputNode::AlertMessage(const InputMessage& message)
{
  for (auto listener : listeners)
  {
    listener->OnMessage(message);
  }
}
