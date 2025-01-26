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
    std::ifstream source_file(argv[2], std::ios::binary);

    try 
    {
        MP3::GetHeading(source_file); 
        std::cout << "nice" << std::endl;
    }
    catch( const std::exception& exeption )
    {
        std::cout << exeption.what() << std::endl;
    }

    return 0;
}
