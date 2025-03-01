#pragma once

#include <chrono>
#include <lyra/args.hpp>
#include <optional>
#include <string>
#include <tuple>
#include <variant>

namespace coronan_cli {

std::variant<
    std::tuple<std::string, std::optional<std::chrono::year_month_day>, std::optional<std::chrono::year_month_day>>,
    int>
parse_commandline_arguments(lyra::args const& args);

} // namespace coronan_cli
