# T2: Idle Thread + Timing

### 1. Task 1: Idle Thread

Some operating systems use an `idle thread` to handle cases in which the `ready queue` would otherwise become empty. Instead of handling those cases individually, the `idle thread` simply gets scheduled. The `idle thread`, with some infamous exceptions, then puts the machine in a low-power state until an event (e.g. interrupt) enables another thread to run.

The didactic version of OpenEPOS (https://epos.lisha.ufsc.br/svn/teaching/ine5424/) you are currently working with implements the `idle` thread as a function that is called whenever there is no other thread on the `ready` queue to run, like in the excerpt of the `suspend()` method reproduced bellow:

```c++
if(!_ready.empty())
       switch_threads(old, new);
else
       idle();
```

The `idle()` function is shown bellow:

```c++
void Thread::idle()
{
       CPU::int_enable();
       CPU::halt();
}
```

#### 1.1. To do

Modify the implementation of `idle` so it becomes a thread that is only scheduled when there are no other threads ready to run. This will eliminate bugs such as having the `timer` handler undesirably waking up suspended threads.

### 2. Task 2: Timing

Some operating systems use a `periodic timer` bound to a `tick counter` to manage time smoothly throughout the system. The didactic version of OpenEPOS (https://epos.lisha.ufsc.br/svn/teaching/ine5424/) you are currently working with, follows this strategy:

```c++
void Alarm::timer_handler(void)
{
       _elapsed++;
       ...
}
```

To implement the timed event concept, EPOS-- uses a event queue that is handled by the timer's interrupt handling routine as follows:

```c++
void Alarm::timer_handler(void)
{
       static Tick next;
       static Handler handler;

       _elapsed++;

       if(next)
              next--;

       if(!next) {
              if(handler)
                     handler();

              if(_requests.empty())
                     handler = 0;
              else {
                     Queue::Element * e = _requests.remove();
                     Alarm * alarm = e->object();
                     next = alarm->_ticks;
                     handler = alarm->_handler;
                     if(alarm->_times != -1)
                            alarm->_times--;
                     if(alarm->_times) {
                            e->rank(alarm->_ticks);
                            _requests.insert(e);
                     }
              }
       }
}
```

In order to avoid searching the `requests` queue on every time tick, the queue is kept ordered and its elements are ranked relatively to each other (i.e. the rank of each element is an offset to the previous).

The main issue in this implementation is that the `handler` indicated by the user to be invoked when the given time arrives is executed within the context of the interrupt handling routing, thus compromising the system timing.

#### 2.1. To do

You are requested to modify the implementation of `Alarm` to eliminate the busy-waiting in method `delay()` and also to remodel the interrupt handling routine so that user's handler functions no longer have a chance to disrupt the system timing.
