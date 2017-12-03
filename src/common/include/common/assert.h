#pragma once

#include <sstream>
#include <string>

namespace Assert {
/**
  Defines the behaviour of the assertion macros.

  When an assertion fails, different behaviours might be desired depending
  on the situation.
  When running unit tests, for example, it is desirable for an exception to
  be thrown.
  In devel builds, on the contrary, one usually wants the program to be
  terminated.
  This class defines two handlers ('availableHandlers::abort' and
  'availableHandlers::throwException')
  and provides the means for choosing between the two.
  In addition, it provides the means for instructing the assert macros to
  use an user defined handler.
  */
struct Handler {
  /**
    The parameters that a custom assertion handler must accept.

    This class provides all of the data available to an assert macro upon
    failure.
    This is what gets passed to an assertion handler, so that it can
    fulfill its task.
    */
  struct Params {
    Params(const std::string &condition, const std::string &file, size_t line,
           const std::string &message)
        : condition(condition), file(file), line(line), message(message) {}
    Params &operator=(const Params &rhs);
    Params(const Params &rhs);
    const std::string &condition;
    const std::string &file;
    size_t line;
    const std::string &message;
  };
  /**
    The signature to be used for writing a custom assertion handler.
    */
  using Type = void (*)(const Params &);
  /**
    Names for the predefined assertion handlers.

    availableHandlers::abort - prints a message to the standard output and
    aborts the execution of the program.
    The message contains the text for the condition, the file name and the
    line.
    availableHandlers::throwException - throw an exception of type
    Handler::Failure, whose 'what' method returns
    a message which contains the text for the condition, the file name and
    the line.
    In both cases, any additional message that has been specified in the
    call to the assertion macro is included.
    */
  // cppcheck-suppress syntaxError
  enum class availableHandlers { abort, throwException };

  /**
    The exception class that gets thrown if the 'throwException' handler
    is used.

    When this gets thrown by the predefined handler
    'availableHandlers::throwException', the 'what' method returns
    a message which contains the text for the condition, the file name and
    the line.
    */
  struct Failure : public std::exception {
    explicit Failure(std::string message) : message(std::move(message)) {}
    const char *what() const noexcept override;

  private:
    std::string message;
  };

  static void invoke(const std::string &text, const std::string &file,
                     size_t line, const std::string &message);
  /**
    Instruct the assertion macros to use a custom assertion handler.
    */
  static void setHandler(Type handler);
  /**
    Instruct the assertion macros to use one of the predefined assertion
    handlers.
    */
  static void setHandler(availableHandlers handler);

private:
  static Handler &instance() {
    static Handler obj;
    return obj;
  }

private:
  Handler();
  Handler(const Handler &);
  Handler &operator=(const Handler &);

  Type currentHandler;
};

template <typename List> struct FormatData {
  using type = List;
  List list;
};

struct None {};

template <typename Begin, typename Value>
FormatData<std::pair<Begin &&, Value &&>> operator<<(FormatData<Begin> &&begin,
                                                     Value &&value) noexcept {
  return {{std::forward<Begin>(begin.list), std::forward<Value>(value)}};
}

using PfnManipulator = std::ostream &(*)(std::ostream &);

template <typename Begin>
FormatData<std::pair<Begin &&, PfnManipulator>>
operator<<(FormatData<Begin> &&begin, PfnManipulator pfn) noexcept {
  return {{std::forward<Begin>(begin.list), pfn}};
}

template <typename Begin, size_t n>
FormatData<std::pair<Begin &&, const char *>>
operator<<(FormatData<Begin> &&begin, const char (&sz)[n]) noexcept {
  return {
      std::pair<Begin &&, const char *>{std::forward<Begin>(begin.list), sz}};
}

template <typename TFormatDataPair>
void FormatRecursive(std::ostream &os, TFormatDataPair &&data) noexcept {
  FormatRecursive(
      os, std::forward<typename TFormatDataPair::first_type>(data.first));
  os << std::forward<typename TFormatDataPair::second_type>(data.second);
}

inline void FormatRecursive(std::ostream & /*unused*/,
                            None /*unused*/) noexcept {}

template <typename TFormatData>
std::string Format(TFormatData &&data) noexcept {
  std::ostringstream ss;
  FormatRecursive(ss, std::forward<typename TFormatData::type>(data.list));
  return ss.str();
}
}

#define FORMAT(...)                                                            \
  Assert::Format(Assert::FormatData<Assert::None>() << __VA_ARGS__)

