#include <core/Tracer.hpp>
#include <core/Subject.hpp>


using namespace std::placeholders;


struct Printer {
  void operator()(std::size_t const value) const {
    std::cout << value << " - Printer" << std::endl;
  }
};

#define Printer Tracer<Printer>


int main()
try {
  Subject<std::size_t> subject;

  auto const print = [] (std::size_t const value) {
    std::cout << value << " - Lambda" << std::endl;
  };

  {
    auto const guard = make_attach_guard(subject, print);
    subject.notify(0);
  }
  subject.notify(100); // Should be no output with number 100.

  subject.attach(print);
  auto const another_print = [] (std::size_t const value) {
    std::cout << value << " - Another lambda" << std::endl;
  };
  subject.attach(another_print);

  Printer printer;
  {
    auto const guard = make_attach_guard(subject, printer);
    subject.notify(200);
  }
  subject.notify(300);

  auto const binded_printer = std::bind(&Printer::operator(), &printer, _1);
  subject.attach(binded_printer);
  auto const another_binded_printer = std::bind(&Printer::operator(), &printer, _1);
  subject.attach(another_binded_printer);

  subject.notify(1);
  subject.detach(print); // NOTE: another_print remains attached.
  subject.notify(2);
  subject.detach(binded_printer); // NOTE: This will detach another_binded_printer too.
  subject.notify(3);
  return EXIT_SUCCESS;
}
catch (std::exception const& error) {
  std::cerr << "Unhandled error: " << error.what() << std::endl;
  return EXIT_FAILURE;
}
catch (...) {
  std::cerr << "Unknown unhandled error" << std::endl;
  return EXIT_FAILURE;
}
