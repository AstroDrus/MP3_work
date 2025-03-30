#pragma once
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cstdint>

namespace MP3
{
 
constexpr static int frameIdSize_{ 4 };
 
#pragma pack( push, 1 )
struct ID3v2FrameHeader
{
    char Frame_ID[ frameIdSize_ ];
    std::uint32_t Size;
    std::uint16_t Flags;
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
EnCoding get_encoding_();
std::string get_Information_();

void set_Frame_ID_(const char val[frameIdSize_]);
void set_size_(const std::uint32_t val);
void set_flags_(const std::uint16_t val);
void set_encoding_(const EnCoding val);
void set_Information_(const std::string val);

ID3v2Frame operator= (const ID3v2Frame& other )
{
    if (this != &other)
    {
        header_ = other.header_;
        data_ = other.data_;
    }
    return *this;
}

private:
    ID3v2FrameHeader header_;
    Data data_;
};

std::ostream& operator<<( std::ostream& stream, ID3v2Frame& out_frame );


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


class Mp3Worker
{
friend std::ostream& operator<<( std::ostream& stream, Mp3Worker& mp3Worker );

public:
    Mp3Worker(std::ifstream&& path2file ) 
    : source_file_(std::move(path2file))
    {
        ParseId3V2Header();
        ReadFrame();
    }

private:
    void ParseId3V2Header();
    void ReadFrame();
 
 
private:
    std::ifstream source_file_;
    ID3v2Header id3v2Header_;
    ID3v2Frame frame_;
}; // class Mp3Worker
 
 
std::ostream& operator<<( std::ostream& stream, Mp3Worker& mp3Worker );

} // namespace MP3

