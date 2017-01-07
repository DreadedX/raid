//----------------------------------------------
#include "flexy/plugin.h"
//----------------------------------------------
class Default : public Plugin {
	public:
		virtual std::shared_ptr<std::iostream> process(std::string file_path) override {

			return open_file(file_path);
		}
};
//----------------------------------------------
share_plugin(Default)
//----------------------------------------------

