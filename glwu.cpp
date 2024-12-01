#define STB_IMAGE_IMPLEMENTATION
#include <glwu.hpp>

namespace glwu {

#pragma region vertex_shaders
	static constexpr const char* s_sprite_vert =
		R"""(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 uMVP;
uniform vec2 uSize;

out vec2 TexCoord;

void main() {
    vec4 pos = vec4(aPos, 1.0);
    pos.x *= uSize.x;
    pos.y *= uSize.y;
    gl_Position = (uMVP * pos);
    TexCoord = aTexCoord;
}
)""";
	static constexpr const char* s_line_vert =
		R"""(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uMVP;

void main() {
	vec4 pos = vec4(aPos, 1.0);
    gl_Position = uMVP * pos;
}
)""";
	static constexpr const char* s_rect_vert =
		R""""(
#version 330

layout (location = 0) in vec3 aPos;

uniform mat4 uMVP;
uniform vec2 uSize;

void main() {
	vec4 pos = vec4(aPos, 1.0);
	pos.x *= uSize.x;
	pos.y *= uSize.y;
	gl_Position = uMVP * pos;
}
)"""";
	static constexpr const char* s_lined_rect_vert =
		R""""(
#version 330

layout (location = 0) in vec3 aPos;

uniform mat4 uMVP;
uniform vec2 uSize;

void main() {
	vec4 pos = vec4(aPos, 1.0);
	pos.x = floor(pos.x * uSize.x) + 0.5;
	pos.y = floor(pos.y * uSize.y) + 0.5;
	gl_Position = uMVP * pos;
}
)"""";
#pragma endregion

#pragma region fragment_shaders
	static constexpr const char* s_sprite_frag =
		R"""(
#version 330 core

in vec2 TexCoord;

uniform sampler2D uTexture;
uniform vec4 uModulate;
out vec4 FragColor;

void main() {
    FragColor = texture(uTexture, TexCoord) * uModulate;
}
)""";
	static constexpr const char* s_blended_sprite_frag =
		R"""(
#version 330 core

#define MAX_TEXTURE_COUNT 4

in vec2 TexCoord;

uniform int uTextureCount;
uniform sampler2D uTextures[MAX_TEXTURE_COUNT];
uniform vec4 uModulate;
out vec4 FragColor;

void main() {
    vec4 _outColor = vec4(1, 1, 1, 0);
    for (int i = 0; i < min(MAX_TEXTURE_COUNT, uTextureCount); i++) {
        vec4 texColor = texture(uTextures[i], TexCoord);
        if (_outColor.a < 1.0) {
            _outColor = mix(_outColor, texColor, texColor.a);
        }
    }
    FragColor = _outColor * uModulate;
}
)""";
	static constexpr const char* s_simple_frag =
		R""""(
#version 330 core

uniform vec4 uColor;
uniform vec4 uModulate;

out vec4 FragColor;

void main() {
    FragColor = uColor * uModulate;
}
)"""";
	static constexpr const char* s_rounded_rect_frag =
		R""""(
#version 330 core

in vec2 TexCoord;

uniform vec4  uColor;
uniform vec4  uModulate;
uniform vec2  uSize;
uniform float uCornerRadius;
uniform float uOutlineWidth;
uniform vec4  uOutlineColor;

out vec4 FragColor;

void main() {
    vec2 normalizedCoord = TexCoord * uSize;
    float radius = uCornerRadius * min(uSize.x, uSize.y) * 0.5;
    vec2 dist = normalizedCoord - vec2(radius, radius);
    vec2 size = uSize - vec2(radius * 2.0, radius * 2.0);

    bool isInside = 
        (dist.x < size.x && dist.y < size.y) || 
            (normalizedCoord.x <= radius ||
             normalizedCoord.y <= radius || 
             normalizedCoord.x >= (uSize.x - radius) || 
             normalizedCoord.y >= (uSize.y - radius)
            );
    bool isOutline = (
            normalizedCoord.x >= (uSize.x - (1.0 + uOutlineWidth)) && (normalizedCoord.y >= radius && normalizedCoord.y <= uSize.y - radius) ||
            normalizedCoord.x <= (1.0 + uOutlineWidth) && (normalizedCoord.y >= radius && normalizedCoord.y <= uSize.y - radius) || 
            normalizedCoord.y >= (uSize.y - (1.0 + uOutlineWidth)) && (normalizedCoord.x >= radius && normalizedCoord.x <= uSize.x - radius) ||
            normalizedCoord.y <= (1.0 + uOutlineWidth) && (normalizedCoord.x >= radius && normalizedCoord.x <= uSize.x - radius)
        );


    if (isInside) {
        float least = max(1.0, uOutlineWidth);
        if (normalizedCoord.x < radius && normalizedCoord.y < radius) {
            if (normalizedCoord.x < (radius - uOutlineWidth) + least && normalizedCoord.y < (radius - uOutlineWidth) + least) {
                float distCorner = length(normalizedCoord - vec2(radius, radius));
                if (distCorner > (radius - uOutlineWidth - 1.0) && distCorner < radius) {
                    isOutline = true;
                }
                else {
                    isInside = bool(distCorner < radius);
                }
            }
            else {
                float distCorner = length(normalizedCoord - vec2(radius, radius));
                if (distCorner > radius) {
                    isInside = false;
                }
            }
        }
        if (normalizedCoord.x > uSize.x - radius && normalizedCoord.y < radius) {
            if (normalizedCoord.x > uSize.x - (radius - uOutlineWidth) - least && normalizedCoord.y < (radius - uOutlineWidth) + least) {
                float distCorner = length(normalizedCoord - vec2(uSize.x - radius, radius));
                if (distCorner > (radius - uOutlineWidth - 1.0) && distCorner < radius) {
                    isOutline = true;
                }
                else {
                    isInside = bool(distCorner < radius);
                }
            }
            else {
                float distCorner = length(normalizedCoord - vec2(uSize.x - radius, radius));
                if (distCorner > radius) {
                    isInside = false;
                }
            }
        }
        if (normalizedCoord.x > uSize.x - radius && normalizedCoord.y > uSize.y - radius) {
            if (normalizedCoord.x > uSize.x - (radius - uOutlineWidth) - least && normalizedCoord.y > uSize.y - (radius - uOutlineWidth) - least) {
                float distCorner = length(normalizedCoord - vec2(uSize.x - radius, uSize.y - radius));
                if (distCorner > (radius - uOutlineWidth - 1.0) && distCorner < radius) {
                    isOutline = true;
                }
                else {
                    isInside = bool(distCorner < radius);
                }
            }
            else {
                float distCorner = length(normalizedCoord - vec2(uSize.x - radius, uSize.y - radius));
                if (distCorner > radius) {
                    isInside = false;
                }
            }
        }
        if (normalizedCoord.x < radius && normalizedCoord.y > uSize.y - radius) {
            if (normalizedCoord.x < (radius - uOutlineWidth) + least && normalizedCoord.y > uSize.y - (radius - uOutlineWidth) - least) {
                float distCorner = length(normalizedCoord - vec2(radius, uSize.y - radius));
                if (distCorner > (radius - uOutlineWidth - 1.0) && distCorner < radius) {
                    isOutline = true;
                }
                else {
                    isInside = bool(distCorner < radius);
                }
            }
            else {
                float distCorner = length(normalizedCoord - vec2(radius, uSize.y - radius));
                if (distCorner > radius) {
                    isInside = false;
                }
            }
        }
        FragColor = isOutline ? uOutlineColor * uModulate : (isInside ? uColor * uModulate : vec4(0.0, 0.0, 0.0, 0.0));
    } else {
        FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
}
)"""";
#pragma endregion

