	#pragma once
#include "Audio.h"
#include "ConsoleAudio.h"

class Locator
{
public:
    static void initialize() { service_ = &nullService_; }

    static Audio* getAudio() { return service_; }

    static void provide(Audio* service)
    {
        if (service == nullptr)
        {
            // Revert to null service.
            service_ = &nullService_;
        }
        else
        {
            service_ = service;
        }
    }

    static void Quit()
    {
        if(service_ != &nullService_ && service_ != nullptr)
        {
	        delete service_;
	        service_ = nullptr;
        }
    }

private:
    static Audio* service_;
    static NullAudio nullService_;
};
