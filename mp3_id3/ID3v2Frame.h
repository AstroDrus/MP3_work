#pragma once

#include <ostream>
#include <string>
#include <cstdint>
#include <array>


namespace MP3
{

constexpr static int frameIdSize_{ 4 };
constexpr static int MaxFrameSize_{ 11 };

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

    ID3v2Frame( ID3v2FrameHeader&& header, Data&& data );
    ID3v2Frame( ID3v2FrameHeader&& header );
    ID3v2Frame();

public:
    std::string get_Frame_ID() const;
    std::uint32_t get_size() const noexcept;
    std::uint16_t get_flags() const noexcept; 
    EnCoding get_encoding() const noexcept;
    std::string get_Information() const;
    
    
    void set_Frame_ID(const std::array< char, frameIdSize_ >& val);
    void set_size(const std::uint32_t val) noexcept;
    void set_flags(const std::uint16_t val) noexcept;
    void set_encoding(const EnCoding val) noexcept;
    void set_Information(const std::string& val);


private:
    ID3v2FrameHeader header_;
    Data data_;
}; // class ID3v2Frame

// redefinition for my testing (will be deleted before release)
std::ostream& operator<<( std::ostream& stream, const ID3v2Frame& out_frame );

} // namespace MP3