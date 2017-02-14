#pragma once
//----------------------------------------------
#include <memory>
#include <chrono>

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
			/// @todo Why exactly is this not just in the header...
			static Engine& instance();

			Engine(Engine const&) = delete;
			void operator=(Engine const&) = delete;

			void set_platform(std::unique_ptr<PlatformProxy> impl) { platform = std::move(impl); }
			auto& get_platform() { return platform; }
			auto& get_file_manager() { return file_manager; }
			auto& get_resource() {return resource; }
			auto& get_timer() {return timer; }
		protected:
			Engine();
		private:
			std::unique_ptr<PlatformProxy> platform;
			FileManager file_manager;
			ResourceManager resource;

			class {
				public:
					void start() { begin = std::chrono::high_resolution_clock::now(); }
					void end() { delta = std::chrono::high_resolution_clock::now() - begin; }
					auto get_delta() { return delta.count(); }

				private:
					std::chrono::time_point<std::chrono::high_resolution_clock> begin;
					std::chrono::duration<float> delta = std::chrono::seconds{0};
			} timer;
	};
	//----------------------------------------------
}
