#ifndef W_EVENT_LISTENER_SET_H
#define W_EVENT_LISTENER_SET_H

#include <functional>
#include <vector>


// Use this Macro to create a set of event listeners for a specific named event, as well as the method encapsulating the addition of listeners to that set.
// This macro should be used in the class where the event listeners should be added, for instance to process mouse or keyboard events.
// It must be included after the class declaration and before any access specifier (like public, private or protected)
#define USE_LISTENERS(event_name, ...)  public: \
                                          void addListener_##event_name(typename glwnd::EventListenerSet< __VA_ARGS__ >::ContextType a_ctxt, typename std::function<void(void* context, __VA_ARGS__)> a_listener) { \
                                            event_name##_listeners.addListener(a_ctxt, a_listener); \
                                          } \
                                        protected: \
                                          glwnd::EventListenerSet< __VA_ARGS__ >  event_name##_listeners

// Use this Macro to invoke all the assigned listeners to a named event, for instance while processing mouse or keyboard events
#define CALL_LISTENERS(event_name, ...)   event_name##_listeners(__VA_ARGS__)


namespace glwnd
{
 
  /**
    *  \brief  This template class encapsulates a set of listeners of the same type.
    *          It behaves as a Functor which invokes all the listeners in the set
    */
  template<class...Targs>
  class EventListenerSet
  {
  public:
    // Definition of the Listener Function type
    using ListenerFn = std::function<void(void* context, Targs...)>;

    // Definition of the Context type
    using ContextType = void*;

    // Default constructor
    EventListenerSet() {}

    // Deleted copy constructors and assignment operators
    EventListenerSet(const EventListenerSet&) = delete;
    EventListenerSet(EventListenerSet&&) = delete;
    EventListenerSet& operator=(const EventListenerSet&) = delete;
    EventListenerSet& operator=(EventListenerSet&&) = delete;


    // Destructor
    ~EventListenerSet() {}

    // Creates a listener with the specified context and function
    void  addListener(ContextType a_ctxt, ListenerFn a_function) { _listeners.push_back(EventListener(a_ctxt, a_function)); }

    // Invokes all the listeners with the specified arguments
    void  operator() (Targs... Nargs) { for (auto& listener : _listeners) listener(Nargs...); }


  protected:
    /**
      *  \brief  This class is a Functor which will process events by calling the provided function.
      *          The listener function (callback) processes an Event which is defined by the following arguments:
      *             - Context: a pointer to any type
      *             - Variable list of arguments especified as a variable list of template parameters
      *          IMPORTANT! : The listener function CANNOT BE a class member. It can be a class friend functions or a static class method.
      */
    class EventListener
    {
    public:
      // Constructor: the context to be used by the listener function and the listener function to be called 
      EventListener(ContextType a_ctxt, ListenerFn a_function) : _listener_fn{ a_function }, _listener_ctxt{ a_ctxt } {}

      // Destructor
      ~EventListener() {}

      // Function call operator: invokes the listener function with the specified variable list of arguments
      void operator()(Targs... listener_args) { _listener_fn(_listener_ctxt, listener_args...); }


    protected:
      // The listener function
      ListenerFn                _listener_fn;

      // The context to be used by the listener function
      ContextType               _listener_ctxt;
    };

    // Set of listeners, defined as a vector
    std::vector<EventListener>  _listeners;
  };

}

#endif //W_EVENT_LISTENER_SET_H