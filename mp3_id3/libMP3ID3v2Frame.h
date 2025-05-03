#pragma once
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cstdint>

#include "libMP3Worker.h"
#include "libMP3ID3v2Header.h"


namespace MP3
{

constexpr static int frameIdSize_{ 4 };
const static int  MaxFrameSize_{11};

#pragma pack( push, 1 )
struct ID3v2FrameHeader
{
    char Frame_ID[ frameIdSize_ ];
    std::uint32_t Size = 0;
    std::uint16_t Flags = 0;
}; 
#pragma pack( pop )
 
enum EnCoding : std::uint8_t
{
      Iso8859 = 0x00
    , Unicode = 0x01
    , None
};
 
struct Data
{
    EnCoding encoding;
    std::string Information;
};
 
class ID3v2Frame
{
public:
    ID3v2Frame( ID3v2FrameHeader&& header, Data&& data ) 
    : header_(std::move(header)), data_(std::move(data)){}
    
    ID3v2Frame( ID3v2FrameHeader&& header ) 
    : header_(std::move(header)){}
    
    ID3v2Frame(): header_(), data_()
    {
        data_.encoding = None;
        data_.Information;
        header_.Frame_ID[ frameIdSize_ ] = {'0'};
        header_.Size = 0;
        header_.Flags = 0;
    };

public:
std::string get_Frame_ID_();
std::uint32_t get_size_();
std::uint16_t get_flags_();
std::string get_encoding_();
std::string get_Information_();

void set_Frame_ID_(const char val[frameIdSize_]);
void set_size_(const std::uint32_t val);
void set_flags_(const std::uint16_t val);
void set_encoding_(const EnCoding val);
void set_Information_(const std::string val);

ID3v2Frame operator= (const ID3v2Frame& other )
{
    this->data_ = other.data_;
    this->header_ = other.header_;
    return *this;
}

private:
    ID3v2FrameHeader header_;
    Data data_;
};

std::ostream& operator<<( std::ostream& stream, ID3v2Frame& out_frame );

}