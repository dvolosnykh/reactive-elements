#include <core/Subject.hpp>
#include <iostream>
#include <deque>


template<template<typename... Args> class S>
class Element
{
private:
  S<std::size_t> subject;

public:
  using Subject = decltype(subject);
  using Observer = typename Subject::ObserverType;

  explicit Element(std::size_t const id, typename Subject::ObserverStoreType observer)
    : id(id)
  {
    subject.attach(std::move(observer));
  }

  void notify() const { subject.notify(id); }

private:
  std::size_t const id;
};


template<typename E, typename O = typename E::Observer>
class Container
{
public:
  void createElement(std::size_t const id)
  {
    children.emplace_back(id, observer);
  }

  void triggerElements()
  {
    for (auto const & child : children)
      child.notify();
  }

private:
  std::size_t hit_counter = 0;
  O const observer{
    E::Subject::createObserver([this] (std::size_t const id) {
      hit_counter++;
      std::cout << "Notification from element " << id << " (hit counter: " << hit_counter << ")" << std::endl;
  })};
  std::deque<E> children;
};


int main()
try
{
#if 1
  using El = Element<functional::Subject>;
  Container<El> container;
#else
  using El = Element<shared::Subject>;
  Container<El, std::shared_ptr<El::Observer>> container;
#endif

  for (std::size_t i = 0; i < 5; ++i)
    container.createElement(i);

  container.triggerElements();

  return EXIT_SUCCESS;
}
catch (const std::exception &error)
{
  std::cerr << "Unhandled error: " << error.what() << std::endl;
  return EXIT_FAILURE;
}
catch (...)
{
  std::cerr << "Unknown unhandled error" << std::endl;
  return EXIT_FAILURE;
}
