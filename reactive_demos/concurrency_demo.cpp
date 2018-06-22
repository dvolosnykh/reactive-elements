#include <core/Subject.hpp>
#include <boost/asio/io_service.hpp>
#include <iostream>


namespace io = boost::asio;


// TODO: Provide self-describing example instead of some dummy woker below.
class Worker
{
public:
  explicit Worker(io::io_service & context, std::size_t const limit = 20)
    : context(context)
    , limit(limit)
  {
    subject.attach([this] (std::size_t const counter) {
      this->context.post([this, counter] {
        std::cout << "Counter: " << counter << std::endl;
        advanceCounter(counter);
      });
    });

    advanceCounter(0);
  }

private:
  void advanceCounter(std::size_t const counter)
  {
    if (counter >= limit)
      return;

    context.post([this, counter] {
      subject.notify(counter + 1);
    });
  }

private:
  io::io_service & context;
  std::size_t const limit;
  Subject<std::size_t> subject;
};


int main()
try
{
  io::io_service context;
  Worker worker(context);
  context.run();

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
