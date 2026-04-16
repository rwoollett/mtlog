#pragma once
#include <string>
#include <utility>
#include <fmt/format.h>

// Custom formatter for std::pair<std::string,std::string>
template <>
struct fmt::formatter<std::pair<std::string, std::string>> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const std::pair<std::string, std::string>& p,
                FormatContext& ctx) const {
        return fmt::format_to(ctx.out(), "{} = {}", p.first, p.second);
    }
};

