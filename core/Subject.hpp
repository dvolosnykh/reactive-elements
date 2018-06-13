#pragma once

#include <algorithm>
#include <deque>
#include <functional>
#include <memory>


namespace detail
{
  template<typename... Args>
  using Reaction = std::function<void(Args...)>;


  // NOTE: Observer is expected to support operator==. If not, you shall provide
  // compareAgent() specialization which returns object of some type
  // which does.
  template<typename Observer>
  inline
  Observer const & compareAgent(Observer const & observer)
  {
    return observer;
  }

  // NOTE: Such implementation of compareAgent() leads to a subtle effect
  // of detaching not only the observer in question but all the observers
  // of the same type. In case of extensive lambdas usage this does not
  // pose any problems since each lambda has its own type, thus effectively
  // erasing any difference between types and objects. The strange behaviour
  // appears while using object created as a result of calling std::bind,
  // for example. Even in this case, you may consider using lambda instead.
  template<typename... Args>
  inline
  std::type_info const & compareAgent(Reaction<Args...> const & observer)
  {
    return observer.target_type();
  }

  template<typename Observer>
  inline
  Observer * compareAgent(std::weak_ptr<Observer> const & observer)
  {
    return observer.lock().get();
  }


  // NOTE: Observer is expected to support operator(Args...). If not, you shall provide
  // apply() specialization for your particular observer type which handles Args...
  // in an appropriate manner.
  template<typename Observer, typename... Args>
  inline
  void apply(Observer const & observer, Args &&... args)
  {
    observer(std::forward<Args>(args)...);
  }

  template<typename Observer, typename... Args>
  inline
  void apply(std::weak_ptr<Observer> const & observer, Args &&... args)
  {
    if (auto const o = observer.lock()) {
      (*o)(std::forward<Args>(args)...);
    }
  }


  template<typename Observer>
  class Subject
  {
  public:
    using ObserverStoreType = Observer;
    using ObserverType = Observer;

    void attach(Observer const & observer)
    {
      m_observers.emplace_back(observer);
    }

    void attach(Observer && observer)
    {
      m_observers.emplace_back(std::move(observer));
    }

    void detach(Observer const & observer)
    {
      auto const & detached_agent = compareAgent(observer);

      // SUGGESTION: remove_if algorithm may be replaced with an alternative
      // one which moves only some of the elements in order to fill 'holes'
      // resulting from previously removed elements. Thus, order of
      // elements guarantee will be lost. The advantage, though, is
      // that the number of moved elements may decrease drastically.
      // This advantage may be escalated in case elements' type is not
      // movable. In this case less copies will be made.
      auto erase_iter = std::remove_if(
        std::begin(m_observers), std::end(m_observers),
        [&detached_agent] (Observer const & current) {
          return compareAgent(current) == detached_agent;
        }
      );

      m_observers.erase(std::move(erase_iter), std::end(m_observers));

      m_observers.shrink_to_fit();
    }

    // TODO: Check statically if Args are supported by Observer.
    template<typename... Args>
    void notify(Args &&... args) const
    {
      for (auto const & observer : m_observers) {
        apply(observer, std::forward<Args>(args)...);
      }
    }

  protected:
    // TODO: Derive container type which suites best depending on Observer
    // type details described below.
    // * If it supports hash(), then use std::unordered_set.
    // * else if it supports operator<, then use std::set.
    // * else use std::deque.
    // Note that for the case of functional API multi- counterparts of associative
    // containers are required since several observers may have the same type.
    std::deque<Observer> m_observers;
  };
}


template<typename Subject>
class AttachGuard
{
public:
  AttachGuard(Subject & subject, typename Subject::ObserverStoreType observer)
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
  typename Subject::ObserverStoreType const observer;
};

template<typename Subject, typename Observer>
AttachGuard<Subject> makeAttachGuard(Subject & subject, Observer const & observer)
{
  return AttachGuard<Subject>(subject, observer);
}


namespace functional
{
  template<typename... Args>
  class Subject : public detail::Subject<detail::Reaction<Args...>>
  {
  public:
    // NOTE: This method won't be needed if we commit to functional style of API only.
    static auto createObserver(detail::Reaction<Args...> f) -> decltype(f) { return f; }
  };
}

namespace shared
{
  template<typename... Args>
  class Subject : public detail::Subject<std::weak_ptr<detail::Reaction<Args...>>>
  {
    using Base = detail::Subject<std::weak_ptr<detail::Reaction<Args...>>>;

  public:
    using ObserverType = typename Base::ObserverType::element_type;

    // This method is provided for convenience purpose only. One may well
    // create observers by calling std::make_shared directly, yet long typing of
    // fully defined type will be required.
    static auto createObserver(detail::Reaction<Args...> f) -> std::shared_ptr<decltype(f)>
    {
      return std::make_shared<decltype(f)>(std::move(f));
    }

    void notify(Args... args) const
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
        [] (typename Subject::ObserverStoreType const & observer_weak) {
          auto const observer = observer_weak.lock();
          return not observer;
        }
      );
      observers.erase(std::move(erase_iter), std::end(observers));
    }
  };
}
