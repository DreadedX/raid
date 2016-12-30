#pragma once
//----------------------------------------------
#include <iostream>
#include <memory>

#include "typedef.h"
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	class PlatformProxy {
		public:
			class Impl {
				public:
					virtual ~Impl() {}

					virtual void create_window(int width, int height, std::string title) = 0;
					virtual void terminate() = 0;
					virtual bool should_window_close() = 0;
					virtual bool has_context() = 0;
					virtual void poll_events() = 0;
					virtual void swap_buffers() = 0;

					/// @todo Return a shader asset derived from a platform independent shader asset
					// virtual uint load_shader() = 0;
					virtual void test_setup() = 0;
					virtual void test_render() = 0;
			};

			/// Set the implementation that GraphicsKit will use
			// @todo Add error handling when the impl is not set
			template <class T>
			void set_impl(std::unique_ptr<T> t) { impl = std::move(t); }
			
			/// Call the implementation
			inline void create_window(int width, int height, std::string title) { impl->create_window(width, height, title); }
			inline void terminate() { impl->terminate(); }
			inline bool should_window_close() { return impl->should_window_close(); }
			inline bool has_context() { return impl->has_context(); }
			inline void poll_events() { impl->poll_events(); }
			inline void swap_buffers() { impl->swap_buffers(); }

			// inline uint load_shader() { return impl->load_shader(); }
			inline void test_setup() { impl->test_setup(); }
			inline void test_render() { impl->test_render(); }

		private:
			/// Pointer to the implementation
			std::unique_ptr<Impl> impl;
	};
	//----------------------------------------------
}
