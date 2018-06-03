#include "tracer.hpp"
#include "subject.hpp"
#include <iostream>


struct Printer
{
  void operator()(std::size_t value) const
  {
    std::cout << "Printer: " << value << std::endl;
  }
};

#define Printer Tracer<Printer>


int main()
try
{
  functional::Subject<std::size_t> subject;

  const auto print = [] (std::size_t value) {
    std::cout << "Lambda: " << value << std::endl;
  };

  {
    const auto guard = makeAttachGuard(subject, print);
    subject.notify(0);
  }
  subject.notify(100); // Should be no output with number 100.

  subject.attach(print);
  const auto another_print = [] (std::size_t value) {
    std::cout << "Lambda: " << value << std::endl;
  };
  subject.attach(another_print);

  Printer printer;
  {
    const auto guard = makeAttachGuard(subject, printer);
    subject.notify(200);
  }
  subject.notify(300);

  const auto binded_printer = std::bind(&Printer::operator(), &printer, std::placeholders::_1);
  subject.attach(binded_printer);
  const auto another_binded_printer = std::bind(&Printer::operator(), &printer, std::placeholders::_1);
  subject.attach(another_binded_printer);

  subject.notify(1);
  subject.detach(print); // NOTE: another_print stays attached.
  subject.notify(2);
  subject.detach(binded_printer); // NOTE: This will detach another_binded_printer too.
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
