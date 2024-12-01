#include <glw.hpp>

namespace glw {

	void Context::initialize() {
		int _err = glfwInit();
		if (_err != GLFW_TRUE) {
			_GLW_THROW_GLFW_ERROR;
		}
	}

	void Context::make_current(Window* _target) {
		glfwMakeContextCurrent(_target->get());
	}

	void Context::activate() {
		int _glad_err_code = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		if (_glad_err_code != 1) {
			throw exception::GLADLoadingFailure();
		}
	}

	void Context::set_interval(int _value) noexcept {
		glfwSwapInterval(_value);
	}

	void Context::finalize() noexcept {
		glfwTerminate();
	}

	double Context::get_time() noexcept {
		return glfwGetTime();
	}

	ErrorType Context::get_error() noexcept {
		return static_cast<ErrorType>(glGetError());
	}

	void Context::swap_buffers(Window* _target) {
		glfwSwapBuffers(_target->get());
	}

	void Context::poll_events() noexcept {
		glfwPollEvents();
	}

	void Context::viewport(int _xpos, int _ypos, uint32_t _width, uint32_t _height) {
		glViewport(_xpos, _ypos, static_cast<int>(_width), static_cast<int>(_height));
	}

	void Context::get_viewport(int& _out_xpos, int& _out_ypos, uint32_t& _out_width, uint32_t& _out_height) {
		int _vp[4] = { 0 };
		glGetIntegerv(GL_VIEWPORT, &_vp[0]);
		_out_xpos = _vp[0];
		_out_ypos = _vp[1];
		_out_width = static_cast<uint32_t>(_vp[2]);
		_out_height = static_cast<uint32_t>(_vp[3]);
	}

	bool Context::get_is_enabled(CapabilityType _type) {
		return glIsEnabled(static_cast<GLenum>(_type));
	}

	void Context::blend_func(BlendFuncType _src, BlendFuncType _dst) {
		glBlendFunc(static_cast<GLenum>(_src), static_cast<GLenum>(_dst));
	}

	void Context::enable(CapabilityType _type) {
		glEnable(static_cast<GLenum>(_type));
	}

	void Context::disable(CapabilityType _type) {
		glDisable(static_cast<GLenum>(_type));
	}

	void Context::set_line_width(float _value) {
		glLineWidth(_value);
	}
	
	void Context::set_scissor(int _x, int _y, uint32_t _w, uint32_t _h) {
		glScissor(_x, _y, static_cast<GLsizei>(_w), static_cast<GLsizei>(_h));
	}

	void Context::set_culling_mode(CullingMode _mode) {
		glCullFace(static_cast<GLenum>(_mode));
	}

	void Context::clear_buffer(BufferType _flags) {
		glClear(static_cast<GLbitfield>(_flags));
	}

	void Context::clear_color(const float& _r, const float& _g, const float& _b, const float& _a) {
		glClearColor(_r, _g, _b, _a);
	}

	uint32_t Context::generate_vertex_array() {
		uint32_t _id = 0U;
		glGenVertexArrays(1, &_id);
		return _id;
	}

	void Context::bind_vertex_array(const uint32_t& _id) {
		glBindVertexArray(_id);
	}

	void Context::enable_vertex_attrib_array(const uint32_t& _offset_index) {
		glEnableVertexAttribArray(_offset_index);
	}

	void Context::vertex_attrib_pointer(const uint32_t& _offset_index, const uint32_t& _count, const NumericType& _type, bool _norm, const size_t& _stride, const intptr_t& _offset) {
		glVertexAttribPointer(_offset_index, _count, static_cast<uint32_t>(_type), _norm, static_cast<GLsizei>(_stride), reinterpret_cast<const GLvoid*>(_offset));
	}

	void Context::delete_vertex_array(const uint32_t& _id) {
		glDeleteVertexArrays(1, &_id);
	}

	uint32_t Context::generate_buffer() {
		uint32_t _id = 0U;
		glGenBuffers(1, &_id);
		return _id;
	}

	void Context::bind_array_buffer(const ArrayBufferType& _type, const uint32_t& _id) {
		glBindBuffer(static_cast<GLenum>(_type), _id);
	}

	void Context::bind_array_buffer_data(const ArrayBufferType& _type, const BufferData& _data, BufferUsage _usage) {
		glBufferData(static_cast<GLenum>(_type), _data.get_length(), _data.get_pointer(), static_cast<GLenum>(_usage));
	}

	void Context::bind_empty_array_buffer_data(const ArrayBufferType& _type, const size_t& _length, BufferUsage _usage) {
		glBufferData(static_cast<GLenum>(_type), _length, nullptr, static_cast<GLenum>(_usage));
	}

	void Context::bind_array_buffer_sub_data(const ArrayBufferType& _type, const BufferData& _data, const size_t& _offset) {
		glBufferSubData(static_cast<GLenum>(_type), _offset, _data.get_length(), _data.get_pointer());
	}

	void Context::delete_buffer(const uint32_t& _id) {
		glDeleteBuffers(1, &_id);
	}

	uint32_t Context::generate_frame_buffer() {
		uint32_t _id = 0U;
		glGenFramebuffers(1, &_id);
		return _id;
	}