#define PADS_ASSERT_ASSERT(message, ...)                                       \
  do {                                                                         \
    if (!(__VA_ARGS__)) {                                                      \
      Assert::Handler::invoke(#__VA_ARGS__, __FILE__, __LINE__, message);      \
    }                                                                          \
  } while (false)

#define PADS_ASSERT_CODE_EXECUTE(...)                                          \
  do {                                                                         \
    __VA_ARGS__;                                                               \
  } while (false)

#define PADS_ASSERT_CODE_DONT_EXECUTE(...)                                     \
  do {                                                                         \
    (void)sizeof((__VA_ARGS__));                                               \
  } while (false)

#if defined(PADS_ASSERT_ENABLED)
#define PADS_ASSERT_OPT_ENABLED
#endif

#if defined(PADS_ASSERT_SAFE_ENABLED)
#define PADS_ASSERT_ENABLED
#define PADS_ASSERT_OPT_ENABLED
#endif

#if defined(PADS_ASSERT_OPT_ENABLED)
/**
  Assert condition.

  This is intended as a cheap runtime check that can be enabled in all build
  modes
  */
#define assert_opt(...) PADS_ASSERT_ASSERT("", __VA_ARGS__)
/**
  Assert condition, add 'message' to the default failure message.

  This is intended as a cheap runtime check that can be enabled in all build
  modes
  @message can be any streamable object (as in: "my message goes here" ') or any
  combination of streamable objects, as
  in ' "the variable named varName contains: " << varName << ", which is not
  correct" '
  */
#define assert_opt_message(condition, message)                                 \
  PADS_ASSERT_ASSERT(FORMAT(message), condition)
/**
  Preprocess the argument code out of the program if the assertions of the
  corresponding level are disabled.
  */
#define assert_opt_code(...) PADS_ASSERT_CODE_EXECUTE(__VA_ARGS__)
#else
#define assert_opt(...)                                                        \
  do {                                                                         \
    (void)sizeof((__VA_ARGS__));                                               \
  } while (false)
#define assert_opt_message(condition, message) assert_opt(condition)
#define assert_opt_code(...) PADS_ASSERT_CODE_DONT_EXECUTE(__VA_ARGS__)
#endif

#if defined(PADS_ASSERT_ENABLED)
/**
  Assert condition.

  This is intended as a fairly cheap runtime check that can be enabled in all
  the non-release builds
  */
#define assert_normal(...) PADS_ASSERT_ASSERT("", __VA_ARGS__)
/**
  Assert condition, add 'message' to the default failure message.

  This is intended as a fairly cheap runtime check that can be enabled in all
  the non-release builds
  @message can be any streamable object (as in: ' Assert::PringBacktrace{}
  ' or ' "my message goes here" ') or any combination of streamable objects, as
  in ' "the variable named varName contains: " << varName << ", which is not
  correct" << Assert::PrintBacktrace{} '
  */
#define assert_normal_message(condition, message)                              \
  PADS_ASSERT_ASSERT(FORMAT(message), condition)
/**
  Preprocess the argument code out of the program if the assertions of the
  corresponding level are disabled.
  */
#define assert_normal_code(...) PADS_ASSERT_CODE_EXECUTE(__VA_ARGS__)
#else
#define assert_normal(...)                                                     \
  do {                                                                         \
    (void)sizeof((__VA_ARGS__));                                               \
  } while (false)
#define assert_normal_message(condition, message) assert_normal(condition)
#define assert_normal_code(...) PADS_ASSERT_CODE_DONT_EXECUTE(__VA_ARGS__)
#endif

#if defined(PADS_ASSERT_SAFE_ENABLED)
/**
  Assert condition.

  This is intended as a expensive runtime check to be enabled in debug build
  mode only
  */
#define assert_safe(...) PADS_ASSERT_ASSERT("", __VA_ARGS__)
/**
  Assert condition, add 'message' to the default failure message.

  This is intended as a expensive runtime check to be enabled in debug build
  mode only
  @message can be any streamable object (as in: ' Assert::PringBacktrace{}
  ' or ' "my message goes here" ') or any combination of streamable objects, as
  in ' "the variable named varName contains: " << varName << ", which is not
  correct" << Assert::PrintBacktrace{} '
  */
#define assert_safe_message(condition, message)                                \
  PADS_ASSERT_ASSERT(FORMAT(message), condition)
/**
  Preprocess the argument code out of the program if the assertions of the
  corresponding level are disabled.
  */
#define assert_safe_code(...) PADS_ASSERT_CODE_EXECUTE(__VA_ARGS__)
#else
#define assert_safe(...)                                                       \
  do {                                                                         \
    (void)sizeof((__VA_ARGS__));                                               \
  } while (false)
#define assert_safe_message(condition, message) assert_safe(condition)
#define assert_safe_code(...) PADS_ASSERT_CODE_DONT_EXECUTE(__VA_ARGS__)
#endif
