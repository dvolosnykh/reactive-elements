#pragma once

#include <algorithm>
#include <deque>
#include <functional>


// TODO: Remove this type support utility once C++20 is used.
template<typename T>
struct remove_cvref
{
  using type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
};

template<typename... Args>
using canonical_tuple = std::tuple<typename remove_cvref<Args>::type...>;


template<typename... Args>
class Subject
{
public:
  using Observer = std::function<void(Args...)>;

  void attach(Observer const & observer)
  {
    m_observers.emplace_back(observer);
  }

  void attach(Observer && observer)
  {
    m_observers.emplace_back(std::move(observer));
  }

  // NOTE: This metod relies on RTTI being enabled.
  void detach(Observer const & observer)
  {
    auto const & detached_type = observer.target_type();

    // SUGGESTION: remove_if algorithm may be replaced with an alternative
    // one which moves only some of the elements in order to fill 'holes'
    // resulting from previously removed elements. Thus, order of
    // elements guarantee will be lost. The advantage, though, is
    // that the number of moved elements may decrease drastically.
    // This advantage may be escalated in case elements' type is not
    // movable. In this case less copies will be made.
    auto erase_iter = std::remove_if(
      std::begin(m_observers), std::end(m_observers),
      [&detached_type] (Observer const & current) {
        return current.target_type() == detached_type;
      }
    );

    m_observers.erase(std::move(erase_iter), std::end(m_observers));

    m_observers.shrink_to_fit();
  }

  template<typename... NotifyArgs,
    typename = typename std::enable_if<
      std::is_convertible<canonical_tuple<Args...>, canonical_tuple<NotifyArgs...>>::value
    >::type>
  void notify(NotifyArgs &&... args) const
  {
    for (auto const & observer : m_observers) {
      observer(std::forward<NotifyArgs>(args)...);
    }
  }

protected:
  std::deque<Observer> m_observers;
};


template<typename Subject>
class AttachGuard
{
public:
  AttachGuard(Subject & subject, typename Subject::Observer observer)
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
  typename Subject::Observer const observer;
};

// NOTE: This helper method will not be needed once C++17 is used.
template<typename Subject, typename Observer>
AttachGuard<Subject> makeAttachGuard(Subject & subject, Observer const & observer)
{
  return AttachGuard<Subject>(subject, observer);
}
