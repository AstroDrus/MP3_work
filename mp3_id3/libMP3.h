#pragma once
#include <fstream>
#include <string>
#include <cstdint>

namespace MP3
{
struct ID3
{
    std::uint8_t version_ = 0;
    std::uint8_t sub_version_ = 0;
    std::uint8_t flag_ = 0;
    std::uint32_t size_ = 0;
};

//give to the main.cpp the head of the MP3_file
ID3 GetHeading( std::ifstream& source_file);
} // namespace MP3