	Server::Server(uint32_t _initial_window_width, uint32_t _initial_window_height)
		: m_initial_window_width(_initial_window_width),
		m_initial_window_height(_initial_window_height)
	{
		glw::BufferData _rect_vertices; {
			_rect_vertices.add_float(0.5f);
			_rect_vertices.add_float(0.5f);
			_rect_vertices.add_float(0.0f);

			_rect_vertices.add_float(0.5f);
			_rect_vertices.add_float(-0.5f);
			_rect_vertices.add_float(0.0f);

			_rect_vertices.add_float(-0.5f);
			_rect_vertices.add_float(-0.5f);
			_rect_vertices.add_float(0.0f);

			_rect_vertices.add_float(-0.5f);
			_rect_vertices.add_float(0.5f);
			_rect_vertices.add_float(0.0f);
		}
		glw::BufferData _rect_indices; {
			_rect_indices.add_uint32(0);
			_rect_indices.add_uint32(1);
			_rect_indices.add_uint32(3);

			_rect_indices.add_uint32(1);
			_rect_indices.add_uint32(2);
			_rect_indices.add_uint32(3);
		}

		glw::BufferData _linedrect_vertices; {
			_linedrect_vertices.add_float(-0.5f);
			_linedrect_vertices.add_float(0.5f);
			_linedrect_vertices.add_float(0.0f);

			_linedrect_vertices.add_float(0.5f);
			_linedrect_vertices.add_float(0.5f);
			_linedrect_vertices.add_float(0.0f);

			_linedrect_vertices.add_float(0.5f);
			_linedrect_vertices.add_float(-0.5f);
			_linedrect_vertices.add_float(0.0f);

			_linedrect_vertices.add_float(-0.5f);
			_linedrect_vertices.add_float(-0.5f);
			_linedrect_vertices.add_float(0.0f);
		}

		glw::BufferData _linedrect_indices; {
			_linedrect_indices.add_uint32(0);
			_linedrect_indices.add_uint32(1);

			_linedrect_indices.add_uint32(1);
			_linedrect_indices.add_uint32(2);

			_linedrect_indices.add_uint32(2);
			_linedrect_indices.add_uint32(3);

			_linedrect_indices.add_uint32(3);
			_linedrect_indices.add_uint32(0);
		}

		glw::BufferData _sprite_vertices; {
			_sprite_vertices.add_float(0.5f);
			_sprite_vertices.add_float(0.5f);
			_sprite_vertices.add_float(0.0f);
			_sprite_vertices.add_float(1.0f);
			_sprite_vertices.add_float(1.0f);

			_sprite_vertices.add_float(0.5f);
			_sprite_vertices.add_float(-0.5f);
			_sprite_vertices.add_float(0.0f);
			_sprite_vertices.add_float(1.0f);
			_sprite_vertices.add_float(0.0f);

			_sprite_vertices.add_float(-0.5f);
			_sprite_vertices.add_float(-0.5f);
			_sprite_vertices.add_float(0.0f);
			_sprite_vertices.add_float(0.0f);
			_sprite_vertices.add_float(0.0f);

			_sprite_vertices.add_float(-0.5f);
			_sprite_vertices.add_float(0.5f);
			_sprite_vertices.add_float(0.0f);
			_sprite_vertices.add_float(0.0f);
			_sprite_vertices.add_float(1.0f);
		}
		glw::BufferData _sprite_indices; {
			_sprite_indices.add_uint32(0);
			_sprite_indices.add_uint32(1);
			_sprite_indices.add_uint32(3);

			_sprite_indices.add_uint32(1);
			_sprite_indices.add_uint32(2);
			_sprite_indices.add_uint32(3);
		}

		glw::BufferData _line_indices; {
			_line_indices.add_uint32(0);
			_line_indices.add_uint32(1);
		}



		glw::VertexArrayObject* _rectVAO = new glw::VertexArrayObject();
		glw::VertexBufferObject* _rectVBO = new glw::VertexBufferObject();
		glw::ElementBufferObject* _rectEBO = new glw::ElementBufferObject();

		_rectVAO->bind();
		{
			_rectVBO->bind();
			_rectVBO->bind_data(_rect_vertices, glw::BufferUsage::StaticDraw);
			_rectEBO->bind();
			_rectEBO->bind_data(_rect_indices, glw::BufferUsage::StaticDraw);
			_rectVAO->attrib_pointer(0, 3, glw::NumericType::Float, sizeof(float) * 3, 0);
			_rectVAO->attrib_pointer(1, 2, glw::NumericType::Float, sizeof(float) * 3, sizeof(float) * 3);
		}
		glw::VertexArrayObject::bind_default();
		glw::VertexBufferObject::bind_default();
		glw::ElementBufferObject::bind_default();

		glw::VertexArrayObject* _linedrectVAO = new glw::VertexArrayObject();
		glw::VertexBufferObject* _linedrectVBO = new glw::VertexBufferObject();
		glw::ElementBufferObject* _linedrectEBO = new glw::ElementBufferObject();

		_linedrectVAO->bind();
		{
			_linedrectVBO->bind();
			_linedrectVBO->bind_data(_linedrect_vertices, glw::BufferUsage::StaticDraw);
			_linedrectEBO->bind();
			_linedrectEBO->bind_data(_linedrect_indices, glw::BufferUsage::StaticDraw);
			_linedrectVAO->attrib_pointer(0, 3, glw::NumericType::Float, sizeof(float) * 3, 0);
		}
		glw::VertexArrayObject::bind_default();
		glw::VertexBufferObject::bind_default();
		glw::ElementBufferObject::bind_default();

		glw::VertexArrayObject* _spriteVAO = new glw::VertexArrayObject();
		glw::VertexBufferObject* _spriteVBO = new glw::VertexBufferObject();
		glw::ElementBufferObject* _spriteEBO = new glw::ElementBufferObject();

		_spriteVAO->bind();
		{
			_spriteVBO->bind();
			_spriteVBO->bind_data(_sprite_vertices, glw::BufferUsage::StaticDraw);
			_spriteEBO->bind();
			_spriteEBO->bind_data(_sprite_indices, glw::BufferUsage::StaticDraw);
			_spriteVAO->attrib_pointer(0, 3, glw::NumericType::Float, sizeof(float) * 3 + sizeof(float) * 2, 0);
			_spriteVAO->attrib_pointer(1, 2, glw::NumericType::Float, sizeof(float) * 3 + sizeof(float) * 2, sizeof(float) * 3);
		}
		glw::VertexArrayObject::bind_default();
		glw::VertexBufferObject::bind_default();
		glw::ElementBufferObject::bind_default();

		glw::VertexArrayObject* _lineVAO = new glw::VertexArrayObject();
		glw::VertexBufferObject* _lineVBO = new glw::VertexBufferObject();
		glw::ElementBufferObject* _lineEBO = new glw::ElementBufferObject();

		_lineVAO->bind();
		{
			_lineVBO->bind();
			_lineVBO->bind_empty_data((sizeof(float) * 3) * 2, glw::BufferUsage::DynamicDraw);
			_lineEBO->bind();
			_lineEBO->bind_data(_line_indices, glw::BufferUsage::StaticDraw); // TODO
			//_lineEBO->bind_data(_line_indices, glw::BufferUsage::DynamicDraw); // TODO
			_lineVAO->attrib_pointer(0, 3, glw::NumericType::Float, sizeof(float) * 3, 0);
		}
		glw::VertexArrayObject::bind_default();
		glw::VertexBufferObject::bind_default();
		glw::ElementBufferObject::bind_default();

		GLWObjectSet _setSlot{};

		std::get<glw::VertexArrayObject*>(_setSlot) = _spriteVAO;
		std::get<glw::VertexBufferObject*>(_setSlot) = _spriteVBO;
		std::get<glw::ElementBufferObject*>(_setSlot) = _spriteEBO;

		m_glw_object_set_map[DrawableType::Sprite] = _setSlot;
		m_glw_object_set_map[DrawableType::BlendedSprite] = _setSlot;
		m_glw_object_set_map[DrawableType::RoundedColorRect] = _setSlot;

		std::get<glw::VertexArrayObject*>(_setSlot) = _lineVAO;
		std::get<glw::VertexBufferObject*>(_setSlot) = _lineVBO;
		std::get<glw::ElementBufferObject*>(_setSlot) = _lineEBO;

		m_glw_object_set_map[DrawableType::Line] = _setSlot;

		std::get<glw::VertexArrayObject*>(_setSlot) = _rectVAO;
		std::get<glw::VertexBufferObject*>(_setSlot) = _rectVBO;
		std::get<glw::ElementBufferObject*>(_setSlot) = _rectEBO;

		m_glw_object_set_map[DrawableType::ColorRect] = _setSlot;

		std::get<glw::VertexArrayObject*>(_setSlot) = _linedrectVAO;
		std::get<glw::VertexBufferObject*>(_setSlot) = _linedrectVBO;
		std::get<glw::ElementBufferObject*>(_setSlot) = _linedrectEBO;

		m_glw_object_set_map[DrawableType::LinedRect] = _setSlot;

		glw::Shader _sprite_vert{ glw::ShaderType::Vertex, s_sprite_vert };
		glw::Shader _line_vert{ glw::ShaderType::Vertex, s_line_vert };
		glw::Shader _rect_vert{ glw::ShaderType::Vertex, s_rect_vert };
		glw::Shader _lined_rect_vert{ glw::ShaderType::Vertex, s_lined_rect_vert };

		glw::Shader _sprite_frag{ glw::ShaderType::Fragment, s_sprite_frag };
		glw::Shader _blended_sprite_frag{ glw::ShaderType::Fragment, s_blended_sprite_frag };
		glw::Shader _simple_frag{ glw::ShaderType::Fragment, s_simple_frag };
		glw::Shader _rounded_rect_frag{ glw::ShaderType::Fragment, s_rounded_rect_frag };

		glw::Program* _sprite_program = new glw::Program(); {
			_sprite_program->attach(&_sprite_vert);
			_sprite_program->attach(&_sprite_frag);
			_sprite_program->link();
			_sprite_program->register_uniform("uMVP");
			_sprite_program->register_uniform("uSize");
			_sprite_program->register_uniform("uTexture");
			_sprite_program->register_uniform("uModulate");
		}
		glw::Program* _blended_sprite_program = new glw::Program(); {
			_blended_sprite_program->attach(&_sprite_vert);
			_blended_sprite_program->attach(&_blended_sprite_frag);
			_blended_sprite_program->link();
			_blended_sprite_program->register_uniform("uMVP");
			_blended_sprite_program->register_uniform("uSize");
			_blended_sprite_program->register_uniform("uTextureCount");
			_blended_sprite_program->register_uniform("uTextures[0]");
			_blended_sprite_program->register_uniform("uTextures[1]");
			_blended_sprite_program->register_uniform("uTextures[2]");
			_blended_sprite_program->register_uniform("uTextures[3]");
			_blended_sprite_program->register_uniform("uModulate");
		}
		glw::Program* _line_program = new glw::Program(); {
			_line_program->attach(&_line_vert);
			_line_program->attach(&_simple_frag);
			_line_program->link();
			_line_program->register_uniform("uMVP");
			_line_program->register_uniform("uColor");
			_line_program->register_uniform("uModulate");
		}
		glw::Program* _color_rect_program = new glw::Program(); {
			_color_rect_program->attach(&_rect_vert);
			_color_rect_program->attach(&_simple_frag);
			_color_rect_program->link();
			_color_rect_program->register_uniform("uMVP");
			_color_rect_program->register_uniform("uSize");
			_color_rect_program->register_uniform("uColor");
			_color_rect_program->register_uniform("uModulate");
		}
		glw::Program* _lined_rect_program = new glw::Program(); {
			_lined_rect_program->attach(&_lined_rect_vert);
			_lined_rect_program->attach(&_simple_frag);
			_lined_rect_program->link();
			_lined_rect_program->register_uniform("uMVP");
			_lined_rect_program->register_uniform("uSize");
			_lined_rect_program->register_uniform("uColor");
			_lined_rect_program->register_uniform("uModulate");
		}
		glw::Program* _rounded_color_rect_program = new glw::Program(); {
			_rounded_color_rect_program->attach(&_sprite_vert);
			_rounded_color_rect_program->attach(&_rounded_rect_frag);
			_rounded_color_rect_program->link();
			_rounded_color_rect_program->register_uniform("uMVP");
			_rounded_color_rect_program->register_uniform("uSize");
			_rounded_color_rect_program->register_uniform("uColor");
			_rounded_color_rect_program->register_uniform("uModulate");
			_rounded_color_rect_program->register_uniform("uCornerRadius");
			_rounded_color_rect_program->register_uniform("uOutlineWidth");
			_rounded_color_rect_program->register_uniform("uOutlineColor");
		}

		m_program_map[DrawableType::Sprite] = _sprite_program;
		m_program_map[DrawableType::BlendedSprite] = _blended_sprite_program;
		m_program_map[DrawableType::Line] = _line_program;
		m_program_map[DrawableType::ColorRect] = _color_rect_program;
		m_program_map[DrawableType::LinedRect] = _lined_rect_program;
		m_program_map[DrawableType::RoundedColorRect] = _rounded_color_rect_program;

		m_glw_object_sets.emplace_back(std::make_tuple(_spriteVAO, _spriteVBO, _spriteEBO));
		m_glw_object_sets.emplace_back(std::make_tuple(_lineVAO, _lineVBO, _lineEBO));
		m_glw_object_sets.emplace_back(std::make_tuple(_rectVAO, _rectVBO, _rectEBO));
		m_glw_object_sets.emplace_back(std::make_tuple(_linedrectVAO, _linedrectVBO, _linedrectEBO));

		m_programs.emplace_back(_sprite_program);
		m_programs.emplace_back(_blended_sprite_program);
		m_programs.emplace_back(_line_program);
		m_programs.emplace_back(_color_rect_program);
		m_programs.emplace_back(_lined_rect_program);
		m_programs.emplace_back(_rounded_color_rect_program);
	}

