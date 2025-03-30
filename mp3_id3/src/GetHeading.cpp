#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cstring>

#include <arpa/inet.h>

#include "../libMP3.h"

namespace MP3
{

//give to the main.cpp the head of the MP3_file
void Mp3Worker :: ParseId3V2Header()
{  

    static const std::string ID3_head = "ID3";
    // array-buffer for the result Class (besides size_) of this func (C-style) 
    //verification if file can be opened  
    if (!source_file_.is_open())
    {

        throw std::invalid_argument("Incorrect source file name!"); 

    }
    // amount of bayts in head of MP3_file
    static constexpr const int sizeID3 = 3;
    // array-buffer for the result Class (besides size_) (C-style) 
    char signature[sizeID3+1] = {'\0'};
    source_file_.read(signature, sizeID3);
    if (ID3_head != signature)
    {
        throw std::invalid_argument("source file is not an MP3 file with IDv3 tag");
    }
       
    // besides size_
    constexpr int size_of_elements = 3;

    std::uint8_t array[ size_of_elements ] = { 0 };

    for (int i = 0; i < size_of_elements; ++i) 
    {
        source_file_.read( reinterpret_cast< char* >( &array[ i ] ), sizeof( ( array[ i ] ) ) );
    }

    id3v2Header_.set_version_( array[0] );
    id3v2Header_.set_sub_version_( array[1] ); 
    id3v2Header_.set_flag_( array[2] );

    std::uint32_t buffer_size;
    source_file_.read( reinterpret_cast< char* >( &buffer_size ), sizeof( buffer_size) );
    id3v2Header_.set_size_( buffer_size );
    id3v2Header_.set_size_(id3v2Header_.make_right_sizeadr(id3v2Header_.get_size_()));
}

void Mp3Worker::ReadFrame()
{
    ID3v2FrameHeader header;
    source_file_.read( reinterpret_cast< char* >( &header ), sizeof( header) );
 
    Data data;
    source_file_.read( reinterpret_cast< char* >( &( data.encoding ) ), sizeof( EnCoding ) );
    // minus encoding
    const std::uint32_t dataSize{ header.Size - 1 };
 
    data.Information.resize( dataSize );
 
    source_file_.read( reinterpret_cast< char* >( data.Information.data() ), sizeof( dataSize ) );
    
    ID3v2Frame frame{std::move(header), std::move(data)};

    frame_ = frame;



    //frame_.header_= header;
    //frame_.data_ = data;
}


std::uint32_t ID3v2Header::make_right_sizeadr( std::uint32_t size )
{
 size = ntohl(size);
 
		constexpr static std::uint32_t mask = 0x7F;
		
		std::uint32_t prevMask{0};
		for( std::size_t byte = 1; byte < sizeof( std::uint32_t ); ++byte )
		{
       const std::uint32_t crntByteMask = (( prevMask << 7 ) | mask);
       const std::uint32_t oldButGold = ( size & crntByteMask );
       const std::uint32_t shifted = size >> 1;
       const std::uint32_t newValue = ( shifted & ~( crntByteMask ) );
       
       size = ( (oldButGold) | newValue );
       
       prevMask = crntByteMask;
		}

return size;
}

//getters for main header (id3v2Header_)
std::uint8_t ID3v2Header::get_version_()
{
	return version_;
} 

std::uint8_t ID3v2Header::get_sub_version_()
{
	return sub_version_;
} 

std::uint8_t ID3v2Header::get_flag_()
{
	return flag_;
} 

std::uint32_t ID3v2Header::get_size_()
{
	return size_;
} 

//setters for main header (id3v2Header_)
void ID3v2Header::set_version_(const std::uint8_t val)
{
	version_ = val;
}

void ID3v2Header::set_sub_version_(const std::uint8_t val)
{
	sub_version_ = val;
}

void ID3v2Header::set_flag_(const std::uint8_t val)
{
	flag_ = val;
}

void ID3v2Header::set_size_(const std::uint32_t val)
{
	size_ = val;
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

EnCoding ID3v2Frame::get_encoding_()
{
    return data_.encoding;
}

std::string ID3v2Frame::get_Information_()
{

     return data_.Information;
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
        
    return stream << "marker - ID3" << "\n" << mp3Worker.id3v2Header_ << "\n"
    << mp3Worker.frame_<< std::endl;
        
}

} //namespace MP3