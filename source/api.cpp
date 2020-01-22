#include <vdtplatform/api.h>

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__)
#define PLATFORM_WINDOWS 1
#elif defined( __linux__ )
#define PLATFORM_LINUX 1
#elif defined( __APPLE__ )
#define PLATFORM_OSX 1
#endif

#define USE_GLFW PLATFORM_WINDOWS | PLATFORM_LINUX | PLATFORM_OSX

#if USE_GLFW
#include <vdtplatform/api/glfw/api_glfw.h>
#endif

namespace platform
{
	API::API(const Type type)
		: m_type(type)
		, m_application()
	{

	}

	API::~API()
	{
		delete m_application;
	}

	API* const API::Factory::get()
	{
		if (s_instance != nullptr)
		{
			return s_instance;
		}
		return get(s_platformDefaultType);
	}

	API* const API::Factory::get(const Type type)
	{
		if (s_instance != nullptr)
		{
			if (s_instance->getType() == type)
				return s_instance;
			return nullptr;
		}

		// create the API instance
		if (std::find(s_availableTypes.begin(), s_availableTypes.end(), type) != s_availableTypes.end())
		{
			// create and cache the new requested api and cache
			switch (type)
			{
#ifdef USE_GLFW
			case API::Type::GLFW:
			{
				s_instance = new API_GLFW();
			}
			break;
#endif 
			case API::Type::Null:
			default:
				// TODO: null api
				break;
			}
		}

		if (s_instance != nullptr)
		{
			s_instance->m_application = s_instance->createApplication();
		}			 
		return s_instance;
	}

	std::vector<API::Type> API::Factory::s_availableTypes{
		API::Type::Null
#ifdef USE_GLFW
		, API::Type::GLFW
#endif
	};

	API::Type API::Factory::s_platformDefaultType =
#ifdef USE_GLFW
		API::Type::GLFW
#else 
		API::Type::Null
#endif
		;

	API* API::Factory::s_instance{};
}