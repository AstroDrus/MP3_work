#pragma once
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cstdint>

#include "ParseMP3.h"
#include "GetHeader.h"

namespace MP3
{

class ID3v2Header
{
public:

std::uint8_t get_version_();
std::uint8_t get_sub_version_();
std::uint8_t get_flag_();
std::uint32_t get_size_();

void set_version_(const std::uint8_t val);
void set_sub_version_(const std::uint8_t val);
void set_flag_(const std::uint8_t val);
void set_size_(const std::uint32_t val);

std::uint32_t make_right_sizeadr(uint32_t size);

private:
    std::uint32_t size_ = 0;
    std::uint8_t version_ = 0;
    std::uint8_t sub_version_ = 0;
    std::uint8_t flag_ = 0;
};

std::ostream& operator<<( std::ostream& stream, ID3v2Header& out_head );

}
