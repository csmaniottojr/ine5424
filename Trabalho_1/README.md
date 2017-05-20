# T1: Thread Synchronization + Thread Joining

### 1. Task 1: Thread Synchronization

The didactic version of OpenEPOS (https://epos.lisha.ufsc.br/svn/teaching/ine5424/) you are currently working with implements thread synchronization with simple trick: yielding other threads to run.

```c++
class Synchronizer_Common
{
protected:
       Synchronizer_Common() {}
protected:
       void sleep() { Thread::yield(); }
       void wakeup() {}
       void wakeup_all() {}
}
```

Therefore, synchronization abstractions such as Semaphore and Mutex continuously test for changes on a control variable, invoking yield after each test. This trick allows for empty implementations of wakeup and wakeup_all, but bears little predictability and performance. It is actually a faulty implementation for Semaphore: replace the Mutex synchronizing the table in the Philosophers' Dinner by a Semaphore, insert a Delay before enabling the philosopher threads to run and observe what happens.

#### 1.1. To do

Modify the implementation of EPOS--synchronization mechanisms so that waiting threads block instead of sampling control variables. Ensure that scheduling is not affected by your modifications implementation.

### 2. Task 2: Thread Joining

The didactic version of OpenEPOS (https://epos.lisha.ufsc.br/svn/teaching/ine5424/) you are currently working with implements thread joining with a simple trick: yielding other threads to run.

```c++	
int Thread::join()
{
       while( state != FINISHING)
              yield();
       return * (reinterpret_cast<int *>_stack);
};
```

That is, a joining thread checks the state of the thread being waited for until it turns to FINISHING. In order to give other threads a chance to run, and thus terminate, the calling thread invokes yield on each step. Line 5, which is not in the scope of this exercise, simply forwards the value left on the stack by the terminating thread to the waiting thread.

#### 2.1. To do

Change the implementation of Thread so that join() no longer wastes time sampling the waited thread status. It must be set to sleep until the waited-for thread terminates, and then waken up accordingly.
