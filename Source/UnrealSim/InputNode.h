#pragma once

#include <string>
#include <vector>
#include "node/Node.h"
#include "Messages/InputMessage.pb.h"

class InputNode;

typedef std::function<void(const InputMessage&)> InputCallback;

class InputNode;
class InputListener;

class UNREALSIM_API InputNode : public FRunnable
{
  public:
    InputNode(const std::string& name, const std::string& topic);
    ~InputNode();
    
  public:
    virtual bool Init();
    virtual uint32 Run();
    virtual void Stop();
    virtual void AddListener(InputListener* listener);
    virtual void RemoveListener(InputListener* listener);
    
  protected:
    virtual void AlertMessage(const InputMessage& message);
    
  protected:
    FRunnableThread* thread;
    std::string name;
    std::string topic;
    node::node node;
    bool running;
    std::vector<InputListener*> listeners;
};

class UNREALSIM_API InputListener
{
  public:
    virtual void OnMessage(const InputMessage& message) = 0;
};
