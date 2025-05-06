#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cstring>

#include "../ParseMP3.h"
#include "../GetHeader.h"
#include "../GetFrame.h"


namespace MP3
{

// frame outstream
std::ostream& operator<<( std::ostream& stream, ID3v2Frame& out_frame )
{
    
    if (out_frame.get_Frame_ID_()[0] != 'T')
    {
        return stream << "frame ID - "<< out_frame.get_Frame_ID_() << "\n"
        << "frame size - " << std::hex << out_frame.get_size_() << "\n"
        << "frame flags - " << int(out_frame.get_flags_()) << "\n"
        << std::endl;
    }
    else 
    {
        return stream << "frame ID - "<< out_frame.get_Frame_ID_() << "\n"
        << "frame encoding - " << out_frame.get_encoding_() << "\n"
        << "frame information - " << out_frame.get_Information_() 
        << std::endl;   
    }
}

//setters for frame header (frame_)
std::string ID3v2Frame::get_Frame_ID_()
{
    return header_.Frame_ID;
}

std::uint32_t ID3v2Frame::get_size_()
{
    return header_.Size;
}

std::uint16_t ID3v2Frame::get_flags_()
{
    return header_.Flags;
}

std::string ID3v2Frame::get_encoding_()
{
    switch (data_.encoding)
    {
    case Iso8859:
        return "Iso8859";
        break;
    
    default:
        return "Unicode";
        break;
    }
}

std::string ID3v2Frame::get_Information_()
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


//setters for frame header (frame_)
void ID3v2Frame::set_Frame_ID_(const char val[frameIdSize_])
{
    strcpy(header_.Frame_ID, val);
}

void ID3v2Frame::set_size_(const std::uint32_t val)
{
    header_.Size = val;
}

void ID3v2Frame::set_flags_(const std::uint16_t val)
{
    header_.Flags = val;
}

void ID3v2Frame::set_encoding_(const EnCoding val)
{
    data_.encoding = val;
}

void ID3v2Frame::set_Information_(const std::string val)
{
    data_.Information = val;
}

std::ostream& operator<<( std::ostream& stream, Mp3Worker& mp3Worker )
{
    return stream << "Header information:\nmarker - ID3" << "\n" << mp3Worker.id3v2Header_ << "\nFrame information: \n"
    << mp3Worker.frame_<< std::endl;
}

}
