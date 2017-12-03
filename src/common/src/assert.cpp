#include "common/assert.h"

#include <iostream>

namespace {
using Params = Assert::Handler::Params;

void abortHandler(const Params &p) {
  if (!p.message.empty()) {
    std::cout << p.message << " (";
  }
  std::cout << R"(assertion failed: ")" << p.condition << R"(" is false in )"
            << p.file << " @ " << p.line;
  if (!p.message.empty()) {
    std::cout << ')';
  }
  std::cout << std::endl;
  abort();
}

void throwHandler(const Params &p) {
  std::ostringstream ss;
  if (!p.message.empty()) {
    ss << p.message << " (";
  }
  ss << R"(assertion failed: ")" << p.condition << R"(" is false in )" << p.file
     << " @ " << p.line;
  if (!p.message.empty()) {
    ss << ')';
  }
  throw Assert::Handler::Failure{ss.str()};
}
} // namespace

namespace Assert {
Handler::Handler() : currentHandler(abortHandler) {}

void Handler::setHandler(Handler::Type handler) {
  instance().currentHandler = handler;
}

void Handler::setHandler(availableHandlers handler) {
  switch (handler) {
  case availableHandlers::throwException:
    instance().currentHandler = throwHandler;
    break;
  case availableHandlers::abort:
  default:
    instance().currentHandler = abortHandler;
  }
}

void Handler::invoke(const std::string &text, const std::string &file,
                     size_t line, const std::string &message) {
  instance().currentHandler(Params{text, file, line, message});
  abort(); // Will not be reached, but we need to convince the compiler
           // that this function does not return
}

const char *Handler::Failure::what() const noexcept { return message.c_str(); }
} // namespace Assert