	void Context::bind_frame_buffer(const uint32_t& _id) {
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
	}

	void Context::attach_texture_2d(const uint32_t& _fbo, AttachmentType _type, const uint32_t& _id) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(_type), GL_TEXTURE_2D, _id, 0);
		Context::check_framebuffer_link_status();
	}

	void Context::attach_render_buffer(const uint32_t& _fbo, AttachmentType _type, const uint32_t& _rbo) {
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, static_cast<GLenum>(_type), GL_RENDERBUFFER, _rbo);
		Context::check_framebuffer_link_status();
	}

	void Context::check_framebuffer_link_status() {
		FramebufferLinkStatusType _err = static_cast<FramebufferLinkStatusType>(glCheckFramebufferStatus(GL_FRAMEBUFFER));
		if (_err != FramebufferLinkStatusType::Complete) {
			throw exception::GLFramebufferLinkFailure(FrameBufferObject::link_status_type_to_str(_err));
		}
	}

	void Context::delete_frame_buffer(const uint32_t& _id) {
		glDeleteFramebuffers(1, &_id);
	}

	void Context::read_buffer(ReadBufferType _type) {
		glReadBuffer(static_cast<GLenum>(_type));
	}

	void Context::draw_buffer(DrawBufferType _type) {
		glDrawBuffer(static_cast<GLenum>(_type));
	}

	uint32_t Context::generate_render_buffer() {
		uint32_t _id = 0U;
		glGenRenderbuffers(1, &_id);
		return _id;
	}

	void Context::bind_render_buffer(const uint32_t& _id) {
		glBindRenderbuffer(GL_RENDERBUFFER, _id);
	}

	void Context::set_render_buffer_storage(InternalFormat _format, const uint32_t& _width, const uint32_t& _height) {
		glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(_format), _width, _height);
	}

	void Context::delete_render_buffer(const uint32_t& _id) {
		glDeleteRenderbuffers(1, &_id);
	}

	uint32_t Context::generate_texture() {
		uint32_t _id = 0U;
		glGenTextures(1, &_id);
		return _id;
	}

	void Context::set_filter(FilterType _mag, FilterType _min) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(_mag));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(_min));
	}

	void Context::set_wrapping_mode(WrappingType _s, WrappingType _t) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(_s));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(_t));
	}

	void Context::set_image(const unsigned char* _data, uint32_t _width, uint32_t _height, InternalFormat _i_format, TextureFormat _t_format, DataType _type) {
		glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(_i_format), _width, _height, 0, static_cast<GLenum>(_t_format), static_cast<GLenum>(_type), _data);
	}

	void Context::generate_mipmap() {
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void Context::activate_texture(const TextureSlot& _slot) {
		glActiveTexture(static_cast<GLenum>(_slot));
	}

	void Context::bind_texture_2d(const uint32_t& _id) {
		glBindTexture(GL_TEXTURE_2D, _id);
	}

	void Context::delete_texture(const uint32_t& _id) {
		glDeleteTextures(1, &_id);
	}

	uint32_t Context::create_shader(const ShaderType& _type) {
		return glCreateShader(static_cast<GLenum>(_type));
	}

	void Context::set_source(const uint32_t& _id, const char* _src) {
		glShaderSource(_id, 1, &_src, nullptr);
	}

	void Context::compile_shader(const ShaderType& _type, const uint32_t& _id) {
		glCompileShader(_id);
		Context::check_shader_compile_status(_type, _id);
	}

	void Context::delete_shader(const uint32_t& _id) {
		glDeleteShader(_id);
	}

	void Context::check_shader_compile_status(const ShaderType& _type, const uint32_t& _id) {
		int _success = 0;
		glGetShaderiv(_id, GL_COMPILE_STATUS, &_success);
		if (_success == 0) {
			char _log[1024] = { '\0' };
			glGetShaderInfoLog(_id, 1024, nullptr, _log);
			throw exception::GLShaderCompileFailure(Shader::type_to_str(_type), _log);
		}
	}

	uint32_t Context::create_program() {
		return glCreateProgram();
	}

	void Context::check_program_link_status(const uint32_t& _id) {
		int _success = 0;
		glGetProgramiv(_id, GL_LINK_STATUS, &_success);
		if (_success == 0) {
			char _log[1024] = { '\0' };
			glGetProgramInfoLog(_id, 1024, nullptr, _log);
			throw exception::GLProgramLinkFailure(_log);
		}
	}

	int Context::get_uniform_location(const uint32_t& _id, const char* _name) {
		return glGetUniformLocation(_id, _name);
	}

	void Context::set_uniform_int(const int& _loc, int _value) {
		glUniform1i(_loc, _value);
	}

	void Context::set_uniform_uint(const int& _loc, uint32_t _value) {
		glUniform1ui(_loc, _value);
	}

	void Context::set_uniform_float(const int& _loc, float _value) {
		glUniform1f(_loc, _value);
	}

	void Context::set_uniform_vec2(const int& _loc, float _x, float _y) {
		glUniform2f(_loc, _x, _y);
	}

	void Context::set_uniform_vec3(const int& _loc, float _x, float _y, float _z) {
		glUniform3f(_loc, _x, _y, _z);
	}

	void Context::set_uniform_vec4(const int& _loc, float _x, float _y, float _z, float _w) {
		glUniform4f(_loc, _x, _y, _z, _w);
	}

	void Context::set_uniform_mat2(const int& _loc, const float* _ptr) {
		glUniformMatrix2fv(_loc, 1, GL_FALSE, _ptr);
	}

	void Context::set_uniform_mat3(const int& _loc, const float* _ptr) {
		glUniformMatrix3fv(_loc, 1, GL_FALSE, _ptr);
	}

	void Context::set_uniform_mat4(const int& _loc, const float* _ptr) {
		glUniformMatrix4fv(_loc, 1, GL_FALSE, _ptr);
	}

	void Context::use_program(const uint32_t& _id) {
		glUseProgram(_id);
	}

	void Context::attach_shader(const uint32_t& _program, const uint32_t& _shader) {
		glAttachShader(_program, _shader);
	}

	void Context::link_program(const uint32_t& _id) {
		glLinkProgram(_id);
		Context::check_program_link_status(_id);
	}

	void Context::delete_program(const uint32_t& _id) {
		glDeleteProgram(_id);
	}

	void Context::draw_arrays(
		const PrimitiveType& _mode,
		const int& _offset,
		const uint32_t& _index_count
	) {
		glDrawArrays(static_cast<GLenum>(_mode), _offset, _index_count);
	}

	void Context::draw_elements(
		const PrimitiveType& _mode,
		const uint32_t& _index_count,
		const NumericType& _type,
		const intptr_t& _offset
	) {
		glDrawElements(static_cast<GLenum>(_mode), static_cast<GLsizei>(_index_count), static_cast<GLenum>(_type), reinterpret_cast<const GLvoid*>(_offset));
	}

	void ErrorQueue::check() {
		ErrorType _gl_err_type = static_cast<ErrorType>(Context::get_error());
		if (_gl_err_type != ErrorType::NoError) {
			m_queue.push(_gl_err_type);
		}
	}

	void ErrorQueue::pop() {
		m_queue.pop();
	}

	ErrorType& ErrorQueue::front() {
		return m_queue.front();
	}

	bool ErrorQueue::empty() {
		return m_queue.empty();
	}

	void ErrorQueue::flush() {
		while (empty() == false) {
			pop();
		}
	}

	void Window::callback_window_pos_changed(GLFWwindow* window, int xpos, int ypos) {
		glw::Window* _window = reinterpret_cast<glw::Window*>(glfwGetWindowUserPointer(window));
		glw::Event _event; {
			_event.type = glw::EventType::WindowMoved;
			_event.data.moved_window.xpos = xpos;
			_event.data.moved_window.ypos = ypos;
		}
		_window->m_event_queue.push(_event);
	}

	void Window::callback_window_size_changed(GLFWwindow* window, int width, int height) {
		glw::Window* _window = reinterpret_cast<glw::Window*>(glfwGetWindowUserPointer(window));
		glw::Event _event; {
			_event.type = glw::EventType::WindowResized;
			_event.data.resized_window.width = static_cast<uint32_t>(width);
			_event.data.resized_window.height = static_cast<uint32_t>(height);
		}
		_window->m_config.Width = width;
		_window->m_config.Height = height;
		_window->m_event_queue.push(_event);
	}

	void Window::callback_buffer_size_changed(GLFWwindow* window, int width, int height) {
		glw::Window* _window = reinterpret_cast<glw::Window*>(glfwGetWindowUserPointer(window));
		glw::Event _event; {
			_event.type = glw::EventType::FramebufferResized;
			_event.data.resized_framebuffer.width = static_cast<uint32_t>(width);
			_event.data.resized_framebuffer.height = static_cast<uint32_t>(height);
		}
		_window->m_event_queue.push(_event);
	}

	void Window::callback_key_detected(GLFWwindow* window, int key, int scancode, int action, int mods) {
		glw::Window* _window = reinterpret_cast<glw::Window*>(glfwGetWindowUserPointer(window));
		glw::Event _event; {
			switch (action) {
			case (GLFW_RELEASE): {
				_event.type = glw::EventType::KeyJustReleased;
				_event.data.just_released_key.key = static_cast<Key>(key);
				break;
			}
			case (GLFW_PRESS): {
				_event.type = glw::EventType::KeyJustPressed;
				_event.data.just_pressed_key.key = static_cast<Key>(key);
				break;
			}
			default: {
				return;
			}
			}
		}
		_window->m_event_queue.push(_event);
	}

	void Window::callback_mouse_scrolled(GLFWwindow* window, double xoffset, double yoffset) {
		glw::Window* _window = reinterpret_cast<glw::Window*>(glfwGetWindowUserPointer(window));
		glw::Event _event; {
			_event.type = EventType::MouseScrollDetected;
			_event.data.scrolled_mouse.amount_x = xoffset;
			_event.data.scrolled_mouse.amount_y = yoffset;
		}
		_window->m_event_queue.push(_event);
	}


	void Window::callback_mouse_moved(GLFWwindow* window, double xposIn, double yposIn) {
		static double _lastMousePosX = 0.0;
		static double _lastMousePosY = 0.0f;
		glw::Window* _window = reinterpret_cast<glw::Window*>(glfwGetWindowUserPointer(window));
		glw::Event _event; {
			_event.type = EventType::MousePositionChanged;
			_event.data.changed_mouse_position.x = xposIn;
			_event.data.changed_mouse_position.y = yposIn;
			_event.data.changed_mouse_position.relx = _event.data.changed_mouse_position.x - _lastMousePosX;
			_event.data.changed_mouse_position.rely = _lastMousePosY - _event.data.changed_mouse_position.y;
			_lastMousePosX = xposIn;
			_lastMousePosY = yposIn;
		}
		_window->m_event_queue.push(_event);
	}

	void Window::callback_mouse_button_detected(GLFWwindow* window, int button, int action, int mods) {
		glw::Window* _window = reinterpret_cast<glw::Window*>(glfwGetWindowUserPointer(window));
		glw::Event _event; {
			switch (action) {
			case (GLFW_PRESS): {
				_event.type = glw::EventType::MouseButtonDown;
				_event.data.downed_mouse_button.button_index = static_cast<Button>(button);
				break;
			}
			case (GLFW_RELEASE): {
				_event.type = glw::EventType::MouseButtonUp;
				_event.data.upped_mouse_button.button_index = static_cast<Button>(button);
				break;
			}
			}
		}
		_window->m_event_queue.push(_event);
	}

	void Window::callback_termination_requested(GLFWwindow* window) {
		glw::Window* _window = reinterpret_cast<glw::Window*>(glfwGetWindowUserPointer(window));
		glw::Event _event; {
			_event.type = EventType::TerminationRequested;
		}
		_window->m_event_queue.push(_event);
	}

	void Window::callback_char_inputted(GLFWwindow* window, unsigned int codepoint) {
		glw::Window* _window = reinterpret_cast<glw::Window*>(glfwGetWindowUserPointer(window));
		glw::Event _event; {
			_event.type = glw::EventType::CharInputted;
			_event.data.inputted_char.codepoint = codepoint;
		}
		_window->m_event_queue.push(_event);
	}

	void Window::callback_window_minimized(GLFWwindow* window, int value) {
		glw::Window* _window = reinterpret_cast<glw::Window*>(glfwGetWindowUserPointer(window));
		glw::Event _event; {
			_event.type = glw::EventType::WindowMinimized;
			_event.data.minimized_window.toggle = value;
		}
		_window->m_event_queue.push(_event);
	}

	void Window::callback_window_maximized(GLFWwindow* window, int value) {
		glw::Window* _window = reinterpret_cast<glw::Window*>(glfwGetWindowUserPointer(window));
		glw::Event _event; {
			_event.type = glw::EventType::WindowMaximized;
			_event.data.maximized_window.toggle = value;
		}
		_window->m_event_queue.push(_event);
	}

	Window::Window(const WindowConfig& _cfg)
		: m_config(_cfg),
		m_origin_width(_cfg.Width),
		m_origin_height(_cfg.Height)
	{
		int _monitorWidth = 0, _monitorHeight = 0;
		this->get_monitor_resolution(_monitorWidth, _monitorHeight);

		glfwWindowHint(GLFW_VISIBLE, m_config.Visible);
		glfwWindowHint(GLFW_RESIZABLE, m_config.Resizable);
		glfwWindowHint(GLFW_FOCUS_ON_SHOW, m_config.FocusOnShow);

		if (m_config.Centered == true) {
			glfwWindowHint(GLFW_POSITION_X, (_monitorWidth / 2) - (static_cast<int>(m_config.Width) / 2));
			glfwWindowHint(GLFW_POSITION_Y, (_monitorHeight / 2) - (static_cast<int>(m_config.Height) / 2));
		}
		else {
			glfwWindowHint(GLFW_POSITION_X, m_config.PosX);
			glfwWindowHint(GLFW_POSITION_Y, m_config.PosY);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_config.GLMajorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_config.GLMinorVersion);
		if (m_config.GLCoreProfileEnabled) {
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		}

		glfwWindowHint(GLFW_SAMPLES, m_config.MSAAFactor);

		m_window = glfwCreateWindow(m_config.Width, m_config.Height, m_config.Title.c_str(), nullptr, nullptr);
		if (m_window == nullptr) {
			_GLW_THROW_GLFW_ERROR;
		}

		if (m_config.CursorDisabled == true) {
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPos(m_window, static_cast<double>(_monitorWidth / 2), static_cast<double>(_monitorHeight / 2));
		}
		else {
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		if (m_config.RelativeCursorRequested == true && glfwRawMouseMotionSupported() == GLFW_TRUE) {
			glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}

		glfwSetWindowPosCallback(m_window, Window::callback_window_pos_changed);
		glfwSetWindowSizeCallback(m_window, Window::callback_window_size_changed);
		glfwSetFramebufferSizeCallback(m_window, Window::callback_buffer_size_changed);
		glfwSetKeyCallback(m_window, Window::callback_key_detected);
		glfwSetScrollCallback(m_window, Window::callback_mouse_scrolled);
		glfwSetCursorPosCallback(m_window, Window::callback_mouse_moved);
		glfwSetMouseButtonCallback(m_window, Window::callback_mouse_button_detected);
		glfwSetCharCallback(m_window, Window::callback_char_inputted);
		glfwSetWindowCloseCallback(m_window, Window::callback_termination_requested);
		glfwSetWindowIconifyCallback(m_window, Window::callback_window_minimized);
		glfwSetWindowMaximizeCallback(m_window, Window::callback_window_maximized);

		glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));

		m_cursor_arrow = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		m_cursor_ibeam = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		m_cursor_crosshair = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
		m_cursor_pointing_hand = glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR);
		m_cursor_resize_ew = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
		m_cursor_resize_ns = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
		m_cursor_resize_nwse = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
		m_cursor_resize_nesw = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
		m_cursor_resize_all = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
		m_cursor_not_allowed = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
	}

	Window::~Window() {
		glfwDestroyCursor(m_cursor_arrow);
		glfwDestroyCursor(m_cursor_ibeam);
		glfwDestroyCursor(m_cursor_crosshair);
		glfwDestroyCursor(m_cursor_pointing_hand);
		glfwDestroyCursor(m_cursor_resize_ew);
		glfwDestroyCursor(m_cursor_resize_ns);
		glfwDestroyCursor(m_cursor_resize_nwse);
		glfwDestroyCursor(m_cursor_resize_nesw);
		glfwDestroyCursor(m_cursor_resize_all);
		glfwDestroyCursor(m_cursor_not_allowed);

		glfwDestroyWindow(m_window);
		m_window = nullptr;
	}

	bool Window::pop_event(Event& _out_event) {
		if (m_event_queue.empty()) {
			return false;
		}
		_out_event = m_event_queue.front();
		m_event_queue.pop();
		return true;
	}

	void Window::set_icon(unsigned char* _data) {
		GLFWimage _images[1];
		_images[0].pixels = _data;
		glfwSetWindowIcon(m_window, 1, _images);
	}

	void Window::set_fullscreen_enabled(bool _value) {
		m_config.FullscreenEnabled = _value;

		auto _monitor = glfwGetPrimaryMonitor();
		auto _mode = glfwGetVideoMode(_monitor);
		if (_value) {
			glfwSetWindowMonitor(m_window, _monitor, 0, 0, _mode->width, _mode->height, _mode->refreshRate);
			m_config.Width = _mode->width;
			m_config.Height = _mode->height;
		}
		else {
			int originW = m_origin_width;
			int originH = m_origin_height;
			glfwSetWindowMonitor(m_window, nullptr, (_mode->width / 2) - (originW / 2), (_mode->height / 2) - (originH / 2), originW, originH, GLFW_DONT_CARE);
			m_config.Width = originW;
			m_config.Height = originH;
		}
	}

	void Window::set_cursor_disabled(bool _value) {
		m_config.CursorDisabled = _value;
		glfwSetInputMode(m_window, GLFW_CURSOR, _value);
	}

	void Window::set_title(const char* _value) {
		m_config.Title = _value;
		glfwSetWindowTitle(m_window, _value);
	}

	void Window::set_resizable(bool _value) {
		m_config.Resizable = _value;
		glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, _value);
	}

	void Window::set_visible(bool _value) {
		m_config.Visible = _value;
		if (_value) {
			glfwShowWindow(m_window);
		}
		else {
			glfwHideWindow(m_window);
		}
	}

	void Window::set_size(int _width, int _height) {
		glfwSetWindowSize(m_window, _width, _height);
		m_config.Width = _width;
		m_config.Height = _height;
	}

	void Window::set_width(int _value) {
		glfwSetWindowSize(m_window, _value, m_config.Height);
		m_config.Width = _value;
	}

	void Window::set_height(int _value) {
		glfwSetWindowSize(m_window, m_config.Width, _value);
		m_config.Height = _value;
	}

	void Window::set_position(int _xpos, int _ypos) {
		glfwSetWindowPos(m_window, _xpos, _ypos);
	}

	void Window::set_mouse_position(double _xpos, double _ypos) {
		glfwSetCursorPos(m_window, _xpos, _ypos);
	}

	void Window::set_cursor(const CursorType& _type) {
		switch (_type) {
		case (CursorType::Arrow): {
			glfwSetCursor(m_window, m_cursor_arrow);
			break;
		}
		case (CursorType::IBeam): {
			glfwSetCursor(m_window, m_cursor_ibeam);
			break;
		}
		case (CursorType::CrossHair): {
			glfwSetCursor(m_window, m_cursor_crosshair);
			break;
		}
		case (CursorType::PointingHand): {
			glfwSetCursor(m_window, m_cursor_pointing_hand);
			break;
		}
		case (CursorType::ResizeEW): {
			glfwSetCursor(m_window, m_cursor_resize_ew);
			break;
		}
		case (CursorType::ResizeNS): {
			glfwSetCursor(m_window, m_cursor_resize_ns);
			break;
		}
		case (CursorType::ResizeNWSE): {
			glfwSetCursor(m_window, m_cursor_resize_nwse);
			break;
		}
		case (CursorType::ResizeNESW): {
			glfwSetCursor(m_window, m_cursor_resize_nesw);
			break;
		}
		case (CursorType::ResizeAll): {
			glfwSetCursor(m_window, m_cursor_resize_all);
			break;
		}
		case (CursorType::NotAllowed): {
			glfwSetCursor(m_window, m_cursor_not_allowed);
			break;
		}
		}
	}

	GLFWwindow* Window::get() {
		return m_window;
	}

	bool Window::get_should_be_closed() {
		return glfwWindowShouldClose(m_window);
	}

	void Window::get_position(int& _out_xpos, int& _out_ypos) const {
		glfwGetWindowPos(m_window, &_out_xpos, &_out_ypos);
	}

	void Window::get_mouse_position(double& _out_xpos, double& _out_ypos) const {
		glfwGetCursorPos(m_window, &_out_xpos, &_out_ypos);
	}

	bool Window::get_key_is_held(const Key& _key) const {
		return (glfwGetKey(m_window, static_cast<int>(_key)) == GLFW_PRESS);
	}

	bool Window::get_button_is_held(const Button& _btn) const {
		return (glfwGetMouseButton(m_window, static_cast<int>(_btn)) == GLFW_PRESS);
	}

	void Window::get_monitor_resolution(int& _out_width, int& _out_height) const {
		const GLFWvidmode* _vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		_out_width = _vidmode->width;
		_out_height = _vidmode->height;
	}

	const bool& Window::get_visible() const {
		return m_config.Visible;
	}

	const int& Window::get_width() const {
		return m_config.Width;
	}

	const int& Window::get_height() const {
		return m_config.Height;
	}

	const int& Window::get_origin_width() const {
		return m_origin_width;
	}

	const int& Window::get_origin_height() const {
		return m_origin_height;
	}

	const bool& Window::get_fullscreen_enabled() const {
		return m_config.FullscreenEnabled;
	}

	const bool& Window::get_cursor_disabled() const {
		return m_config.CursorDisabled;
	}

	void BufferData::add_int32(const int& _data) {
		this->_add(reinterpret_cast<const unsigned char*>(&_data), sizeof(_data));
	}

	void BufferData::add_uint32(const uint32_t& _data) {
		this->_add(reinterpret_cast<const unsigned char*>(&_data), sizeof(_data));
	}

	void BufferData::add_float(const float& _data) {
		this->_add(reinterpret_cast<const unsigned char*>(&_data), sizeof(_data));
	}

	void BufferData::_add(const unsigned char* _data, size_t _count) {
		for (size_t i = 0; i < _count; i++) {
			m_data.emplace_back(_data[i]);
		}
	}

	const void* BufferData::get_pointer() const {
		return m_data.data();
	}

	size_t BufferData::get_length() const {
		return m_data.size();
	}

	Handle::Handle(const uint32_t& _id)
		: m_ID(_id)
	{

	}

	Handle::~Handle() {

	}

	void Handle::bind() const {

	}

	const uint32_t& Handle::id() const {
		return m_ID;
	}

	VertexArrayObject::VertexArrayObject()
		: Handle(glw::Context::generate_vertex_array())
	{

	}

	VertexArrayObject::~VertexArrayObject() {
		glw::Context::delete_vertex_array(m_ID);
	}

	void VertexArrayObject::bind() const {
		glw::Context::bind_vertex_array(m_ID);
	}

	void VertexArrayObject::attrib_pointer(const uint32_t& _offset_index, const uint32_t& _count, const NumericType& _type, const size_t& _stride, intptr_t _offset) const {
		glw::Context::enable_vertex_attrib_array(_offset_index);
		glw::Context::vertex_attrib_pointer(_offset_index, _count, _type, false, _stride, _offset);
	}

	void VertexArrayObject::bind_default() {
		glw::Context::bind_vertex_array(0U);
	}

	VertexBufferObject::VertexBufferObject()
		: Handle(glw::Context::generate_buffer())
	{

	}

	VertexBufferObject::~VertexBufferObject() {
		glw::Context::delete_buffer(m_ID);
	}

	void VertexBufferObject::bind() const {
		glw::Context::bind_array_buffer(glw::ArrayBufferType::VertexBuffer, m_ID);
	}

	void VertexBufferObject::bind_default() {
		glw::Context::bind_array_buffer(glw::ArrayBufferType::VertexBuffer, 0);
	}

	void VertexBufferObject::bind_data(const BufferData& _data, BufferUsage _usage) const {
		glw::Context::bind_array_buffer_data(glw::ArrayBufferType::VertexBuffer, _data, _usage);
	}

	void VertexBufferObject::bind_empty_data(const size_t& _length, BufferUsage _usage) const {
		glw::Context::bind_empty_array_buffer_data(glw::ArrayBufferType::VertexBuffer, _length, _usage);
	}

	void VertexBufferObject::bind_sub_data(const BufferData& _data, size_t _offset) const {
		glw::Context::bind_array_buffer_sub_data(glw::ArrayBufferType::VertexBuffer, _data, _offset);
	}

	ElementBufferObject::ElementBufferObject()
		: Handle(glw::Context::generate_buffer())
	{

	}

	ElementBufferObject::~ElementBufferObject() {
		glw::Context::delete_buffer(m_ID);
	}

	void ElementBufferObject::bind() const {
		glw::Context::bind_array_buffer(glw::ArrayBufferType::ElementBuffer, m_ID);
	}

	void ElementBufferObject::bind_default() {
		glw::Context::bind_array_buffer(glw::ArrayBufferType::ElementBuffer, 0);
	}

	void ElementBufferObject::bind_data(const BufferData& _data, BufferUsage _usage) const {
		glw::Context::bind_array_buffer_data(glw::ArrayBufferType::ElementBuffer, _data, _usage);
	}

	void ElementBufferObject::bind_empty_data(const size_t& _length, BufferUsage _usage) const {
		glw::Context::bind_empty_array_buffer_data(glw::ArrayBufferType::ElementBuffer, _length, _usage);
	}

	void ElementBufferObject::bind_sub_data(const BufferData& _data, size_t _offset) const {
		glw::Context::bind_array_buffer_sub_data(glw::ArrayBufferType::ElementBuffer, _data, _offset);
	}

	FrameBufferObject::FrameBufferObject()
		: Handle(glw::Context::generate_frame_buffer())
	{

	}

	FrameBufferObject::~FrameBufferObject() {
		glw::Context::delete_frame_buffer(m_ID);
	}

	void FrameBufferObject::bind() const {
		glw::Context::bind_frame_buffer(m_ID);
	}

	void FrameBufferObject::bind_default() {
		glw::Context::bind_frame_buffer(0);
	}

	void FrameBufferObject::attach_texture(AttachmentType _type, const Texture* _tex) {
		glw::Context::attach_texture_2d(m_ID, _type, _tex->id());
		m_texture_attachments[_type] = _tex;
	}

	void FrameBufferObject::attach_render_buffer(AttachmentType _type, const RenderBufferObject* _rbo) {
		glw::Context::attach_render_buffer(m_ID, _type, _rbo->id());
	}

	const Texture* FrameBufferObject::get_attached_texture(AttachmentType _type) const {
		if (m_texture_attachments.find(_type) != m_texture_attachments.end()) {
			return m_texture_attachments.at(_type);
		}
		return nullptr;
	}

	const char* FrameBufferObject::link_status_type_to_str(const FramebufferLinkStatusType& _type) {
		switch (_type) {
		case (FramebufferLinkStatusType::Complete): {
			return "COMPLETE";
			break;
		}
		case (FramebufferLinkStatusType::IncompleteAttachment): {
			return "INCOMPLETE_ATTACHMENT";
			break;
		}
		case (FramebufferLinkStatusType::IncompleteMissingAttachment): {
			return "INCOMPLETE_MISSING_ATTACHMENT";
			break;
		}
		case (FramebufferLinkStatusType::IncompleteReadBuffer): {
			return "INCOMPLETE_READ_BUFFER";
			break;
		}
		case (FramebufferLinkStatusType::IncompleteDrawBuffer): {
			return "INCOMPLETE_DRAW_BUFFER";
			break;
		}
		case (FramebufferLinkStatusType::IncompleteLayerTargets): {
			return "INCOMPLETE_LAYER_TARGETS";
			break;
		}
		case (FramebufferLinkStatusType::IncompleteMultisample): {
			return "INCOMPLETE_MULTISAMPLE";
			break;
		}
		case (FramebufferLinkStatusType::Undefined): {
			return "UNDEFINED";
			break;
		}
		case (FramebufferLinkStatusType::Unsupported): {
			return "UNSUPPORTED";
			break;
		}
		default: {
			break;
		}
		}
		return "UNKNOWN";
	}

	RenderBufferObject::RenderBufferObject()
		: Handle(glw::Context::generate_render_buffer())
	{

	}

	RenderBufferObject::~RenderBufferObject() {
		glw::Context::delete_render_buffer(m_ID);
	}

	void RenderBufferObject::bind() const {
		glw::Context::bind_render_buffer(m_ID);
	}

	void RenderBufferObject::bind_default() {
		glw::Context::bind_render_buffer(0);
	}

	void RenderBufferObject::set_storage(InternalFormat _format, uint32_t _width, uint32_t _height) {
		glw::Context::set_render_buffer_storage(_format, _width, _height);
	}

	Texture::Texture(const TextureConfig& _cfg, const uint32_t& _width, const uint32_t& _height, const uint32_t& _channels)
		: Handle(glw::Context::generate_texture()),
		m_config(_cfg),
		m_width(_width),
		m_height(_height),
		m_channels(_channels)
	{
		this->bind();
		glw::Context::set_filter(m_config.MagFilter, m_config.MinFilter);
		glw::Context::set_wrapping_mode(m_config.WrappingS, m_config.WrappingT);
		glw::Context::set_image(nullptr, m_width, m_height, m_config.FormatInternal, m_config.FormatTexture, m_config.FormatData);
		if (m_config.MipmapGeneration) {
			glw::Context::generate_mipmap();
		}
		Texture::bind_default();
	}

	Texture::Texture(const TextureConfig& _cfg, const unsigned char* _data, const uint32_t& _width, const uint32_t& _height, const uint32_t& _channels)
		: Handle(glw::Context::generate_texture()),
		m_config(_cfg),
		m_width(_width),
		m_height(_height),
		m_channels(_channels)
	{
		this->bind();
		glw::Context::set_filter(m_config.MagFilter, m_config.MinFilter);
		glw::Context::set_wrapping_mode(m_config.WrappingS, m_config.WrappingT);
		glw::Context::set_image(_data, m_width, m_height, m_config.FormatInternal, m_config.FormatTexture, m_config.FormatData);
		if (m_config.MipmapGeneration) {
			glw::Context::generate_mipmap();
		}
		Texture::bind_default();
	}

	Texture::~Texture() {
		glw::Context::delete_texture(m_ID);
	}

	void Texture::bind() const {
		glw::Context::bind_texture_2d(m_ID);
	}

	void Texture::bind_default() {
		glw::Context::bind_texture_2d(0);
	}

	const uint32_t& Texture::width() const {
		return m_width;
	}

	const uint32_t& Texture::height() const {
		return m_height;
	}

	const uint32_t& Texture::channels() const {
		return m_channels;
	}

	Shader::Shader(const ShaderType& _type, const char* _src)
		: m_ID(glw::Context::create_shader(_type)),
		m_type(_type)
	{
		glw::Context::set_source(m_ID, _src);
		glw::Context::compile_shader(_type, m_ID);
	}

	Shader::~Shader() {
		glw::Context::delete_shader(m_ID);
	}

	const uint32_t& Shader::id() const {
		return m_ID;
	}

	const ShaderType& Shader::type() const {
		return m_type;
	}

	const char* Shader::type_str() const {
		return Shader::type_to_str(m_type);
	}

	const char* Shader::type_to_str(const ShaderType& _type) {
		switch (_type) {
		case (ShaderType::Vertex): {
			return "VERTEX";
			break;
		}
		case (ShaderType::Geometry): {
			return "GEOMETRY";
			break;
		}
		case (ShaderType::Fragment): {
			return "FRAGMENT";
			break;
		}
		case (ShaderType::Compute): {
			return "COMPUTE";
			break;
		}
		case (ShaderType::TessellationControl): {
			return "TESS_CONTROL";
			break;
		}
		case (ShaderType::TessellationEvaluation): {
			return "TESS_EVALUATION";
			break;
		}
		default: {
			break;
		}
		}
		return "UNKNOWN";
	}

	Program::Program()
		: m_ID(glw::Context::create_program())
	{

	}

	Program::~Program() {
		glw::Context::delete_program(m_ID);
	}

	void Program::use() const {
		glw::Context::use_program(m_ID);
	}

	void Program::use_default() {
		glw::Context::use_program(0);
	}

	void Program::attach(const Shader* _shader) const {
		glw::Context::attach_shader(m_ID, _shader->id());
	}

	void Program::link() const {
		glw::Context::link_program(m_ID);
	}

	void Program::register_uniform(const std::string_view& _name) {
		m_uniform_locations[_name] = glw::Context::get_uniform_location(m_ID, _name.data());
	}

	void Program::set_sampler2D(const std::string_view& _name, int _slot_id) const {
		glw::Context::set_uniform_int(m_uniform_locations.at(_name), _slot_id);
	}

	void Program::set_int(const std::string_view& _name, int _value) const {
		glw::Context::set_uniform_int(m_uniform_locations.at(_name), _value);
	}

	void Program::set_uint(const std::string_view& _name, uint32_t _value) const {
		glw::Context::set_uniform_uint(m_uniform_locations.at(_name), _value);
	}

	void Program::set_float(const std::string_view& _name, float _value) const {
		glw::Context::set_uniform_float(m_uniform_locations.at(_name), _value);
	}

	void Program::set_vec2(const std::string_view& _name, float _x, float _y)const {
		glw::Context::set_uniform_vec2(m_uniform_locations.at(_name), _x, _y);
	}

	void Program::set_vec3(const std::string_view& _name, float _x, float _y, float _z) const {
		glw::Context::set_uniform_vec3(m_uniform_locations.at(_name), _x, _y, _z);
	}

	void Program::set_vec4(const std::string_view& _name, float _x, float _y, float _z, float _w) const {
		glw::Context::set_uniform_vec4(m_uniform_locations.at(_name), _x, _y, _z, _w);
	}

	void Program::set_mat2(const std::string_view& _name, const float* _ptr)  const {
		glw::Context::set_uniform_mat2(m_uniform_locations.at(_name), _ptr);
	}

	void Program::set_mat3(const std::string_view& _name, const float* _ptr) const {
		glw::Context::set_uniform_mat3(m_uniform_locations.at(_name), _ptr);
	}

	void Program::set_mat4(const std::string_view& _name, const float* _ptr) const {
		glw::Context::set_uniform_mat4(m_uniform_locations.at(_name), _ptr);
	}

	const uint32_t& Program::id() const {
		return m_ID;
	}

}