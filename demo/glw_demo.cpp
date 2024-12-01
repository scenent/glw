#include <glw.hpp>

static constexpr const char* g_simple_vert =
R"""(
#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 Color;

void main() {
	Color = aColor;
	gl_Position = vec4(aPos, 1.0);
}
)""";

static constexpr const char* g_simple_frag =
R"""(
#version 330

in vec3 Color;

out vec4 FragColor;

void main() {
	FragColor = vec4(Color, 1.0);
}
)""";

int main() {
	constexpr const uint32_t _window_width = 1024;
	constexpr const uint32_t _window_height = 720;

	glw::Context::initialize();
	glw::WindowConfig _windowCfg; {
		_windowCfg.Title = "glw demo";
		_windowCfg.Width = _window_width;
		_windowCfg.Height = _window_height;
	}
	glw::Window* _window = new glw::Window(_windowCfg);
	glw::Context::make_current(_window);
	glw::Context::activate();

	glw::Shader* _vertShader = new glw::Shader(glw::ShaderType::Vertex, g_simple_vert);
	glw::Shader* _fragShader = new glw::Shader(glw::ShaderType::Fragment, g_simple_frag);
	glw::Program* _program = new glw::Program();
	_program->attach(_vertShader);
	_program->attach(_fragShader);
	_program->link();

	glw::BufferData _vertices; {
		_vertices.add_float(-0.5f);
		_vertices.add_float(-0.5f);
		_vertices.add_float(0.0f);
		_vertices.add_float(1.0f);
		_vertices.add_float(0.0f);
		_vertices.add_float(0.0f);

		_vertices.add_float(0.5f);
		_vertices.add_float(-0.5f);
		_vertices.add_float(0.0f);
		_vertices.add_float(0.0f);
		_vertices.add_float(1.0f);
		_vertices.add_float(0.0f);

		_vertices.add_float(0.0f);
		_vertices.add_float(0.5f);
		_vertices.add_float(0.0f);
		_vertices.add_float(0.0f);
		_vertices.add_float(0.0f);
		_vertices.add_float(1.0f);
	}
	glw::BufferData _indices; {
		_indices.add_uint32(0U);
		_indices.add_uint32(1U);
		_indices.add_uint32(2U);
	}

	glw::VertexArrayObject* _VAO = new glw::VertexArrayObject();
	glw::VertexBufferObject* _VBO = new glw::VertexBufferObject();
	glw::ElementBufferObject* _EBO = new glw::ElementBufferObject();

	_VAO->bind();
	{
		_VBO->bind();
		_VBO->bind_data(_vertices, glw::BufferUsage::StaticDraw);
		_EBO->bind();
		_EBO->bind_data(_indices, glw::BufferUsage::StaticDraw);
		_VAO->attrib_pointer(0, 3, glw::NumericType::Float, sizeof(float) * 6, 0);
		_VAO->attrib_pointer(1, 3, glw::NumericType::Float, sizeof(float) * 6, sizeof(float) * 3);
	}
	glw::VertexArrayObject::bind_default();
	glw::VertexBufferObject::bind_default();
	glw::ElementBufferObject::bind_default();

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

		glw::Context::clear_buffer(glw::BufferType::Color | glw::BufferType::Depth);
		{
			_program->use();
			{
				_VAO->bind();
				glw::Context::draw_arrays(glw::PrimitiveType::Triangles, 0, 3);
				glw::VertexArrayObject::bind_default();
			}
			glw::Program::use_default();
		}
		glw::Context::swap_buffers(_window);
		glw::Context::poll_events();
	}

	delete _VAO, _VBO, _EBO;
	delete _vertShader, _fragShader;
	delete _program;
	delete _window;
	glw::Context::finalize();
}