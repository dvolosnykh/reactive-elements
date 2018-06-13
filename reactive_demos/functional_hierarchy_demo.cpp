#include <core/Subject.hpp>
#include <iostream>
#include <vector>


// NOTE: Actually, such implementation clones prototype observer (the one
// defined in Container class) on behalf of every subject. This effectively
// provides many one-to-one relationships instead of desired one many-to-one
// relationship.
// Fortunately, common state may be stored inside Container instance and
// used by reference from lambdas passed as observers.

class Element
{
private:
  functional::Subject<std::size_t> subject;

public:
  using Observer = decltype(subject)::ObserverType;

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
  Element::Observer const observer{
    [this] (std::size_t const id) {
      hit_counter++;
      std::cout << "Notification from element " << id << " (hit counter: " << hit_counter << ")" << std::endl;
  }};
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
