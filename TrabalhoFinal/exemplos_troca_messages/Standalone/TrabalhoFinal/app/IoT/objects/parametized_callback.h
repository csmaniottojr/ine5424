#pragma once

#include "callback.h"

class Parametized_Callback : public Callback
{
    typedef void (callback ) ( void* ) ;

private:

    void do_nothing ( void * ) { };
    callback _callback;
public:

    Parametized_Callback ( ) {
        this->_callback = do_nothing ;
    }
} ;