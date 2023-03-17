#include "Player.h"
#include <iostream>

#include <string>
#include <gst/gst.h>
#include"bits/stdc++.h"

 

pthread_t bus_thread;
void *bus_loop(void *arg);

using namespace std;

namespace WPEFramework
{
namespace Plugin
{
SERVICE_REGISTRATION(Player, 1, 0);

    Player::Player(): PluginHost::JSONRPC()
    {
        Register<WPEFramework::Data::Parameter, void>(_T("Path"), &Player::Path, this);
        Register<WPEFramework::Data::Parameter, void>(_T("Play"), &Player::Play, this);
        Register<WPEFramework::Data::Parameter, void>(_T("Pause"), &Player::Pause, this);
        Register<WPEFramework::Data::Parameter, void>(_T("Stop"), &Player::Stop, this);
        Register<WPEFramework::Data::Parameter, void>(_T("Exit"), &Player::Exit, this);
    }

    Player::~Player()
    {
    }

    const string Player::Initialize(PluginHost::IShell* service )
    {

        gst_init(&argc, &argv);
     //   pthread_create(&bus_thread, NULL, bus_loop, NULL);
        return (string());
    }

    void Player::Deinitialize(PluginHost::IShell* service )
    {
        cout << "Deinitialize" << endl;
    }

    string Player::Information() const
    {
         return _T("The purpose of this plugin is provide ability to execute functional t");
         }
         
         
   
     void *bus_loop(void *arg)
     {
     bus = gst_element_get_bus(pipeline);

     while (1) {
       // msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
       msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

        if (msg == NULL) {
            continue;
        }

        switch (GST_MESSAGE_TYPE(msg)) {
            case GST_MESSAGE_EOS:
                g_print("End of stream message received\n");

                // Seek to the start of the video
               // seek_event = gst_event_new_seek(1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT, GST_SEEK_TYPE_SET, 0, GST_SEEK_TYPE_NONE, 0);
                seek_event = gst_event_new_seek(1.0, GST_FORMAT_TIME, static_cast<GstSeekFlags>(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), GST_SEEK_TYPE_SET, 0, GST_SEEK_TYPE_NONE, 0);

                gst_element_send_event(pipeline, seek_event);

                // Restart the pipeline
                cout<<"loop started"<<endl;
                gst_element_set_state(pipeline, GST_STATE_PLAYING);
                break;
            default:
                break;
        }

        gst_message_unref(msg);
    }

    gst_object_unref(bus);
}


    uint32_t Player::Path(const Data::Parameter& params)
    {

    // string s="playbin uri=file://";
     //s.append(params.output);
     //pipeline = gst_parse_launch(s.c_str(), nullptr);
     if (pipeline != NULL) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
    }
      string uri = "file://" + params.output.Value();
      pipeline = gst_parse_launch(("playbin uri=" + uri).c_str(), nullptr);
     if (pipeline != NULL)
     {
     cout<<"url stored in the pipeline"<<endl;
     }
    pthread_create(&bus_thread, NULL, bus_loop, NULL);
     
     cout<<"give the curl command to play"<<endl;
     return Core::ERROR_NONE;
    }
    
    
    uint32_t Player::Play(const Data::Parameter& params)
    {
     if (pipeline != NULL)
     {
     cout<<"playing"<<endl;
     
     gst_element_set_state(pipeline, GST_STATE_PLAYING);
     }
     return Core::ERROR_NONE;
    }   
    
    
    uint32_t Player::Pause(const Data::Parameter& params)
    {

     cout<<"paused"<<endl;
     gst_element_set_state(pipeline, GST_STATE_PAUSED);
     return Core::ERROR_NONE;
    } 
    
    

    uint32_t Player::Stop(const Data::Parameter& params)
    {
        if (pipeline != NULL) 
        {
         gst_element_set_state(pipeline, GST_STATE_NULL);
         gst_object_unref(pipeline);
         pipeline = NULL;
         }
         return Core::ERROR_NONE;
    }
    
    
      
    uint32_t Player::Exit(const Data::Parameter& params)
    {
      return Core::ERROR_NONE;
    }
    
    
    }// namespace plugin  
}  // namespace framework
