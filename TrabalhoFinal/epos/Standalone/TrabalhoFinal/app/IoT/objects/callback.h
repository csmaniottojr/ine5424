#pragma once

class Callback
{
    //A function that is called when a Parameter has changed
    typedef void ( *callback ) (  ); //

private:

    static void do_nothing ( ) { };
    callback _callback;
public:

    Callback ( )  {
        this->_callback = &do_nothing;
    }

    Callback ( callback call ) : _callback ( call ) { }

    void operator() ( ) {
        _callback ( );
    }

    void set_callback ( callback call ) {
        this->_callback = call;
    }
} ;