	Server::~Server() {
		for (auto& _object : m_objects) {
			delete _object;
			_object = nullptr;
		}
		m_objects.clear();

		for (auto& [_vao, _vbo, _ebo] : m_glw_object_sets) {
			delete _vao, _vbo, _ebo;
			_vao = nullptr;
			_vbo = nullptr;
			_ebo = nullptr;
		}
		m_glw_object_sets.clear();
		m_glw_object_set_map.clear();

		for (auto& _prog : m_programs) {
			delete _prog;
			_prog = nullptr;
		}
		m_programs.clear();
		m_program_map.clear();

		for (auto& _tex : m_textures) {
			delete _tex;
			_tex = nullptr;
		}
		m_textures.clear();
		m_texture_map.clear();
	}

	const glw::Texture* Server::fetch_texture(const std::string& _path, bool _auto_format, glw::TextureConfig _cfg) {
		TextureKey _key = std::make_tuple(
			_path,
			_cfg.MagFilter,
			_cfg.MinFilter,
			_cfg.WrappingS,
			_cfg.WrappingT,
			_cfg.FormatInternal,
			_cfg.FormatTexture,
			_cfg.FormatData,
			_cfg.MipmapGeneration,
			_cfg.FilpY
		);
		if (m_texture_map.find(_key) != m_texture_map.end()) {
			return m_texture_map.at(_key);
		}
		stbi_set_flip_vertically_on_load(_cfg.FilpY);
		int _width = 0, _height = 0, _channels = 0;
		unsigned char* _data = stbi_load(_path.c_str(), &_width, &_height, &_channels, 0);
		if (_data == nullptr) {
			throw exception::STBILoadingFailure(_path);
		}
		if (_auto_format) {
			glw::Context::enable(glw::CapabilityType::Blend);
			glw::Context::blend_func(glw::BlendFuncType::SrcAlpha, glw::BlendFuncType::OneMinusSrcAlpha);

			switch (_channels) {
			case (4): {
				_cfg.FormatInternal = glw::InternalFormat::RGBA;
				_cfg.FormatTexture = glw::TextureFormat::RGBA;
				break;
			}
			case (3): {
				_cfg.FormatInternal = glw::InternalFormat::RGB;
				_cfg.FormatTexture = glw::TextureFormat::RGB;
				break;
			}
			case (1): {
				_cfg.FormatInternal = glw::InternalFormat::Red;
				_cfg.FormatTexture = glw::TextureFormat::Red;
				break;
			}
			default: {
				break;
			}
			}
		}
		glw::Texture* _tex = new glw::Texture(_cfg, _data, _width, _height, _channels);
		stbi_image_free(_data);

		m_texture_map[_key] = _tex;
		m_textures.emplace_back(_tex);

		return _tex;
	}

