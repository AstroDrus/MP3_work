#pragma once
#include <ostream>
#include <cstdint>

namespace MP3
{

struct ID3v2Header
{
    std::uint32_t size = 0;
    std::uint8_t version = 0;
    std::uint8_t sub_version = 0;
    std::uint8_t flag = 0;
}; // struct ID3v2Header

// redefinition for my testing (will be deleted before release)
std::ostream& operator<<( std::ostream& stream, const ID3v2Header& out_head );

} // namespace MP3
 