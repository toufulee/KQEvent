#ifndef __ABSOBSERVER_H__
#define __ABSOBSERVER_H__

#include <functional>
#include <string>
#include <atomic>
#include <memory>

namespace KQEvent {
    class Subject;

    class Observer : public std::enable_shared_from_this<Observer> {
    public:
        typedef enum {
            DELETE, ALIVE
        } Command_t;
        using ObserverPtr = std::shared_ptr<Observer>;
        using Handle_t = std::function<Command_t(std::shared_ptr<Subject>)>;

        /*不推荐直接使用构造函数new对象，因为KQEvent内并不鼓励使用原生指针*/
        Observer(Handle_t const &handle, std::string const &name) :
                _handle(handle),
                _name(name) {

        }

        /*推荐使用该方法new对象*/
        template<typename ..._Args>
        static ObserverPtr newInstance(_Args &&...args) {
            auto aNew = new Observer(std::forward<_Args>(args)...);
            return Observer::ObserverPtr(aNew);
        }

        Observer(Observer const &) = delete;

        Observer &operator=(Observer const &) = delete;

        void setHandle(Handle_t const &handle) {
            _handle = handle;
        }

        Handle_t getHandle() { return _handle; }

        virtual Command_t update(std::shared_ptr<Subject> subject) {
            return _handle(subject);
        }
/*
    std::shared_ptr<AbsObserver> getPtr(void){
		return this->shared_from_this();
    }
*/
        //for log
        virtual void onAttach(void) {}

        virtual void onDetach(void) {}

    private:
    	Observer() = default;
        Handle_t _handle;
        std::string _name;//used by log
    };
}
#endif