	const uint32_t& Server::get_initial_window_width() const {
		return m_initial_window_width;
	}

	const uint32_t& Server::get_initial_window_height() const {
		return m_initial_window_height;
	}

	const Server::GLWObjectSet& Server::get_glw_object_set(const DrawableType& _type) const {
		return m_glw_object_set_map.at(_type);
	}

	const glw::Program* Server::get_generic_program(const DrawableType& _type) const {
		return m_program_map.at(_type);
	}

	Object::Object(Server* _owner)
		: m_owner(_owner)
	{
		this->_update_model_matrix();
	}

	Object::~Object() {

	}

	void Object::update() {
		if (m_need_to_update_model_matrix) {
			m_need_to_update_model_matrix = false;
			this->_update_model_matrix();
		}
	}

	void Object::draw(Camera2D* _cam) {

	}

	void Object::set_position(const glm::vec2& _value) {
		m_position = _value;
		m_need_to_update_model_matrix = true;
	}

	void Object::set_rotation(const float& _value) {
		m_rotation = _value;
		m_need_to_update_model_matrix = true;
	}

	void Object::set_scale(const glm::vec2& _value) {
		m_scale = _value;
		m_need_to_update_model_matrix = true;
	}

	void Object::set_modulate(const glm::vec4& _value) {
		m_modulate = _value;
	}

