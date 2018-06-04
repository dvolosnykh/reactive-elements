#include <core/Subject.hpp>
#include <iostream>

struct Tracer
{
  void operator()(std::size_t value) const
  {
    std::cout << "Printer: " << value << std::endl;
  }
};

int main()
try
{
  shared::Subject<std::size_t> subject;

  const auto print = subject.createObserver([] (std::size_t value) {
    std::cout << "Lambda: " << value << std::endl;
  });
  subject.attach(print);
  const auto another_print = subject.createObserver([] (std::size_t value) {
    std::cout << "Lambda: " << value << std::endl;
  });
  subject.attach(another_print);

  Tracer printer;
  const auto binded_printer = subject.createObserver(
    std::bind(&Tracer::operator(), &printer, std::placeholders::_1)
  );
  subject.attach(binded_printer);
  const auto another_binded_printer = subject.createObserver(
    std::bind(&Tracer::operator(), &printer, std::placeholders::_1)
  );
  subject.attach(another_binded_printer);

  subject.notify(1);
  subject.detach(print); // NOTE: another_print stays attached.
  subject.notify(2);
  subject.detach(binded_printer); // NOTE: another_binded_printer stays attached.
  subject.notify(3);
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
