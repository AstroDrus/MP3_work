#include <ostream>
#include <string>
#include <cstdint>
#include <cstring>
#include <algorithm>


#include "../Mp3Worker.h"
#include "../ID3v2Frame.h"


namespace MP3
{

ID3v2Frame::ID3v2Frame( ID3v2FrameHeader&& header, Data&& data ) : 
      header_(std::move(header))
    , data_(std::move(data))
{
    
}

ID3v2Frame::ID3v2Frame( ID3v2FrameHeader&& header ) : 
      header_(std::move(header))
    , data_{}
{
    data_.encoding = None;
}

ID3v2Frame::ID3v2Frame() :
      header_{}
    , data_{}
{

};

// frame outstream
std::ostream& operator<<( std::ostream& stream, const ID3v2Frame& out_frame )
{
    stream << "frame ID - "<< out_frame.get_Frame_ID() << "\n"
                << "frame size - " << std::hex << out_frame.get_size() << "\n"
                        << "frame flags - " << int(out_frame.get_flags()) << "\n"
        << std::endl;
    

    
    if (out_frame.get_Frame_ID()[0] == 'T')
    {
         stream << "frame encoding - " << out_frame.get_encoding() << "\n"
        << "frame information - " << out_frame.get_Information() 
        << std::endl;
    }
    
    return stream;
}

//setters for frame header (frame)
std::string ID3v2Frame::get_Frame_ID() const
{
    return header_.Frame_ID;
}

std::uint32_t ID3v2Frame::get_size() const noexcept
{
    return header_.Size;
}

std::uint16_t ID3v2Frame::get_flags() const noexcept
{
    return header_.Flags;
}

EnCoding ID3v2Frame::get_encoding() const noexcept
{
    switch (data_.encoding)
    {
    case Iso8859:
        return Iso8859;
        break;
    
    default:
        return Unicode;
        break;
    }
}

std::string ID3v2Frame::get_Information() const
{
    if (data_.Information.size() == 0)
    {
    return "no data";
    }
    else
    {
    return data_.Information;
    }
}


//setters for frame header (frame)
void ID3v2Frame::set_Frame_ID(const std::array< char, frameIdSize_ >& val)
{
    std::copy(val.begin(), val.end(), header_.Frame_ID);
    header_.Frame_ID[frameIdSize_] = '\0';
}

void ID3v2Frame::set_size(const std::uint32_t val) noexcept
{
    header_.Size = val;
}

void ID3v2Frame::set_flags(const std::uint16_t val) noexcept
{
    header_.Flags = val;
}

void ID3v2Frame::set_encoding(const EnCoding val) noexcept
{
    data_.encoding = val;
}

void ID3v2Frame::set_Information(const std::string& val)
{
    data_.Information = val;
}

std::ostream& operator<<( std::ostream& stream, Mp3Worker& mp3Worker )
{
    return stream << "Header information:\nmarker - ID3" << "\n" << mp3Worker.id3v2Header_ << "\nFrame information: \n"
    << mp3Worker.frame_<< std::endl;
}

}// namespace MP3