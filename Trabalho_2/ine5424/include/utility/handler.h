// EPOS Handler Utility Declarations

#ifndef __handler_h
#define __handler_h

#include <system/config.h>

__BEGIN_UTIL
class Handler
{
public:
    typedef void (Function) (); //Uma função 

    Handler () { }

    virtual ~Handler () { }

    virtual void operator() () = 0;
};

class Function_Handler : public Handler
{
public:

    Function_Handler (Function * handler) : _handler (handler) { }

    ~Function_Handler (){};

    void operator() () {
        _handler ();
    }


private:
    Function * _handler;
};



__END_UTIL

#endif
