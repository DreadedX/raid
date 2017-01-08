//----------------------------------------------
#include <GLES3/gl3.h>
#include <android/log.h>

#include "raid/platform/android/android.h"

#include "logger.h"

//----------------------------------------------
static void engine_handle_cmd(android_app*, int32_t cmd) {
	switch(cmd) {
		case APP_CMD_INIT_WINDOW:
			break;
		case APP_CMD_TERM_WINDOW:
			break;
	}
}
//----------------------------------------------
raid::Android::Android(android_app* app) { 
	app_dummy();
	app->onAppCmd = engine_handle_cmd;
	this->app = app;
}
//----------------------------------------------
void raid::Android::create_window(int, int, std::string) {
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    EGLint format;
    EGLint numConfigs;
    EGLConfig config;

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires.
     * find the best match if possible, otherwise use the very first one
     */
    eglChooseConfig(display, attribs, nullptr,0, &numConfigs);
    std::unique_ptr<EGLConfig[]> supportedConfigs(new EGLConfig[numConfigs]);
    assert(supportedConfigs);
    eglChooseConfig(display, attribs, supportedConfigs.get(), numConfigs, &numConfigs);
    assert(numConfigs);
    auto i = 0;
    for (; i < numConfigs; i++) {
        auto& cfg = supportedConfigs[i];
        EGLint r, g, b, d;
        if (eglGetConfigAttrib(display, cfg, EGL_RED_SIZE, &r)   &&
            eglGetConfigAttrib(display, cfg, EGL_GREEN_SIZE, &g) &&
            eglGetConfigAttrib(display, cfg, EGL_BLUE_SIZE, &b)  &&
            eglGetConfigAttrib(display, cfg, EGL_DEPTH_SIZE, &d) &&
            r == 8 && g == 8 && b == 8 && d == 0 ) {

            config = supportedConfigs[i];
            break;
        }
    }
    if (i == numConfigs) {
        config = supportedConfigs[0];
    }

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    surface = eglCreateWindowSurface(display, config, app->window, NULL);
	const EGLint context_version[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
    context = eglCreateContext(display, config, NULL, context_version);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        warning << "Unable to eglMakeCurrent\n";
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &width);
    eglQuerySurface(display, surface, EGL_HEIGHT, &height);

    // Check openGL on the system
    auto opengl_info = {GL_VENDOR, GL_RENDERER, GL_VERSION, GL_EXTENSIONS};
    for (auto name : opengl_info) {
        auto info = glGetString(name);
        warning << "OpenGL Info: " << info << '\n';
    }
    // Initialize GL state.
    // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    // glEnable(GL_CULL_FACE);
    // glShadeModel(GL_SMOOTH);
    // glDisable(GL_DEPTH_TEST);
}
//----------------------------------------------
void raid::Android::terminate() {
	if (display != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (context != EGL_NO_CONTEXT){
			eglDestroyContext(display, context);
		}
		if (surface != EGL_NO_SURFACE){
			eglDestroySurface(display, surface);
		}
		eglTerminate(display);
	}
	display = EGL_NO_DISPLAY;
	context = EGL_NO_CONTEXT;
	surface = EGL_NO_SURFACE;
}
//----------------------------------------------
bool raid::Android::should_window_close() {

	return app->destroyRequested;
}
//----------------------------------------------
bool raid::Android::has_context() {

	return app->window != nullptr;
}
//----------------------------------------------
void raid::Android::poll_events() {
	int ident;
	int fdesc;
	int events;
	struct android_poll_source* source;

	while((ident = ALooper_pollAll(0, &fdesc, &events, (void**)&source)) >= 0)
	{
		// process this event
		if (source) {
			source->process(app, source);
		}
	}
}
//----------------------------------------------
void raid::Android::swap_buffers() {
    eglSwapBuffers(display, surface);
}
//----------------------------------------------
