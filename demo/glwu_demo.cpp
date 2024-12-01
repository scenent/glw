#include <glw.hpp>
#include <glwu.hpp>

int main() {
	constexpr const uint32_t _window_width = 1024;
	constexpr const uint32_t _window_height = 720;
	constexpr const float    _speed = 0.1f;
	constexpr const float    _angular_speed = 0.0006f;

	glw::Context::initialize();
	glw::WindowConfig _windowCfg; {
		_windowCfg.Title = "glwu demo";
		_windowCfg.Width = _window_width;
		_windowCfg.Height = _window_height;
	}
	glw::Window* _window = new glw::Window(_windowCfg);
	glw::Context::make_current(_window);
	glw::Context::activate();

	glwu::Server* _server = new glwu::Server(_window_width, _window_height);

	std::vector<glwu::Object*> _objects { };

	glwu::Camera2D* _cam = _server->fetch_object<glwu::Camera2D>();
	glwu::ColorRect* _color_rect = _server->fetch_object<glwu::ColorRect>();
	glwu::Line* _line = _server->fetch_object<glwu::Line>();
	glwu::LinedRect* _lined_rect = _server->fetch_object<glwu::LinedRect>();
	glwu::RoundedColorRect* _rounded_color_rect = _server->fetch_object<glwu::RoundedColorRect>();

	_cam->set_position({ _window_width / 2, _window_height / 2 });

	_color_rect->set_position({150.0f, 150.0f});
	_line->set_begin({ _window_width - 200.0f, 100.0f });
	_line->set_end({ _window_width - 100.0f, 200.0f });
	_lined_rect->set_position({ _window_width - 150.0f, _window_height - 150.0f});

	_rounded_color_rect->set_position({ 150.0f, _window_height - 150.0f });
	_rounded_color_rect->set_color({0.6f, 0.7f, 0.5f, 1.0f});
	_rounded_color_rect->set_outline_width(2.0f);
	_rounded_color_rect->set_outline_color({1.0f, 1.0f, 1.0f, 1.0f});

	_objects.emplace_back(_cam);
	_objects.emplace_back(_color_rect);
	_objects.emplace_back(_line);
	_objects.emplace_back(_lined_rect);
	_objects.emplace_back(_rounded_color_rect);

	glw::Context::clear_color(0.4f, 0.5f, 0.3f, 1.0f);

	glw::Event _event{};
	while (_window->get_should_be_closed() == false) {
		while (_window->pop_event(_event)) {
			switch (_event.type) {
			case (glw::EventType::WindowResized): {
				glw::Context::viewport(0, 0, _event.data.resized_window.width, _event.data.resized_window.height);
				break;
			}
			default: {
				break;
			}
			}
		}
		{
			if (_window->get_key_is_held(glw::Key::A)) {
				glm::vec3 _value = glm::rotateZ(glm::vec3(_speed, 0.0f, 0.0f), _cam->get_rotation());
				_cam->set_position(_cam->get_position() - glm::vec2(_value.x, _value.y));
			}
			if (_window->get_key_is_held(glw::Key::D)) {
				glm::vec3 _value = glm::rotateZ(glm::vec3(_speed, 0.0f, 0.0f), _cam->get_rotation());
				_cam->set_position(_cam->get_position() + glm::vec2(_value.x, _value.y));
			}
			if (_window->get_key_is_held(glw::Key::W)) {
				glm::vec3 _value = glm::rotateZ(glm::vec3(0.0f, _speed, 0.0f), _cam->get_rotation());
				_cam->set_position(_cam->get_position() - glm::vec2(_value.x, _value.y));
			}
			if (_window->get_key_is_held(glw::Key::S)) {
				glm::vec3 _value = glm::rotateZ(glm::vec3(0.0f, _speed, 0.0f), _cam->get_rotation());
				_cam->set_position(_cam->get_position() + glm::vec2(_value.x, _value.y));
			}
			if (_window->get_key_is_held(glw::Key::Q)) {
				_cam->set_rotation(_cam->get_rotation() - _angular_speed);
			}
			if (_window->get_key_is_held(glw::Key::E)) {
				_cam->set_rotation(_cam->get_rotation() + _angular_speed);
			}
			for (auto& _object : _objects) {
				_object->update();
			}
		}
		glw::Context::clear_buffer(glw::BufferType::Color | glw::BufferType::Depth);
		{
			for (const auto& _object : _objects) {
				_object->draw(_cam);
			}
		}
		glw::Context::swap_buffers(_window);
		glw::Context::poll_events();
	}

	delete _server;
	delete _window;
	glw::Context::finalize();
}