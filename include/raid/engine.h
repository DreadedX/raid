#pragma once
//----------------------------------------------
#include <memory>

#include "raid/platform/proxy.h"
#include "raid/io/manager.h"
#include "raid/resource.h"

#include "logger.h"
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	class Engine {
		public:
			~Engine() { debug << "Engine destructed\n"; }
			// static std::shared_ptr<Engine> instance();
			static Engine& instance();

			Engine(Engine const&) = delete;
			void operator=(Engine const&) = delete;

			// std::shared_ptr<GraphicsManager> get_graphics() { return graphics; }
			// std::shared_ptr<FileManager> get_file_manager() { return file_manager; }
			// std::shared_ptr<ResourceManager> get_resource() { return resource; }
			auto& get_platform() { return platform; }
			auto& get_file_manager() { return file_manager; }
			auto& get_resource() {return resource; }
		protected:
			Engine();
		private:
			// std::shared_ptr<GraphicsManager> graphics = std::make_shared<GraphicsManager>();
			// std::shared_ptr<FileManager> file_manager = std::make_shared<FileManager>();
			// std::shared_ptr<ResourceManager> resource = std::make_shared<ResourceManager>();
			PlatformProxy platform;
			FileManager file_manager;
			ResourceManager resource;
	};
	//----------------------------------------------
}
