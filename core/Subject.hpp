#pragma once

#include <algorithm>
#include <deque>
#include <memory>

#include <iostream>


// TODO: Use detection idiom to derive container type which suites best depending
// on Observer type details described below.
// * If it supports hash(), then use std::unordered_set.
// * else if it supports operator<, then use std::set.
// * else use std::deque.

namespace detail
{
  template<typename... Args>
  using Reaction = std::function<void(Args...)>;


  // NOTE: Observer is expected to support operator==. If not, you shall provide
  // compareAgent() specialization which returns object of some type
  // which does.
  template<typename Observer>
  inline
  const Observer & compareAgent(const Observer & observer)
  {
    return observer;
  }

  // NOTE: Such implementation of compareAgent() leads to a subtle effect
  // of detaching not only the observer in question but all the observers
  // of the same type. In case of extensive lambdas usage this does not
  // pose any problems since each lambda has its own type. Thus effectively
  // erasing any difference between types and objects. The strange behaviour
  // appears while using object created as a result of calling std::bind,
  // for example. Even in this case, you may consider using lambda instead.
  template<typename... Args>
  inline
  const std::type_info & compareAgent(const Reaction<Args...> & observer)
  {
    return observer.target_type();
  }

  template<typename Observer>
  inline
  Observer * compareAgent(const std::weak_ptr<Observer> & observer)
  {
    return observer.lock().get();
  }


  // NOTE: Observer is expected to support operator(Args...). If not, you shall provide
  // apply() specialization for your particular observer type which handles Args...
  // in an appropriate manner.
  template<typename Observer, typename... Args>
  inline
  void apply(const Observer & observer, Args &&... args)
  {
    observer(std::forward<Args>(args)...);
  }

  // NOTE: For some reason deduction fails if args is declared as Args && like in all
  // other similar places.
  template<typename Observer, typename... Args>
  inline
  void apply(const std::weak_ptr<Observer> & observer, Args... args)
  {
    if (const auto o = observer.lock()) {
      (*o)(std::forward<Args>(args)...);
    }
  }


  template<typename Observer, typename... Args>
  class Subject
  {
  public:
    using ObserverType = Observer;

    void attach(const Observer & observer)
    {
      m_observers.emplace_back(observer);
    }

    void attach(Observer && observer)
    {
      m_observers.emplace_back(std::move(observer));
    }

    void detach(const Observer & observer)
    {
      const auto & detached_agent = compareAgent(observer);
      auto erase_iter = std::remove_if(
        std::begin(m_observers), std::end(m_observers),
        [&detached_agent] (const Observer & current) {
          return compareAgent(current) == detached_agent;
        }
      );
      m_observers.erase(std::move(erase_iter), std::end(m_observers));
    }

    void notify(Args &&... args) const
    {
      for (const auto & observer : m_observers) {
        apply(observer, args...);
      }
    }

  protected:
    std::deque<Observer> m_observers;
  };
}


template<typename Subject>
class AttachGuard
{
public:
  AttachGuard(Subject & subject, typename Subject::ObserverType observer)
    : subject(subject)
    , observer(std::move(observer))
  {
    this->subject.attach(this->observer);
  }

  ~AttachGuard()
  {
    subject.detach(observer);
  }

private:
  Subject & subject;
  const typename Subject::ObserverType observer;
};

template<typename Subject, typename Observer>
AttachGuard<Subject> makeAttachGuard(Subject & subject, const Observer & observer)
{
  return AttachGuard<Subject>(subject, observer);
}


namespace functional
{
  template<typename... Args>
  class Subject : public detail::Subject<detail::Reaction<Args...>, Args...> {};
}

namespace shared
{
  template<typename... Args>
  class Subject : public detail::Subject<std::weak_ptr<detail::Reaction<Args...>>, Args...>
  {
    using Base = detail::Subject<std::weak_ptr<detail::Reaction<Args...>>, Args...>;

  public:
    // NOTE: This method may technically be static, but it is declared const instead
    // in order to force its usage via existing Subject instances instead of
    // class name qualified invokations.
    // Actually, this method is provided for convenience purpose only. One may well
    // create observers by calling std::make_shared directly, yet long typing of
    // fully defined type will be required.
    auto createObserver(detail::Reaction<Args...> f) const -> std::shared_ptr<decltype(f)>
    {
      return std::make_shared<decltype(f)>(std::move(f));
    }

    void notify(Args &&... args) const
    {
      Base::notify(std::move(args)...);
      const_cast<Subject*>(this)->removeExpiredObservers();
    }

  private:
    void removeExpiredObservers()
    {
      auto & observers = this->m_observers;
      auto erase_iter = std::remove_if(
        std::begin(observers), std::end(observers),
        [] (const typename Base::ObserverType & observer_weak) {
          const auto observer = observer_weak.lock();
          return not observer;
        }
      );
      observers.erase(std::move(erase_iter), std::end(observers));
    }
  };
}
