#pragma once
//----------------------------------------------
#include <memory>

#include "raid/graphics/manager.h"
#include "raid/io/manager.h"
#include "raid/resource.h"
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	class Engine {
		public:
			~Engine();
			static std::shared_ptr<Engine> instance();

			Engine(Engine const&) = delete;
			void operator=(Engine const&) = delete;

			std::shared_ptr<GraphicsManager> get_graphics() { return graphics; }
			std::shared_ptr<FileManager> get_file_manager() { return file_manager; }
			std::shared_ptr<ResourceManager> get_resource() { return resource; }
		protected:
			Engine();
		private:
			static std::shared_ptr<Engine> engine;

			std::shared_ptr<GraphicsManager> graphics;
			std::shared_ptr<FileManager> file_manager;
			std::shared_ptr<ResourceManager> resource;
	};
	//----------------------------------------------
}
