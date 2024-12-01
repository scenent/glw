#ifndef __GLWU_HPP__
#define __GLWU_HPP__

/*
	glwu

	Utility extension for glw(https://github.com/scenent/glw.hpp/).

	Produced by scenent(https://github.com/scenent/).

	Distributed under MIT License.

	Features
		Buffer object & Program Caching
		Object interface
			Sprite
			BlendedSprite
			Camera
			Line
			LinedRect
			ColorRect
			RoundedColorRect
	Dependencies
		glw
			C++ 17+
			GLFW 3.4
			GLAD (which supports GL 4.6)
		stb_image
		glm
*/

/* STL */
#include <stdexcept> // std::runtime_error
#include <vector>    // std::vector
#include <tuple>     // std::tuple
/* glw */
#include <glw.hpp>
/* stb_image */
#include <stb_image.h>
/* glm */
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace glwu {

	namespace exception {
		class STBILoadingFailure : public std::runtime_error {
		public:
			STBILoadingFailure(const std::string& _path)
				: std::runtime_error("[STBI] : Failed to load image : Path = " + _path) { }
		};
	}

	enum class DrawableType {
		Sprite,
		BlendedSprite,
		Line,
		LinedRect,
		ColorRect,
		RoundedColorRect
	};

	class Server;
	class Object;
		class Camera2D;
		class Sprite;
		class BlendedSprite;
		class ColoredObject;
			class Line;
			class LinedRect;
			class ColorRect;
			class RoundedColorRect;

	class Server final {
		using GLWObjectSet = std::tuple<glw::VertexArrayObject*, glw::VertexBufferObject*, glw::ElementBufferObject*>;
		using TextureKey =
			std::tuple <
				std::string,
				glw::FilterType,
				glw::FilterType,
				glw::WrappingType,
				glw::WrappingType,
				glw::InternalFormat,
				glw::TextureFormat,
				glw::DataType,
				bool,
				bool
			>;
	private:
		std::vector<GLWObjectSet> m_glw_object_sets { };
		std::vector<glw::Program*> m_programs { };
		std::vector<glw::Texture*> m_textures { };
	private:
		std::map<DrawableType, GLWObjectSet> m_glw_object_set_map { };
		std::map<DrawableType, glw::Program*> m_program_map { };
		std::map<TextureKey, glw::Texture*> m_texture_map { };
	private:
		std::vector<Object*> m_objects { };
	private:
		uint32_t m_initial_window_width = 1280;
		uint32_t m_initial_window_height = 720;
	public:
		Server() = delete;
		Server(uint32_t _initial_window_width, uint32_t _initial_window_height);
		~Server();
	public:
		const glw::Texture* fetch_texture(const std::string& _path, bool _auto_format = false, glw::TextureConfig _cfg = glw::TextureConfig());
	public:
		const uint32_t& get_initial_window_width() const;
		const uint32_t& get_initial_window_height() const;
	public:
		const GLWObjectSet& get_glw_object_set(const DrawableType& _type) const;
		const glw::Program* get_generic_program(const DrawableType& _type) const;
	public:
		template<typename T>
		T* fetch_object() {
			T* _object = new T(this);
			m_objects.emplace_back(_object);
			return _object;
		}
	};

	class Object abstract {
	private:
		Server* m_owner = nullptr;
	private:
		glm::vec4 m_modulate { 1.0f, 1.0f, 1.0f, 1.0f };
	private:
		glm::vec2 m_position { 0.0f, 0.0f };
		float     m_rotation { 0.0f };
		glm::vec2 m_scale    { 1.0f, 1.0f };
	private:
		bool      m_need_to_update_model_matrix = false;
		glm::mat4 m_model_matrix = glm::mat4(1.0f);
	public:
		Object() = delete;
		Object(Server* _owner);
		virtual ~Object() = 0;
	public:
		virtual void update() = 0;
		virtual void draw(Camera2D* _cam = nullptr) = 0;
	public:
		virtual void set_position(const glm::vec2& _value);
		virtual void set_rotation(const float& _value);
		virtual void set_scale(const glm::vec2& _value);
		virtual void set_modulate(const glm::vec4& _value);
	public:
		Server* fetch_owner();
	public:
		const Server* get_owner() const;
		const glm::vec2& get_position() const;
		const float& get_rotation() const;
		const glm::vec2& get_scale() const;
		const glm::vec4& get_modulate() const;
		const glm::mat4& get_model_matrix() const;
	private:
		void _update_model_matrix();
	};

	class Camera2D final : public Object {
	private:
		glm::vec2 m_zoom { 1.0f, 1.0f };
	private:
		bool m_need_to_view_proj_matrix = false;
		glm::mat4 m_view_proj_matrix = glm::mat4(1.0f);
	public:
		Camera2D(Server* _owner);
		~Camera2D() override;
	public:
		void update() override;
		void draw(Camera2D* _cam) override;
	public:
		virtual void set_position(const glm::vec2& _value);
		virtual void set_rotation(const float& _value);
		virtual void set_scale(const glm::vec2& _value);
		void set_zoom(const glm::vec2& _value);
	public:
		const glm::vec2& get_zoom()const;
		const glm::mat4& get_view_proj_matrix() const;
	public:
		static const glm::mat4& get_default_view_proj_matrix(const uint32_t& _width, const uint32_t& _height);
	private:
		void _update_view_proj_matrix();
	};

	class Sprite : public Object {
	private:
		const glw::VertexArrayObject* m_VAO = nullptr;
		const glw::VertexBufferObject* m_VBO = nullptr;
		const glw::ElementBufferObject* m_EBO = nullptr;
	private:
		const glw::Program* m_program = nullptr;
	private:
		const glw::Texture* m_texture = nullptr;
	public:
		Sprite(Server* _owner);
		~Sprite() override;
	public:
		void update() override;
		void draw(Camera2D* _cam) override;
	public:
		void load(const std::string& _path, bool _auto_format = false, glw::TextureConfig _cfg = glw::TextureConfig());
	};

	class BlendedSprite : public Object {
	public:
		static constexpr size_t maximum_blended_texture_count = 4ULL;
	private:
		const glw::VertexArrayObject* m_VAO = nullptr;
		const glw::VertexBufferObject* m_VBO = nullptr;
		const glw::ElementBufferObject* m_EBO = nullptr;
	private:
		const glw::Program* m_program = nullptr;
	private:
		uint32_t m_base_width = 0U;
		uint32_t m_base_height = 0U;
		std::vector<const glw::Texture*> m_textures { };
	public:
		BlendedSprite(Server* _owner);
		~BlendedSprite() override;
	public:
		void update() override;
		void draw(Camera2D* _cam) override;
	public:
		void set_base_size(const uint32_t& _width, const uint32_t& _height);
	public:
		void clear_textures();
		void load_extra_texture(const std::string& _path, bool _auto_format = false, glw::TextureConfig _cfg = glw::TextureConfig());
	public:
		size_t get_loaded_texture_count() const;
	};

	class ColoredObject : public Object {
	protected:
		const glw::VertexArrayObject* m_VAO = nullptr;
		const glw::VertexBufferObject* m_VBO = nullptr;
		const glw::ElementBufferObject* m_EBO = nullptr;
	protected:
		const glw::Program* m_program = nullptr;
	private:
		glm::vec4 m_color { 1.0f, 1.0f, 1.0f, 1.0f };
	public:
		ColoredObject(Server* _owner);
		virtual ~ColoredObject() override;
	public:
		virtual void update() override;
		virtual void draw(Camera2D* _cam) override;
	public:
		virtual void set_color(const glm::vec4& _value);
	public:
		const glm::vec4& get_color() const;
	};

	class Line final : public ColoredObject {
	private:
		float m_width = 1.0f;
	private:
		glm::vec2 m_point_begin = { 0.0f, 0.0f };
		glm::vec2 m_point_end = { 100.0f, 100.0f };
	public:
		Line(Server* _owner);
		~Line();
	public:
		void update() override;
		void draw(Camera2D* _cam) override;
	public:
		void set_width(const float& _value);
		void set_begin(const glm::vec2& _value);
		void set_end(const glm::vec2& _value);
	public:
		const float& get_width() const;
		const glm::vec2& get_begin() const;
		const glm::vec2& get_end() const;
	};

	class LinedRect final : public ColoredObject {
	private:
		glm::vec2 m_size{ 100.0f, 100.0f };
	private:
		float m_line_width = 1.0f;
	public:
		LinedRect(Server* _owner);
		~LinedRect();
	public:
		void update() override;
		void draw(Camera2D* _cam) override;
	public:
		void set_line_width(const float& _value);
		void set_size(const glm::vec2& _value);
	public:
		const float& get_line_width() const;
		const glm::vec2& get_size() const;
	};

	class ColorRect final : public ColoredObject {
	private:
		glm::vec2 m_size{ 100.0f, 100.0f };
	public:
		ColorRect(Server* _owner);
		~ColorRect();
	public:
		void update() override;
		void draw(Camera2D* _cam) override;
	public:
		void set_size(const glm::vec2& _value);
	public:
		const glm::vec2& get_size() const;
	};

	class RoundedColorRect final : public ColoredObject {
	private:
		float m_outline_width = 1.0f;
		glm::vec4 m_outline_color { 1.0f, 1.0f, 1.0f, 1.0f };
	private:
		float m_corner_radius = 0.5f;
		glm::vec2 m_size { 100.0f, 100.0f };
	public:
		RoundedColorRect(Server* _owner);
		~RoundedColorRect();
	public:
		void update() override;
		void draw(Camera2D* _cam) override;
	public:
		void set_size(const glm::vec2& _value);
		void set_outline_width(const float& _value);
		void set_outline_color(const glm::vec4& _value);
		void set_corner_radius(const float& _value);
	public:
		const glm::vec2& get_size() const;
		const float& get_outline_width() const;
		const glm::vec4& get_outline_color() const;
		const float& get_corner_radius() const;
	};
}

#endif // !__GLWU_HPP__