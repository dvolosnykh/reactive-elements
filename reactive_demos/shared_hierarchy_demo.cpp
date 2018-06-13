#include <core/Subject.hpp>
#include <iostream>
#include <vector>


class Element
{
private:
  shared::Subject<std::size_t> subject;

public:
  using Observer = decltype(subject)::ObserverType;

  explicit Element(std::size_t const id, std::weak_ptr<Observer> observer)
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
  std::shared_ptr<Element::Observer> const observer{
    std::make_shared<Element::Observer>(
      [] (std::size_t const id) {
        std::cout << "Notification from element " << id << std::endl;
  })};
  std::vector<Element> children;
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