	Server* Object::fetch_owner() {
		return m_owner;
	}

	const Server* Object::get_owner() const {
		return m_owner;
	}

	const glm::vec2& Object::get_position() const {
		return m_position;
	}

	const float& Object::get_rotation() const {
		return m_rotation;
	}

	const glm::vec2& Object::get_scale() const {
		return m_scale;
	}

	const glm::vec4& Object::get_modulate() const {
		return m_modulate;
	}

	const glm::mat4& Object::get_model_matrix() const {
		return m_model_matrix;
	}

	void Object::_update_model_matrix() {
		m_model_matrix = glm::mat4(1.0f);
		m_model_matrix = glm::translate(m_model_matrix, { m_position.x, -m_position.y, 0.0f });
		m_model_matrix *= glm::eulerAngleZ(m_rotation);
		m_model_matrix = glm::scale(m_model_matrix, { m_scale.x, m_scale.y, 1.0f });
	}

	Camera2D::Camera2D(Server* _owner)
		: Object(_owner)
	{
		this->_update_view_proj_matrix();
	}

	Camera2D::~Camera2D() {

	}

	void Camera2D::update() {
		if (m_need_to_view_proj_matrix) {
			m_need_to_view_proj_matrix = false;
			this->_update_view_proj_matrix();
		}
		Object::update();
	}

	void Camera2D::draw(Camera2D* _cam) {
		Object::draw(_cam);
	}

	void Camera2D::set_position(const glm::vec2& _value) {
		Object::set_position(_value);
		m_need_to_view_proj_matrix = true;
	}

	void Camera2D::set_rotation(const float& _value) {
		Object::set_rotation(_value);
		m_need_to_view_proj_matrix = true;
	}

	void Camera2D::set_scale(const glm::vec2& _value) {
		Object::set_scale(_value);
		m_need_to_view_proj_matrix = true;
	}

	void Camera2D::set_zoom(const glm::vec2& _value) {
		m_zoom = _value;
		m_need_to_view_proj_matrix = true;
	}

	const glm::vec2& Camera2D::get_zoom()const {
		return m_zoom;
	}

	const glm::mat4& Camera2D::get_view_proj_matrix() const {
		return m_view_proj_matrix;
	}

	const glm::mat4& Camera2D::get_default_view_proj_matrix(const uint32_t& _width, const uint32_t& _height) {
		static glm::mat4 _default_view_proj_matrix = {
			2.0f / 1280.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / 720.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f / (1000.0f - 0.1f), (0.1f) / (0.1f - 1000.0f),
			0.0f, 0.0f, 0.0f, 1.0f
		};
		_default_view_proj_matrix[0][0] = 2.0f / _width;
		_default_view_proj_matrix[1][1] = 2.0f / _height;
		return _default_view_proj_matrix;
	}

