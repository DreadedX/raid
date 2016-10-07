#pragma once
//----------------------------------------------
#include "raid/graphics/kit.h"
//----------------------------------------------
namespace raid {
	//----------------------------------------------
	class OpenGLImpl : public GraphicsKit::Impl {

		public:
			void create_window() override;
	};
	//----------------------------------------------
};
