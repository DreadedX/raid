#pragma once
//----------------------------------------------
#include <memory>
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	class GraphicsKit {
		public:
			class Impl {
				public:
					virtual void create_window() = 0;
			};

			GraphicsKit(std::unique_ptr<Impl> m_impl) : impl(std::move(m_impl)) {}

			void create_window() {
				impl->create_window();
			}

		private:
			std::unique_ptr<Impl> impl;
	};
	//----------------------------------------------
};
