#include "Application.h"

#include <iostream>
#include <stdexcept>

int main() 
{
	Vulkan::Application app;
    
    try
    {
        app.Run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}