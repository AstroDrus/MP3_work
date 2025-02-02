#pragma once
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include <algorithm>

namespace MP3
{

class ID3
{
public:

std::uint8_t* get_adr_version_();
std::uint8_t* get_adr_sub_version_();
std::uint8_t* get_adr_flag_();
std::uint32_t* get_adr_size_();

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

//give to the main.cpp the head of the MP3_file
ID3 GetHeading( std::ifstream& source_file);
} // namespace MP3