	void Camera2D::_update_view_proj_matrix() {
		m_view_proj_matrix = {
			2.0f / get_owner()->get_initial_window_width(), 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / get_owner()->get_initial_window_height(), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f / (1000.0f - 0.1f), (0.1f) / (0.1f - 1000.0f),
			0.0f, 0.0f, 0.0f, 1.0f
		};
		m_view_proj_matrix = glm::scale(m_view_proj_matrix, glm::vec3(m_zoom.x, m_zoom.y, 1.0));
		m_view_proj_matrix = glm::rotate(m_view_proj_matrix, this->get_rotation(), glm::vec3(0, 0, 1));
		m_view_proj_matrix = glm::translate(m_view_proj_matrix, glm::vec3(-this->get_position().x, this->get_position().y, 0.0f));
	}

	Sprite::Sprite(Server* _owner)
		: Object(_owner)
	{
		std::tie(m_VAO, m_VBO, m_EBO) = get_owner()->get_glw_object_set(glwu::DrawableType::Sprite);
		m_program = get_owner()->get_generic_program(glwu::DrawableType::Sprite);
	}

	Sprite::~Sprite() {

	}

	void Sprite::update() {
		Object::update();
	}

	void Sprite::draw(Camera2D* _cam) {
		if (m_texture == nullptr) { return; }

		const glm::mat4& _view_proj_matrix = (_cam != nullptr ? _cam->get_view_proj_matrix() : Camera2D::get_default_view_proj_matrix(get_owner()->get_initial_window_width(), get_owner()->get_initial_window_height()));
		const glm::mat4& _model_matrix = get_model_matrix();

		glw::Context::enable(glw::CapabilityType::Blend);
		glw::Context::blend_func(glw::BlendFuncType::SrcAlpha, glw::BlendFuncType::OneMinusSrcAlpha);
		glw::Context::disable(glw::CapabilityType::CullFace);
		glw::Context::disable(glw::CapabilityType::DepthTest);

		m_program->use();
		{
			m_program->set_mat4("uMVP", glm::value_ptr(_view_proj_matrix * _model_matrix));
			m_program->set_vec2("uSize", static_cast<float>(m_texture->width()), static_cast<float>(m_texture->height()));
			m_program->set_vec4("uModulate", get_modulate().r, get_modulate().g, get_modulate().b, get_modulate().a);

			glw::Context::activate_texture(glw::TextureSlot::S0);
			m_texture->bind();
			m_program->set_sampler2D("uTexture", 0);
			{
				m_VAO->bind();
				glw::Context::draw_elements(glw::PrimitiveType::Triangles, 6, glw::NumericType::UnsignedInt, 0);
				glw::VertexArrayObject::bind_default();
			}
			glw::Texture::bind_default();
		}
		glw::Program::use_default();

		glw::Context::enable(glw::CapabilityType::CullFace);
		glw::Context::enable(glw::CapabilityType::DepthTest);
		glw::Context::disable(glw::CapabilityType::Blend);
		Object::draw(_cam);
	}

	void Sprite::load(const std::string& _path, bool _auto_format, glw::TextureConfig _cfg) {
		m_texture = fetch_owner()->fetch_texture(_path, _auto_format, _cfg);
	}

	BlendedSprite::BlendedSprite(Server* _owner)
		: Object(_owner)
	{
		std::tie(m_VAO, m_VBO, m_EBO) = get_owner()->get_glw_object_set(glwu::DrawableType::BlendedSprite);
		m_program = get_owner()->get_generic_program(glwu::DrawableType::BlendedSprite);
	}

	BlendedSprite::~BlendedSprite() {

	}

	void BlendedSprite::update() {
		Object::update();
	}

	void BlendedSprite::draw(Camera2D* _cam) {
		if (m_textures.empty()) { return; }
		const glm::mat4& _view_proj_matrix = (_cam != nullptr ? _cam->get_view_proj_matrix() : Camera2D::get_default_view_proj_matrix(get_owner()->get_initial_window_width(), get_owner()->get_initial_window_height()));
		const glm::mat4& _model_matrix = get_model_matrix();

		glw::Context::enable(glw::CapabilityType::Blend);
		glw::Context::blend_func(glw::BlendFuncType::SrcAlpha, glw::BlendFuncType::OneMinusSrcAlpha);
		glw::Context::disable(glw::CapabilityType::CullFace);
		glw::Context::disable(glw::CapabilityType::DepthTest);

		m_program->use();
		{
			m_program->set_mat4("uMVP", glm::value_ptr(_view_proj_matrix * _model_matrix));
			m_program->set_vec2("uSize", static_cast<float>(m_base_width), static_cast<float>(m_base_height));
			m_program->set_vec4("uModulate", get_modulate().r, get_modulate().g, get_modulate().b, get_modulate().a);

			const size_t _texs_len = m_textures.size();
			m_program->set_int("uTextureCount", static_cast<int>(_texs_len));

			for (size_t i = 0; i < _texs_len; i++) {
				glw::TextureSlot _slot = static_cast<glw::TextureSlot>(static_cast<uint32_t>(glw::TextureSlot::S0) + static_cast<uint32_t>(i));
				glw::Context::activate_texture(_slot);
				m_textures[i]->bind();
				m_program->set_sampler2D("uTextures[" + std::to_string(i) + "]", static_cast<int>(i));
			}

			{
				m_VAO->bind();
				glw::Context::draw_elements(glw::PrimitiveType::Triangles, 6, glw::NumericType::UnsignedInt, 0);
				glw::VertexArrayObject::bind_default();
			}
			glw::Texture::bind_default();
		}
		glw::Program::use_default();

		glw::Context::enable(glw::CapabilityType::CullFace);
		glw::Context::enable(glw::CapabilityType::DepthTest);
		glw::Context::disable(glw::CapabilityType::Blend);
		Object::draw(_cam);
	}

	void BlendedSprite::set_base_size(const uint32_t& _width, const uint32_t& _height) {
		m_base_width = _width;
		m_base_height = _height;
	}

	void BlendedSprite::clear_textures() {
		m_textures.clear();
	}

