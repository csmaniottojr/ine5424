// EPOS Alarm Abstraction Implementation

#include <semaphore.h>
#include <alarm.h>
#include <display.h>

__BEGIN_SYS

// Class attributes
Alarm_Timer * Alarm::_timer;
volatile Alarm::Tick Alarm::_elapsed;
Alarm::Queue Alarm::_request;


// Methods

Alarm::Alarm(const Microsecond & time, Handler * handler, int times)
: _ticks(ticks(time)), _handler(handler), _times(times), _link(this, _ticks) {
    lock();

    db<Alarm>(TRC) << "Alarm(t=" << time << ",tk=" << _ticks << ",h=" << reinterpret_cast<void *> (handler)
            << ",x=" << times << ") => " << this << endl;

    if (_ticks) {
        _request.insert(&_link);
        unlock();
    } else {
        unlock();
        (*handler)();
    }
}

Alarm::~Alarm() {
    lock();

    db<Alarm>(TRC) << "~Alarm(this=" << this << ")" << endl;

    _request.remove(this);

    unlock();
}


// Class methods

void Alarm::delay(const Microsecond & time) {
    db<Alarm>(TRC) << "Alarm::delay(time=" << time << ")" << endl;
    db<Alarm>(WRN) << _elapsed << endl;


    Semaphore semaphore(0);
    Semaphore_Handler handler(&semaphore);
    Alarm alarm_delay(time, &handler, 1); //Se time < tick, v();
    semaphore.p();
    db<Alarm>(WRN) << _elapsed << endl;

}

void Alarm::handler(const IC::Interrupt_Id & i) {
    lock();

    _elapsed++;

    if (Traits<Alarm>::visible) {
        Display display;
        int lin, col;
        display.position(&lin, &col);
        display.position(0, 79);
        display.putc(_elapsed);
        display.position(lin, col);
    }

    Alarm * alarm = 0;
    if (!_request.empty()) {
        if (_request.head()->promote() <= 0) {
            Queue::Element * e = _request.remove();
            alarm = e->object();
            if (alarm->_times != -1)
                alarm->_times--;
            if (alarm->_times) {
                e->rank(alarm->_ticks);
                _request.insert(e);
            }
        }
    }

    if (alarm != 0) {
        db<Alarm>(TRC) << "Alarm::handler(h=" << reinterpret_cast<void *> (alarm->_handler) << ")" << endl;
        (*alarm->_handler)();
    }

    unlock();
}

__END_SYS
