#include <iostream>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

#include "libMP3.h"


int main(int argc, char ** argv )
{
    if (argc != 2)
    {
        std::cerr << "wrong count of files!\n";
        return 1;
    }
    // MP3_file for the result after using MP3::GetHeading
    std::ofstream Head_file("Head_file.txt", std::ios_base::app);
    // source MP3_file for the MP3::GetHeading
    std::ifstream source_file(argv[1], std::ios::binary);

    try 
    {
        MP3::ID3 id3;
        id3 = MP3::GetHeading(source_file); 
        
        std::cout << "marker - ID3\n" << "version is - " << int(id3.get_version_()) << "\n" << "sub version is - " << int(id3.get_sub_version_())
        << "\n" << "flag is - " << int(id3.get_flag_()) << "\n" << "size is - " << std::hex << id3.get_size_() << std::endl;

    }
    catch( const std::exception& exeption )
    {
        std::cout << exeption.what() << std::endl;
    }

    return 0;
}
