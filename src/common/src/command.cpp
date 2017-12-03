#include "common/command.h"
#include <boost/program_options.hpp>

// GenericOptions

std::ostream &operator<<(std::ostream &out, const GenericOptions &options) {
  out << "help: " << options.help << '\n';
  out << "debug: " << options.debug << std::endl;

  return out;
}

void GenericOptions::printHelp(std::ostream &out) {
  out << "usage:\n";
  out << "pads [--debug] [--help] command [command options]" << '\n';
  out << "where command is one of:" << '\n';
  for (const auto &command : CommandFactory::Instance().registered()) {
    if (!command.empty()) {
      out << '\t' << command << '\n';
    }
  }
  out << R"(use "pads --help command" for help on the usage of specifi commands)"
      << std::endl;
}

// Command

std::ostream &operator<<(std::ostream &out, const Command &command) {
  out << "--------------------------------------------\n";
  out << "Here are the options as seen by the program:\n";
  out << command.genericOptions << '\n';
  command.print(out);
  out << "--------------------------------------------" << std::endl;
  return out;
}

Command::Command(const GenericOptions &genericOptions)
    : genericOptions(genericOptions) {}

void Command::operator()() {
  if (genericOptions.debug) {
    std::cout << *this;
  }
}

// NoCommand

struct NoCommand : Command {
  NoCommand(const GenericOptions &genericOptions,
            const std::vector<std::string> & /*unused*/);

  void operator()() override;

private:
  void print(std::ostream &out) const override;
};

NoCommand::NoCommand(const GenericOptions &genericOptions,
                     const std::vector<std::string> & /*unused*/)
    : Command(genericOptions) {}

void NoCommand::operator()() {
  if (genericOptions.help) {
    GenericOptions::printHelp(std::cout);
  }
}

void NoCommand::print(std::ostream &out) const {
  out << "no command was specified" << std::endl;
}

static auto noCommandRegistered = CommandFactory::Instance().add(
    "", [](const GenericOptions &genericOptions,
           const std::vector<std::string> &options) {
      return std::make_unique<NoCommand>(genericOptions, options);
    });

std::unique_ptr<Command> parseOptions(int argc, const char *argv[]) {
  namespace po = boost::program_options;

  po::options_description global("Global options");
  global.add_options()("help", "shows the help message")("debug",
                                                         "enable debugging")(
      "command", po::value<std::string>(),
      "command to execute")("subargs", po::value<std::vector<std::string>>(),
                            "Arguments for command");

  po::positional_options_description pos;
  pos.add("command", 1).add("subargs", -1);

  po::parsed_options parsed = po::command_line_parser(argc, argv)
                                  .options(global)
                                  .positional(pos)
                                  .allow_unregistered()
                                  .run();
  po::variables_map vm;
  po::store(parsed, vm);

  GenericOptions genericOptions{};
  genericOptions.help = (vm.count("help") != 0u);
  genericOptions.debug = (vm.count("debug") != 0u);

  if (vm.count("command") != 0u) {
    std::string cmd = vm["command"].as<std::string>();

    // Collect all the unrecognized options from the first pass. This will
    // include the
    // (positional) command name, so we need to erase that.
    std::vector<std::string> opts =
        po::collect_unrecognized(parsed.options, po::include_positional);
    opts.erase(opts.begin());

    return CommandFactory::Instance().create(vm["command"].as<std::string>(),
                                             genericOptions, opts);
  }

  (void)(noCommandRegistered);
  return CommandFactory::Instance().create("", genericOptions,
                                           std::vector<std::string>{});
}
