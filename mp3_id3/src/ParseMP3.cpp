#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cstring>

#include <arpa/inet.h>

#include "../ParseMP3.h"
#include "../GetHeader.h"
#include "../GetFrame.h"

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
    source_file_.read( reinterpret_cast< char* >( &header.Frame_ID ), frameIdSize_ );
    
    if (header.Frame_ID[0] != 'T')
    {
        source_file_.read( reinterpret_cast< char* >( &header.Size ), sizeof( header.Size ) );
        header.Size = ntohl(header.Size);
        
        source_file_.read( reinterpret_cast< char* >( &header.Flags ), sizeof( header.Flags ) );

        ID3v2Frame frame{std::move(header)};

        frame_ = frame;
    }
    else 
    {        
        Data data;
        source_file_.read( reinterpret_cast< char* >( &( data.encoding ) ), sizeof( EnCoding ) );

        
        for (int i = 0; i < MaxFrameSize_ ; ++i)
        {
            source_file_.read( data.Information.data() , sizeof(std::uint8_t) );
            
            if (data.Information[i] == 0)
            {
                source_file_.seekg(sizeof(std::uint8_t));
                break;
            }
            else
            {
                continue;
            }
            
        }
        
        ID3v2Frame frame{std::move(header), std::move(data)};

        frame_ = frame;
    }
}


} //namespace MP3