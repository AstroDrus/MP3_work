#include <fstream>
#include <cstdint>

#include <arpa/inet.h>

#include "../ID3v2Frame.h"
#include "../ID3v2Header.h"
#include "../Mp3Worker.h"


namespace MP3
{
Mp3Worker :: Mp3Worker(std::ifstream&& path2file ) 
    : source_file_(std::move(path2file))
    {
        ParseId3V2Header();
        ReadFrame();
    }


std::uint32_t Mp3Worker::make_right_sizeadr( std::uint32_t size )
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
    
    std::uint8_t array[ size_of_version_flag ] = { 0 };

    for (int i = 0; i < size_of_version_flag; ++i) 
    {
        source_file_.read( reinterpret_cast< char* >( &array[ i ] ), sizeof( ( array[ i ] ) ) );
    }

    id3v2Header_.version = array[0];
    id3v2Header_.sub_version = array[1]; 
    id3v2Header_.flag = array[2];

    std::uint32_t buffer_size;
    source_file_.read( reinterpret_cast< char* >( &buffer_size ), sizeof( buffer_size) );
    id3v2Header_.size = buffer_size;
    id3v2Header_.size = make_right_sizeadr(id3v2Header_.size);
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