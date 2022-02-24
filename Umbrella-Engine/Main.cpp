#include "Application/Application.h"
#include <iostream>


//#include <boost/array.hpp>
//using namespace UE;


int main(int argc, char* argv[])
{
	const std::string FilePath = "C:/Users/ДНС/Documents/Projects/VisualStudioProjects/JFla-Engine/JFlaEngine/Resources/Images/dendy.png";
	const std::string SavePath = std::filesystem::path(std::string("C:/Users/ДНС/Documents/Projects/VisualStudioProjects/JFla-Engine/JFlaEngine/Resources/Images/Flopped.png")).string();
	
	// sleepy-cat.jpg 2560x1600 

	//auto image = Ref<Utils::Image>(Utils::ImageLoader::Load(FilePath));
	//auto result = MakeRef<Utils::Image>();

	//Utils::ImageUtils::HorizontalFlip(image, *result);

	//if (!Utils::ImageLoader::Save(SavePath, result))
	//{
	//	std::cout << OIIO::geterror() << '\n';
	//	return 1;
	//}

	//auto image = Utils::ImageLoader::Load(TexturePath);

	//if (!image)
	//{
	//	std::cout << std::format("Could not load {} image.\n", TexturePath);
	//	return EXIT_FAILURE;
	//}
	

	UE::Application app( "Umbrella Editor", 720, 720 );

	app.Run();

	return 0;
}

#undef main