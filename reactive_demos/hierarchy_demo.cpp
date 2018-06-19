#include <core/Subject.hpp>
#include <iostream>


class Element
{
private:
  Subject<std::size_t> subject;

public:
  using Subject = decltype(subject);
  using Observer = typename Subject::Observer;

  explicit Element(std::size_t const id, Observer observer)
    : id(id)
  {
    subject.attach(std::move(observer));
  }

  void notify() const { subject.notify(id); }

private:
  std::size_t const id;
};


class Container
{
public:
  explicit Container()
    : observer{[this] (std::size_t const id) {
        hit_counter++;
        std::cout << "Notification from element " << id << " (hit counter: " << hit_counter << ")" << std::endl;
      }}
  {}

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
  Element::Observer const observer;
  std::deque<Element> children;
};


int main()
try
{
  Container container;

  for (std::size_t i = 0; i < 5; ++i)
    container.createElement(i);

  container.triggerElements();

  return EXIT_SUCCESS;
}
catch (const std::exception & error)
{
  std::cerr << "Unhandled error: " << error.what() << std::endl;
  return EXIT_FAILURE;
}
catch (...)
{
  std::cerr << "Unknown unhandled error" << std::endl;
  return EXIT_FAILURE;
}