	void BlendedSprite::load_extra_texture(const std::string& _path, bool _auto_format, glw::TextureConfig _cfg) {
		if (m_textures.size() < BlendedSprite::maximum_blended_texture_count) {
			m_textures.emplace_back(fetch_owner()->fetch_texture(_path, _auto_format, _cfg));
		}
	}

	size_t BlendedSprite::get_loaded_texture_count() const {
		return m_textures.size();
	}

	ColoredObject::ColoredObject(Server* _owner)
		: Object(_owner)
	{

	}

	ColoredObject::~ColoredObject() {

	}

	void ColoredObject::update() {
		Object::update();
	}

	void ColoredObject::draw(Camera2D* _cam) {
		Object::draw(_cam);
	}

	void ColoredObject::set_color(const glm::vec4& _value) {
		m_color = _value;
	}

	const glm::vec4& ColoredObject::get_color() const {
		return m_color;
	}

	Line::Line(Server* _owner)
		: ColoredObject(_owner)
	{
		std::tie(m_VAO, m_VBO, m_EBO) = get_owner()->get_glw_object_set(glwu::DrawableType::Line);
		m_program = get_owner()->get_generic_program(glwu::DrawableType::Line);
	}

	Line::~Line() {

	}

	void Line::update() {
		ColoredObject::update();
	}

	void Line::draw(Camera2D* _cam) {
		const glm::mat4& _view_proj_matrix = (_cam != nullptr ? _cam->get_view_proj_matrix() : Camera2D::get_default_view_proj_matrix(get_owner()->get_initial_window_width(), get_owner()->get_initial_window_height()));
		const glm::mat4& _model_matrix = get_model_matrix();

		glw::Context::enable(glw::CapabilityType::Blend);
		glw::Context::blend_func(glw::BlendFuncType::SrcAlpha, glw::BlendFuncType::OneMinusSrcAlpha);
		glw::Context::disable(glw::CapabilityType::CullFace);
		glw::Context::disable(glw::CapabilityType::DepthTest);

		m_program->use();
		{
			m_program->set_mat4("uMVP", glm::value_ptr(_view_proj_matrix * _model_matrix));
			m_program->set_vec4("uColor", get_color().r, get_color().g, get_color().b, get_color().a);
			m_program->set_vec4("uModulate", get_modulate().r, get_modulate().g, get_modulate().b, get_modulate().a);

			glw::Context::set_line_width(m_width);

			glw::BufferData _vertices; {
				_vertices.add_float(m_point_begin.x);
				_vertices.add_float(-m_point_begin.y);
				_vertices.add_float(0.0f);

				_vertices.add_float(m_point_end.x);
				_vertices.add_float(-m_point_end.y);
				_vertices.add_float(0.0f);
			}

			m_VAO->bind();
			{
				m_VBO->bind();
				m_VBO->bind_sub_data(_vertices, 0);
				glw::Context::draw_arrays(glw::PrimitiveType::Lines, 0, 2);
			}
			glw::VertexArrayObject::bind_default();
			glw::VertexBufferObject::bind_default();
		}
		glw::Program::use_default();

		glw::Context::enable(glw::CapabilityType::CullFace);
		glw::Context::enable(glw::CapabilityType::DepthTest);
		glw::Context::disable(glw::CapabilityType::Blend);
		ColoredObject::draw(_cam);
	}

	void Line::set_width(const float& _value) {
		m_width = _value;
	}

	void Line::set_begin(const glm::vec2& _value) {
		m_point_begin = _value;
	}

	void Line::set_end(const glm::vec2& _value) {
		m_point_end = _value;
	}

	const float& Line::get_width() const {
		return m_width;
	}

	const glm::vec2& Line::get_begin() const {
		return m_point_begin;
	}

	const glm::vec2& Line::get_end() const {
		return m_point_end;
	}

	LinedRect::LinedRect(Server* _owner) 
		: ColoredObject(_owner)
	{
		std::tie(m_VAO, m_VBO, m_EBO) = get_owner()->get_glw_object_set(glwu::DrawableType::LinedRect);
		m_program = get_owner()->get_generic_program(glwu::DrawableType::LinedRect);
	}

	LinedRect::~LinedRect() {

	}

	void LinedRect::update() {
		ColoredObject::update();
	}

	void LinedRect::draw(Camera2D* _cam) {
		const glm::mat4& _view_proj_matrix = (_cam != nullptr ? _cam->get_view_proj_matrix() : Camera2D::get_default_view_proj_matrix(get_owner()->get_initial_window_width(), get_owner()->get_initial_window_height()));
		const glm::mat4& _model_matrix = get_model_matrix();

		glw::Context::enable(glw::CapabilityType::Blend);
		glw::Context::blend_func(glw::BlendFuncType::SrcAlpha, glw::BlendFuncType::OneMinusSrcAlpha);
		glw::Context::disable(glw::CapabilityType::CullFace);
		glw::Context::disable(glw::CapabilityType::DepthTest);

		m_program->use();
		{
			m_program->set_mat4("uMVP", glm::value_ptr(_view_proj_matrix * _model_matrix));
			m_program->set_vec4("uColor", get_color().r, get_color().g, get_color().b, get_color().a);
			m_program->set_vec4("uModulate", get_modulate().r, get_modulate().g, get_modulate().b, get_modulate().a);
			m_program->set_vec2("uSize", m_size.x, m_size.y);

			glw::Context::set_line_width(m_line_width);

			m_VAO->bind();
			{
				glw::Context::draw_arrays(glw::PrimitiveType::LineLoop, 0, 4);
			}
			glw::VertexArrayObject::bind_default();
		}
		glw::Program::use_default();

		glw::Context::enable(glw::CapabilityType::CullFace);
		glw::Context::enable(glw::CapabilityType::DepthTest);
		glw::Context::disable(glw::CapabilityType::Blend);
		ColoredObject::draw(_cam);
	}

