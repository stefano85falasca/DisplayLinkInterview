#pragma once

#include "common/factory.h"
#include <boost/program_options.hpp>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct GenericOptions {
  bool help = false;
  bool debug = false;

  friend std::ostream &operator<<(std::ostream &out,
                                  const GenericOptions &options);
  static void printHelp(std::ostream &out);
};

struct Command {
  struct BadParameters : std::exception {};
  struct Help : std::exception {};

  friend std::ostream &operator<<(std::ostream &out, const Command &command);

  Command(const GenericOptions &genericOptions);

  virtual void operator()();

protected:
  const GenericOptions &genericOptions;

  virtual void print(std::ostream &out) const = 0;
};

using CommandFactory =
    Factory<Command, std::string,
            std::function<std::unique_ptr<Command>(
                const GenericOptions &, const std::vector<std::string> &)>>;

using CommandException = DefaultFactoryError<std::string, Command>::Exception;

std::unique_ptr<Command> parseOptions(int argc, const char *argv[]);
