#include "common/command.h"

#include "gtest/gtest.h"

namespace {
struct DummyCommand : Command {
  static std::string ID() { return "dummy_command"; }

  DummyCommand(const GenericOptions &genericOptions,
               const std::vector<std::string> &options)
      : Command(genericOptions) {
    namespace po = boost::program_options;

    po::variables_map vm;
    po::store(po::command_line_parser(options).options(desc).run(), vm);

    po::notify(vm);

    if (vm.count("credentials") == 0u) {
      throw BadParameters{};
    }
    credentials = vm["credentials"].as<std::string>();
    production = (vm.count("production") != 0u);
  }

  void operator()() override {}

private:
  std::string credentials;
  bool production = false;
  static boost::program_options::options_description desc;

  void print(std::ostream & /*out*/) const override {}
  static void printHelp() {}
};

auto populateOptions() {
  namespace po = boost::program_options;
  po::options_description desc(DummyCommand::ID());
  desc.add_options()("credentials", po::value<std::string>(),
                     "json file containing the credentials")("production", "");
  return desc;
}

boost::program_options::options_description DummyCommand::desc =
    populateOptions();

auto dummyCommandRegistered = CommandFactory::Instance().add(
    DummyCommand::ID(), [](const GenericOptions &genericOptions,
                           const std::vector<std::string> &options) {
      return std::make_unique<DummyCommand>(genericOptions, options);
    });
} // namespace

TEST(Command, NoCommand) {
  const char *argv[] = {"pads"};
  EXPECT_TRUE(dummyCommandRegistered);
  EXPECT_NO_THROW(parseOptions(1, argv));
}

TEST(Command, UnknownCommand) {
  const char *argv[] = {"pads", "unknown"};
  EXPECT_THROW(parseOptions(2, argv), CommandException);
}

TEST(Command, DummyCommandNoParms) {
  const char *argv[] = {"pads", "dummy_command"};
  EXPECT_THROW(parseOptions(2, argv), Command::BadParameters);
}

TEST(Command, DummyCommand) {
  const char *argv[] = {"pads", "dummy_command", "--credentials", "file",
                        "--production"};
  EXPECT_NO_THROW(parseOptions(5, argv));
}
