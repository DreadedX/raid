#pragma once
//----------------------------------------------
#include <memory>

#include "typedef.h"
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	class GraphicsKit {
		public:
			class Impl {
				public:
					virtual void create_window(int width, int height, std::string title) = 0;
					virtual bool should_window_close() = 0;
					virtual void poll_events() = 0;
					virtual void swap_buffers() = 0;
			};

			/// Set the implementation that GraphicsKit will use
			GraphicsKit(std::unique_ptr<Impl> m_impl) : impl(std::move(m_impl)) {}

			/// Call the implementation
			inline void create_window(int width, int height, std::string title) {
				impl->create_window(width, height, title);
			}
			
			/// Call the implementation
			inline bool should_window_close() {
				return impl->should_window_close();
			}

			/// Call the implementation
			inline void poll_events() {
				impl->poll_events();
			}

			/// Call the implementation
			inline void swap_buffers() {
				impl->swap_buffers();
			}

		private:
			/// Pointer to the implementation
			std::unique_ptr<Impl> impl;
	};
	//----------------------------------------------
}
