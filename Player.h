#pragma once
#include "Module.h"
#include <pthread.h>
#include <gst/gst.h>
#include <string>


GstElement *pipeline = NULL; 
GstBus *bus = NULL;
GstMessage *msg = NULL;
GstEvent *seek_event = NULL;
  
using namespace std;

namespace WPEFramework {

namespace Data
{
class Parameter : public Core::JSON::Container {
private:
Parameter(const Parameter&) = delete;
Parameter& operator=(const Parameter&) = delete;


public:
    Parameter()
        : Core::JSON::Container()
        , output()
    {
        Add(_T("output"), &output);
    }
    Parameter(const string& a)
        : Core::JSON::Container()
    {
        output = a;
        Add(_T("output"), &output);
    }
    ~Parameter() override = default;
    public:
        Core::JSON::String output;
};

class Response : public Core::JSON::Container {
private:
    Response(const Response&) = delete;
    Response& operator=(const Response&) = delete;

public:
    Response()
        : Core::JSON::Container()
        , output()
    {
        Add(_T("output"), &output);
    }
    Response(const string& a)
        : Core::JSON::Container()
    {
        output = a;
        Add(_T("output"), &output);
    }
    ~Response() override = default;
    public:
        Core::JSON::String output;
};
}
namespace Plugin {
class Player : public PluginHost::IPlugin, public PluginHost::JSONRPC {
private:
Player(const Player&) = delete;
Player& operator=(const Player&) = delete;
        public:
          Player(); 
           virtual ~Player();
            // IPlugin methods
            virtual const string Initialize(PluginHost::IShell* service) override;
            virtual void Deinitialize(PluginHost::IShell* service) override;
            
            virtual string Information() const override ;
            uint32_t Path(const Data::Parameter& params);
            uint32_t Play(const Data::Parameter& params);
            uint32_t Pause(const Data::Parameter& params);
            uint32_t Stop(const Data::Parameter& params);
            uint32_t Exit(const Data::Parameter& params);
            
            
    
            int argc = 0;
            char **argv = nullptr;
                 

            
            
            BEGIN_INTERFACE_MAP(Helloworld)
            INTERFACE_ENTRY(PluginHost::IPlugin)
            INTERFACE_ENTRY(PluginHost::IDispatcher)
            END_INTERFACE_MAP
    };
   } // namespace Plugin
} // namespace WPEFramework