	void LinedRect::set_line_width(const float& _value) {
		m_line_width = _value;
	}

	void LinedRect::set_size(const glm::vec2& _value) {
		m_size = _value;
	}

	const float& LinedRect::get_line_width() const {
		return m_line_width;
	}

	const glm::vec2& LinedRect::get_size() const {
		return m_size;
	}

	ColorRect::ColorRect(Server* _owner)
		: ColoredObject(_owner)
	{
		std::tie(m_VAO, m_VBO, m_EBO) = get_owner()->get_glw_object_set(glwu::DrawableType::ColorRect);
		m_program = get_owner()->get_generic_program(glwu::DrawableType::ColorRect);
	}

	ColorRect::~ColorRect() {

	}

	void ColorRect::update() {
		ColoredObject::update();
	}

	void ColorRect::draw(Camera2D* _cam) {
		const glm::mat4& _view_proj_matrix = (_cam != nullptr ? _cam->get_view_proj_matrix() : Camera2D::get_default_view_proj_matrix(get_owner()->get_initial_window_width(), get_owner()->get_initial_window_height()));
		const glm::mat4& _model_matrix = get_model_matrix();

		glw::Context::enable(glw::CapabilityType::Blend);
		glw::Context::blend_func(glw::BlendFuncType::SrcAlpha, glw::BlendFuncType::OneMinusSrcAlpha);
		glw::Context::disable(glw::CapabilityType::CullFace);
		glw::Context::disable(glw::CapabilityType::DepthTest);

		m_program->use();
		{
			m_program->set_mat4("uMVP", glm::value_ptr(_view_proj_matrix * _model_matrix));
			m_program->set_vec4("uColor", get_color().r, get_color().g, get_color().b, get_color().a);
			m_program->set_vec4("uModulate", get_modulate().r, get_modulate().g, get_modulate().b, get_modulate().a);
			m_program->set_vec2("uSize", m_size.x, m_size.y);

			m_VAO->bind();
			{
				glw::Context::draw_elements(glw::PrimitiveType::Triangles, 6, glw::NumericType::UnsignedInt, 0);
			}
			glw::VertexArrayObject::bind_default();
		}
		glw::Program::use_default();

		glw::Context::enable(glw::CapabilityType::CullFace);
		glw::Context::enable(glw::CapabilityType::DepthTest);
		glw::Context::disable(glw::CapabilityType::Blend);
		ColoredObject::draw(_cam);
	}

	void ColorRect::set_size(const glm::vec2& _value){
		m_size = _value;
	}

	const glm::vec2& ColorRect::get_size() const {
		return m_size;
	}

	RoundedColorRect::RoundedColorRect(Server* _owner) 
		: ColoredObject(_owner)
	{
		std::tie(m_VAO, m_VBO, m_EBO) = get_owner()->get_glw_object_set(glwu::DrawableType::RoundedColorRect);
		m_program = get_owner()->get_generic_program(glwu::DrawableType::RoundedColorRect);
	}

	RoundedColorRect::~RoundedColorRect() {

	}
	
	void RoundedColorRect::update() {
		ColoredObject::update();
	}
	
	void RoundedColorRect::draw(Camera2D* _cam) {
		const glm::mat4& _view_proj_matrix = (_cam != nullptr ? _cam->get_view_proj_matrix() : Camera2D::get_default_view_proj_matrix(get_owner()->get_initial_window_width(), get_owner()->get_initial_window_height()));
		const glm::mat4& _model_matrix = get_model_matrix();

		glw::Context::enable(glw::CapabilityType::Blend);
		glw::Context::blend_func(glw::BlendFuncType::SrcAlpha, glw::BlendFuncType::OneMinusSrcAlpha);
		glw::Context::disable(glw::CapabilityType::CullFace);
		glw::Context::disable(glw::CapabilityType::DepthTest);

		m_program->use();
		{
			m_program->set_mat4("uMVP", glm::value_ptr(_view_proj_matrix * _model_matrix));
			m_program->set_vec4("uColor", get_color().r, get_color().g, get_color().b, get_color().a);
			m_program->set_vec4("uModulate", get_modulate().r, get_modulate().g, get_modulate().b, get_modulate().a);
			m_program->set_vec2("uSize", m_size.x, m_size.y);
			m_program->set_float("uCornerRadius", m_corner_radius);
			m_program->set_float("uOutlineWidth", m_outline_width);
			m_program->set_vec4("uOutlineColor", m_outline_color.r, m_outline_color.g, m_outline_color.b, m_outline_color.a);

			m_VAO->bind();
			{
				glw::Context::draw_elements(glw::PrimitiveType::Triangles, 6, glw::NumericType::UnsignedInt, 0);
			}
			glw::VertexArrayObject::bind_default();
		}
		glw::Program::use_default();

		glw::Context::enable(glw::CapabilityType::CullFace);
		glw::Context::enable(glw::CapabilityType::DepthTest);
		glw::Context::disable(glw::CapabilityType::Blend);
		ColoredObject::draw(_cam);
	}

	void RoundedColorRect::set_size(const glm::vec2& _value) {
		m_size = _value;
	}

	void RoundedColorRect::set_outline_width(const float& _value) {
		m_outline_width = _value;
	}

	void RoundedColorRect::set_outline_color(const glm::vec4& _value) {
		m_outline_color = _value;
	}

	void RoundedColorRect::set_corner_radius(const float& _value) {
		m_corner_radius = _value;
	}

	const glm::vec2& RoundedColorRect::get_size() const{
		return m_size;
	}

	const float& RoundedColorRect::get_outline_width() const {
		return m_outline_width;
	}

	const glm::vec4& RoundedColorRect::get_outline_color() const {
		return m_outline_color;
	}

	const float& RoundedColorRect::get_corner_radius() const {
		return m_corner_radius;
	}
}