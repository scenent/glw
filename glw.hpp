#ifndef __GLW_HPP__
#define __GLW_HPP__

/*
	glw

	Unofficial OpenGL wrapper for C++ 17.

	Produced by scenent(https://github.com/scenent/).

	Distributed under MIT License.

	Features
		Encapsulated Classes
			GLFW Window
			OpenGL Objects (VAO, VBO, EBO ...)
			OpenGL Shader & Program
	
	TODOs
		Uniform Buffer Object

	Dependencies
		C++ 17+
		GLFW 3.4
		GLAD (which supports GL 4.6)
*/

#define _GLW_THROW_GLFW_ERROR \
	int _glfw_err_code = 0; \
	const char* _glfw_msg = nullptr; \
	_glfw_err_code = glfwGetError(&_glfw_msg); \
	throw exception::GLFWException(_glfw_err_code, _glfw_msg); \

#define _GLW_FLAG_COMPOSITION(eName, type) \
	static inline eName operator|(eName _lhs, eName _rhs) { \
		return static_cast<eName>(static_cast<type>(_lhs) | static_cast<type>(_rhs)); \
	} \
	static inline eName operator&(eName _lhs, eName _rhs) { \
		return static_cast<eName>(static_cast<type>(_lhs) & static_cast<type>(_rhs)); \
	} \

#define _GLW_ENUM_EQUIVALENT(eName, type) \
	static inline bool operator==(const eName& _lhs, const type& _rhs) { \
		return static_cast<type>(_lhs) == _rhs; \
	} \
	static inline bool operator==(const type& _lhs, const eName& _rhs) { \
		return _lhs == static_cast<type>(_rhs); \
	} \
	static inline bool operator!=(const eName& _lhs, const type& _rhs) { \
		return static_cast<type>(_lhs) != _rhs; \
	} \
	static inline bool operator!=(const type& _lhs, const eName& _rhs) { \
		return _lhs != static_cast<type>(_rhs); \
	} \

/* STL */
#include <stdexcept> // std::runtime_error
#include <string>    // std::string
#include <queue>     // std::queue
#include <map>       // std::map
/* GLAD */
#include <GLAD/glad.h>
/* GLFW */
#include <GLFW/glfw3.h>

namespace glw {
	/* Exceptions */
	namespace exception {
		class GLFWException : public std::runtime_error {
		public:
			GLFWException(int _glfw_err_code, const char* _glfw_msg)
				: std::runtime_error("[GLFW] : Error Code = " + std::to_string(_glfw_err_code) + " : Message = " + _glfw_msg) { }
		};
		class GLADLoadingFailure : public std::runtime_error {
		public:
			GLADLoadingFailure()
				: std::runtime_error("[GLAD] : Failed to load OpenGL") { }
		};
		class GLFramebufferLinkFailure : public std::runtime_error {
		public:
			GLFramebufferLinkFailure(const char* _type)
				: std::runtime_error("[GL] : Failed to link framebuffer : Type = " + std::string(_type)) { }
		};
		class GLShaderCompileFailure : public std::runtime_error {
		public:
			GLShaderCompileFailure(const char* _type, const char* _log)
				: std::runtime_error("[GL] : Failed to compile shader : Type = " + std::string(_type) + " : Log = " + _log) { }
		};
		class GLProgramLinkFailure : public std::runtime_error {
		public:
			GLProgramLinkFailure(const char* _log)
				: std::runtime_error("[GL] : Failed to link program : Log = " + std::string(_log)) { }
		};
	}
	/* GLFW Flags */
	enum class CursorType {
		Arrow = 0,
		IBeam,
		CrossHair,
		PointingHand,
		ResizeEW,
		ResizeNS,
		ResizeNWSE,
		ResizeNESW,
		ResizeAll,
		NotAllowed
	};

	/* GLFW Buttons */
	enum class Button : int {
		Left = GLFW_MOUSE_BUTTON_LEFT,
		Middle = GLFW_MOUSE_BUTTON_MIDDLE,
		Right = GLFW_MOUSE_BUTTON_RIGHT,
	};

	/* GLFW Keys */
	enum class Key : int {
		Space = 32,
		Apostrophe = 39,
		Comma = 44,
		Minus = 45,
		Period = 46,
		Slash = 47,
		Key0 = 48,
		Key1 = 49,
		Key2 = 50,
		Key3 = 51,
		Key4 = 52,
		Key5 = 53,
		Key6 = 54,
		Key7 = 55,
		Key8 = 56,
		Key9 = 57,
		Semicolon = 59,
		Equal = 61,
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		LeftBracket = 91,
		Backslash = 92,
		RightBracket = 93,
		GraveAccent = 96,
		World1 = 161,
		World2 = 162,
		
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,
		Kp0 = 320,
		Kp1 = 321,
		Kp2 = 322,
		Kp3 = 323,
		Kp4 = 324,
		Kp5 = 325,
		Kp6 = 326,
		Kp7 = 327,
		Kp8 = 328,
		Kp9 = 329,
		KpDecimal = 330,
		KpDivide = 331,
		KpMultiply = 332,
		KpSubtract = 333,
		KpAdd = 334,
		KpEnter = 335,
		KpEqual = 336,
		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348,

		Last = 348
	};

	_GLW_ENUM_EQUIVALENT(Key, int);

	/* OpenGL Types & Flags */
	enum class ArrayBufferType : uint32_t {
		VertexBuffer = GL_ARRAY_BUFFER,
		ElementBuffer = GL_ELEMENT_ARRAY_BUFFER,
	};

	enum class DataType : uint32_t {
		Byte = GL_BYTE,
		UnsignedByte = GL_UNSIGNED_BYTE,
		Short = GL_SHORT,
		UnsignedShort = GL_UNSIGNED_SHORT,
		Int = GL_INT,
		UnsignedInt = GL_UNSIGNED_INT,
		Float = GL_FLOAT,
		Double = GL_DOUBLE,

		UnsignedByte332 = GL_UNSIGNED_BYTE_3_3_2,
		UnsignedByte233Rev = GL_UNSIGNED_BYTE_2_3_3_REV,
		UnsignedShort565 = GL_UNSIGNED_SHORT_5_6_5,
		UnsignedShort565Rev = GL_UNSIGNED_SHORT_5_6_5,
		UnsignedShort4444 = GL_UNSIGNED_SHORT_4_4_4_4,
		UnsignedShort4444Rev = GL_UNSIGNED_SHORT_4_4_4_4_REV,
		UnsignedShort5551 = GL_UNSIGNED_SHORT_5_5_5_1,
		UnsignedShort1555Rev = GL_UNSIGNED_SHORT_1_5_5_5_REV,
		UnsignedInt8888 = GL_UNSIGNED_INT_8_8_8_8,
		UnsignedInt8888Rev = GL_UNSIGNED_INT_8_8_8_8_REV,
		UnsignedInt101010102 = GL_UNSIGNED_INT_10_10_10_2
	};

	enum class NumericType : uint32_t {
		Byte = GL_BYTE,
		UnsignedByte = GL_UNSIGNED_BYTE,
		Short = GL_SHORT,
		UnsignedShort = GL_UNSIGNED_SHORT,
		Int = GL_INT,
		UnsignedInt = GL_UNSIGNED_INT,
		Float = GL_FLOAT,
	};

	enum class BufferUsage : uint32_t {
		StreamDraw = GL_STREAM_DRAW,
		StreamRead = GL_STREAM_READ,
		StreamCopy = GL_STREAM_COPY,
		StaticDraw = GL_STATIC_DRAW,
		StaticRead = GL_STATIC_READ,
		StaticCopy = GL_STATIC_COPY,
		DynamicDraw = GL_DYNAMIC_DRAW,
		DynamicRead = GL_DYNAMIC_READ,
		DynamicCopy = GL_DYNAMIC_COPY
	};

	enum class BufferType : uint32_t {
		Color = GL_COLOR_BUFFER_BIT,
		Depth = GL_DEPTH_BUFFER_BIT,
		Stencil = GL_STENCIL_BUFFER_BIT,
	};

	enum class PrimitiveType : uint32_t {
		Points = GL_POINTS,
		Lines = GL_LINES,
		LineLoop = GL_LINE_LOOP,
		LineStrip = GL_LINE_STRIP,
		Triangles = GL_TRIANGLES,
		TriangleStrip = GL_TRIANGLE_STRIP,
		TriangleFan = GL_TRIANGLE_FAN,
		Quads = GL_QUADS
	};

	enum class CapabilityType : uint32_t {
		DepthTest = GL_DEPTH_TEST,
		StencilTest = GL_STENCIL_TEST,
		CullFace = GL_CULL_FACE,
		Blend = GL_BLEND,
		Scissor = GL_SCISSOR_TEST,
	};

	/*
	GL_ZERO, 
	GL_ONE,

	GL_SRC_COLOR, 
	GL_SRC_ALPHA, 
	GL_DST_COLOR, 
	GL_DST_ALPHA,
	
	GL_ONE_MINUS_SRC_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_ONE_MINUS_SRC_ALPHA, 
	GL_ONE_MINUS_DST_ALPHA. 
	GL_ONE_MINUS_CONSTANT_COLOR, 
	GL_ONE_MINUS_CONSTANT_ALPHA
	GL_CONSTANT_COLOR, 
	GL_CONSTANT_ALPHA,
	*/

	enum class BlendFuncType : uint32_t {
		Zero = GL_ZERO,
		One = GL_ONE,
		SrcColor = GL_SRC_COLOR,
		SrcAlpha = GL_SRC_ALPHA,
		DstColor = GL_DST_COLOR,
		DstAlpha = GL_DST_ALPHA,
		OneMinusSrcColor = GL_ONE_MINUS_SRC_COLOR,
		OneMinusDstColor = GL_ONE_MINUS_DST_COLOR,
		OneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA,
		OneMinusDstAlpha = GL_ONE_MINUS_DST_ALPHA,
		OneMinusConstantColor = GL_ONE_MINUS_CONSTANT_COLOR,
		OneMinusConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA,
		ConstantColor = GL_CONSTANT_COLOR,
		ConstantAlpha = GL_CONSTANT_ALPHA,
	};

	enum class CullingMode : uint32_t {
		Front = GL_FRONT,
		Back = GL_BACK,
		FrontAndBack = GL_FRONT_AND_BACK
	};

	enum class StencilTestFunctionType : uint32_t {
		Never = GL_NEVER,
		Less = GL_LESS,
		LessEqual = GL_LEQUAL,
		Greater = GL_GREATER,
		GreaterEqual = GL_GEQUAL,
		Equal = GL_EQUAL,
		NotEqual = GL_NOTEQUAL,
		Always = GL_ALWAYS,
	};

	enum class StencilActionType : uint32_t {
		Keep = GL_KEEP,
		Zero = GL_ZERO,
		Replace = GL_REPLACE,
		Increase = GL_INCR,
		IncreaseWrap = GL_INCR_WRAP,
		Decrease = GL_DECR,
		DecreaseWrap = GL_DECR_WRAP,
		Invert = GL_INVERT
	};

	enum class InternalFormat : uint32_t {
		CompressedRed = GL_COMPRESSED_RED,
		CompressedRedRGTC1 = GL_COMPRESSED_RED_RGTC1,
		CompressedRG = GL_COMPRESSED_RG,
		CompressedRGB = GL_COMPRESSED_RGB,
		CompressedRGBA = GL_COMPRESSED_RGBA,
		CompressedRGRGTC2 = GL_COMPRESSED_RG_RGTC2,
		CompressedSignedRedRGTC1 = GL_COMPRESSED_SIGNED_RED_RGTC1,
		CompressedSignedRGRGTC2 = GL_COMPRESSED_SIGNED_RG_RGTC2,
		CompressedSRGB = GL_COMPRESSED_SRGB,
		DepthStencil = GL_DEPTH_STENCIL,
		Depth24Stencil8 = GL_DEPTH24_STENCIL8,
		Depth32FStencil8 = GL_DEPTH32F_STENCIL8,
		DepthComponent = GL_DEPTH_COMPONENT,
		DepthComponent16 = GL_DEPTH_COMPONENT16,
		DepthComponent24 = GL_DEPTH_COMPONENT24,
		DepthComponent32F = GL_DEPTH_COMPONENT32F,
		R16F = GL_R16F,
		R16I = GL_R16I,
		R16SNorm = GL_R16_SNORM,
		R16UI = GL_R16UI,
		R32F = GL_R32F,
		R32I = GL_R32I,
		R32UI = GL_R32UI,
		R3G3B2 = GL_R3_G3_B2,
		R8 = GL_R8,
		R8I = GL_R8I,
		R8SNorm = GL_R8_SNORM,
		R8UI = GL_R8UI,
		Red = GL_RED,
		RG = GL_RG,
		RG16 = GL_RG16,
		RG16F = GL_RG16F,
		RG16SNorm = GL_RG16_SNORM,
		RG32F = GL_RG32F,
		RG32I = GL_RG32I,
		RG32UI = GL_RG32UI,
		RG8 = GL_RG8,
		RG8I = GL_RG8I,
		RG8SNorm = GL_RG8_SNORM,
		RG8UI = GL_RG8UI,
		RGB = GL_RGB,
		RGB10 = GL_RGB10,
		RGB10A2 = GL_RGB10_A2,
		RGB12 = GL_RGB12,
		RGB16 = GL_RGB16,
		RGB16F = GL_RGB16F,
		RGB16I = GL_RGB16I,
		RGB16UI = GL_RGB16UI,
		RGB32F = GL_RGB32F,
		RGB32I = GL_RGB32I,
		RGB32UI = GL_RGB32UI,
		RGB4 = GL_RGB4,
		RGB5 = GL_RGB5,
		RGB5A1 = GL_RGB5_A1,
		RGB8 = GL_RGB8,
		RGB8I = GL_RGB8I,
		RGB8UI = GL_RGB8UI,
		RGB9E5 = GL_RGB9_E5,
		RGBA = GL_RGBA,
		RGBA12 = GL_RGBA12,
		RGBA16 = GL_RGBA16,
		RGBA16F = GL_RGBA16F,
		RGBA16I = GL_RGBA16I,
		RGBA16UI = GL_RGBA16UI,
		RGBA2 = GL_RGBA2,
		RGBA32F = GL_RGBA32F,
		RGBA32I = GL_RGBA32I,
		RGBA32UI = GL_RGBA32UI,
		RGBA4 = GL_RGBA4,
		RGBA8 = GL_RGBA8,
		RGBA8UI = GL_RGBA8UI,
		SRGB8 = GL_SRGB8,
		SRGB8A8 = GL_SRGB8_ALPHA8,
		SRGBA = GL_SRGB_ALPHA
	};

	enum class TextureFormat : uint32_t {
		Red = GL_RED,
		Green = GL_GREEN,
		Blue = GL_BLUE,
		Alpha = GL_ALPHA,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		BGR = GL_BGR,
		BGRA = GL_BGRA,
		RG = GL_RG,
		RedInteger = GL_RED_INTEGER, 
		RGInteger = GL_RG_INTEGER, 
		RGBInteger = GL_RGB_INTEGER, 
		BGRInteger = GL_BGR_INTEGER, 
		RGBAInteger = GL_RGBA_INTEGER,
		BGRAInteger = GL_BGRA_INTEGER, 
		StencilIndex = GL_STENCIL_INDEX, 
		DepthComponent = GL_DEPTH_COMPONENT, 
		DepthStencil = GL_DEPTH_STENCIL
	};

	enum class TextureSlot : uint32_t {
		S0 = GL_TEXTURE0,
		S1 = GL_TEXTURE1,
		S2 = GL_TEXTURE2,
		S3 = GL_TEXTURE3,
		S4 = GL_TEXTURE4,
		S5 = GL_TEXTURE5,
		S6 = GL_TEXTURE6,
		S7 = GL_TEXTURE7,
		S8 = GL_TEXTURE8,
		S9 = GL_TEXTURE9,
		S10 = GL_TEXTURE10,
		S11 = GL_TEXTURE11,
		S12 = GL_TEXTURE12,
		S13 = GL_TEXTURE13,
		S14 = GL_TEXTURE14,
		S15 = GL_TEXTURE15,
		S16 = GL_TEXTURE16,
		S17 = GL_TEXTURE17,
		S18 = GL_TEXTURE18,
		S19 = GL_TEXTURE19,
		S20 = GL_TEXTURE20,
		S21 = GL_TEXTURE21,
		S22 = GL_TEXTURE22,
		S23 = GL_TEXTURE23,
		S24 = GL_TEXTURE24,
		S25 = GL_TEXTURE25,
		S26 = GL_TEXTURE26,
		S27 = GL_TEXTURE27,
		S28 = GL_TEXTURE28,
		S29 = GL_TEXTURE29,
		S30 = GL_TEXTURE30,
		S31 = GL_TEXTURE31
	};

	enum class ReadBufferType : uint32_t {
		Back = GL_BACK,
		None = GL_NONE,
		Color0 = GL_COLOR_ATTACHMENT0,
		Color1 = GL_COLOR_ATTACHMENT1,
		Color2 = GL_COLOR_ATTACHMENT2,
		Color3 = GL_COLOR_ATTACHMENT3,
		Color4 = GL_COLOR_ATTACHMENT4,
		Color5 = GL_COLOR_ATTACHMENT5,
		Color6 = GL_COLOR_ATTACHMENT6,
		Color7 = GL_COLOR_ATTACHMENT7,
		Color8 = GL_COLOR_ATTACHMENT8,
		Color9 = GL_COLOR_ATTACHMENT9,
		Color10 = GL_COLOR_ATTACHMENT10,
		Color11 = GL_COLOR_ATTACHMENT11,
		Color12 = GL_COLOR_ATTACHMENT12,
		Color13 = GL_COLOR_ATTACHMENT13,
		Color14 = GL_COLOR_ATTACHMENT14,
		Color15 = GL_COLOR_ATTACHMENT15,
		Color16 = GL_COLOR_ATTACHMENT16,
		Color17 = GL_COLOR_ATTACHMENT17,
		Color18 = GL_COLOR_ATTACHMENT18,
		Color19 = GL_COLOR_ATTACHMENT19,
		Color20 = GL_COLOR_ATTACHMENT20,
		Color21 = GL_COLOR_ATTACHMENT21,
		Color22 = GL_COLOR_ATTACHMENT22,
		Color23 = GL_COLOR_ATTACHMENT23,
		Color24 = GL_COLOR_ATTACHMENT24,
		Color25 = GL_COLOR_ATTACHMENT25,
		Color26 = GL_COLOR_ATTACHMENT26,
		Color27 = GL_COLOR_ATTACHMENT27,
		Color28 = GL_COLOR_ATTACHMENT28,
		Color29 = GL_COLOR_ATTACHMENT29,
		Color30 = GL_COLOR_ATTACHMENT30,
		Color31 = GL_COLOR_ATTACHMENT31,
	};

	enum class DrawBufferType : uint32_t {
		None = GL_NONE, 
		FrontLeft = GL_FRONT_LEFT, 
		FrontRight = GL_FRONT_RIGHT, 
		BackLeft = GL_BACK_LEFT, 
		BackRight = GL_BACK_RIGHT, 
		Front = GL_FRONT, 
		Back = GL_BACK, 
		Left = GL_LEFT, 
		Right = GL_RIGHT,
		FrontAndBack = GL_FRONT_AND_BACK
	};

	enum class AttachmentType : uint32_t {
		Color0 = GL_COLOR_ATTACHMENT0,
		Color1 = GL_COLOR_ATTACHMENT1,
		Color2 = GL_COLOR_ATTACHMENT2,
		Color3 = GL_COLOR_ATTACHMENT3,
		Color4 = GL_COLOR_ATTACHMENT4,
		Color5 = GL_COLOR_ATTACHMENT5,
		Color6 = GL_COLOR_ATTACHMENT6,
		Color7 = GL_COLOR_ATTACHMENT7,
		Color8 = GL_COLOR_ATTACHMENT8,
		Color9 = GL_COLOR_ATTACHMENT9,
		Color10 = GL_COLOR_ATTACHMENT10,
		Color11 = GL_COLOR_ATTACHMENT11,
		Color12 = GL_COLOR_ATTACHMENT12,
		Color13 = GL_COLOR_ATTACHMENT13,
		Color14 = GL_COLOR_ATTACHMENT14,
		Color15 = GL_COLOR_ATTACHMENT15,
		Color16 = GL_COLOR_ATTACHMENT16,
		Color17 = GL_COLOR_ATTACHMENT17,
		Color18 = GL_COLOR_ATTACHMENT18,
		Color19 = GL_COLOR_ATTACHMENT19,
		Color20 = GL_COLOR_ATTACHMENT20,
		Color21 = GL_COLOR_ATTACHMENT21,
		Color22 = GL_COLOR_ATTACHMENT22,
		Color23 = GL_COLOR_ATTACHMENT23,
		Color24 = GL_COLOR_ATTACHMENT24,
		Color25 = GL_COLOR_ATTACHMENT25,
		Color26 = GL_COLOR_ATTACHMENT26,
		Color27 = GL_COLOR_ATTACHMENT27,
		Color28 = GL_COLOR_ATTACHMENT28,
		Color29 = GL_COLOR_ATTACHMENT29,
		Color30 = GL_COLOR_ATTACHMENT30,
		Color31 = GL_COLOR_ATTACHMENT31,
		Depth = GL_DEPTH_ATTACHMENT,
		Stencil = GL_STENCIL_ATTACHMENT,
		DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
	};

	enum class WrappingType : uint32_t {
		ClampEdge = GL_CLAMP_TO_EDGE,
		ClampBorder = GL_CLAMP_TO_BORDER,
		Repeat = GL_REPEAT,
		MirroredRepeat = GL_MIRRORED_REPEAT
	};

	enum class FilterType : uint32_t {
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR,
		NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
		LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
		NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
		LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
	};

	enum class ErrorType : uint32_t {
		NoError = GL_NO_ERROR,
		InvalidEnum = GL_INVALID_ENUM,
		InvalidValue = GL_INVALID_VALUE,
		InvalidOperation = GL_INVALID_OPERATION,
		InvalidFramebufferOperation = GL_INVALID_FRAMEBUFFER_OPERATION,
		OutOfMemory = GL_OUT_OF_MEMORY,
		StackUnderflow = GL_STACK_UNDERFLOW,
		StackOverflow = GL_STACK_OVERFLOW
	};

	enum class FramebufferLinkStatusType : uint32_t {
		Complete = GL_FRAMEBUFFER_COMPLETE,
		IncompleteAttachment = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
		IncompleteMissingAttachment = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
		IncompleteDrawBuffer = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,
		IncompleteReadBuffer = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,
		IncompleteMultisample = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,
		IncompleteLayerTargets = GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS,
		Unsupported = GL_FRAMEBUFFER_UNSUPPORTED,
		Undefined = GL_FRAMEBUFFER_UNDEFINED,
	};

	enum class ShaderType : uint32_t {
		Vertex = GL_VERTEX_SHADER,
		TessellationControl = GL_TESS_CONTROL_SHADER,
		TessellationEvaluation = GL_TESS_EVALUATION_SHADER,
		Geometry = GL_GEOMETRY_SHADER,
		Fragment = GL_FRAGMENT_SHADER,
		Compute = GL_COMPUTE_SHADER,
	};

	_GLW_FLAG_COMPOSITION(BufferType, uint32_t);
	_GLW_FLAG_COMPOSITION(CapabilityType, uint32_t);


	/* GLFW & GLAD Wrappers */
	class Context;
	class ErrorQueue;

	struct Event;
	struct WindowConfig;
	class Window;

	/* OpenGL Wrappers */
	class BufferData;

	class Handle;
	class VertexArrayObject;
	class VertexBufferObject;
	class ElementBufferObject;
	class FrameBufferObject;
	class RenderBufferObject;

	struct TextureConfig;
	class Texture;

	class Shader;
	class Program;
	
	class Context final {
	public:
		Context() = delete;
		Context(const Context&) = delete;
		Context(Context&&) = delete;
		Context& operator=(const Context&) = delete;
		Context& operator=(Context&&) = delete;
		~Context() = delete;
	public:
		static void initialize();
		static void make_current(Window* _target);
		static void activate();
		static void set_interval(int _value) noexcept;
		static void finalize() noexcept;
	public:
		static double get_time() noexcept;
		static ErrorType get_error() noexcept;
		static void swap_buffers(Window* _target);
		static void poll_events() noexcept;
	public:
		static void viewport(int _xpos, int _ypos, uint32_t _width, uint32_t _height);
	public:
		static void get_viewport(int& _out_xpos, int& _out_ypos, uint32_t& _out_width, uint32_t& _out_height);
		static bool get_is_enabled(CapabilityType _type);
	public:
		static void blend_func(BlendFuncType _src, BlendFuncType _dst);
		static void enable(CapabilityType _type);
		static void disable(CapabilityType _type);
		static void set_line_width(float _value);
		static void set_scissor(int _x, int _y, uint32_t _w, uint32_t _h);
		static void set_culling_mode(CullingMode _mode);
		static void clear_buffer(BufferType _flags);
		static void clear_color(const float& _r, const float& _g, const float& _b, const float& _a);
	public:
		static uint32_t generate_vertex_array();
		static void bind_vertex_array(const uint32_t& _id);
		static void enable_vertex_attrib_array(const uint32_t& _offset_index);
		static void vertex_attrib_pointer(const uint32_t& _offset_index, const uint32_t& _count, const NumericType& _type, bool _norm, const size_t& _stride, const intptr_t& _offset);
		static void delete_vertex_array(const uint32_t& _id);
	public:
		static uint32_t generate_buffer();
		static void bind_array_buffer(const ArrayBufferType& _type, const uint32_t& _id);
		static void bind_array_buffer_data(const ArrayBufferType& _type, const BufferData& _data, BufferUsage _usage);
		static void bind_empty_array_buffer_data(const ArrayBufferType& _type, const size_t& _length, BufferUsage _usage);
		static void bind_array_buffer_sub_data(const ArrayBufferType& _type, const BufferData& _data, const size_t& _offset);
		static void delete_buffer(const uint32_t& _id);
	public:
		static uint32_t generate_frame_buffer();
		static void bind_frame_buffer(const uint32_t& _id);
		static void attach_texture_2d(const uint32_t& _fbo, AttachmentType _type, const uint32_t& _id);
		static void attach_render_buffer(const uint32_t& _fbo, AttachmentType _type, const uint32_t& _rbo);
		static void check_framebuffer_link_status();
		static void delete_frame_buffer(const uint32_t& _id);
	public:
		static void read_buffer(ReadBufferType _type);
		static void draw_buffer(DrawBufferType _type);
	public:
		static uint32_t generate_render_buffer();
		static void bind_render_buffer(const uint32_t& _id);
		static void set_render_buffer_storage(InternalFormat _format, const uint32_t& _width, const uint32_t& _height);
		static void delete_render_buffer(const uint32_t& _id);
	public:
		static uint32_t generate_texture();
		static void set_filter(FilterType _mag, FilterType _min);
		static void set_wrapping_mode(WrappingType _s, WrappingType _t);
		static void set_image(const unsigned char* _data, uint32_t _width, uint32_t _height, InternalFormat _i_format, TextureFormat _t_format, DataType _type);
		static void generate_mipmap();
		static void activate_texture(const TextureSlot& _slot);
		static void bind_texture_2d(const uint32_t& _id);
		static void delete_texture(const uint32_t& _id);
	public:
		static uint32_t create_shader(const ShaderType& _type);
		static void set_source(const uint32_t& _id, const char* _src);
		static void compile_shader(const ShaderType& _type, const uint32_t& _id);
		static void delete_shader(const uint32_t& _id);
		static void check_shader_compile_status(const ShaderType& _type, const uint32_t& _id);
	public:
		static uint32_t create_program();
		static void attach_shader(const uint32_t& _program, const uint32_t& _shader);
		static void link_program(const uint32_t& _id);
		static void check_program_link_status(const uint32_t& _id);
		static int get_uniform_location(const uint32_t& _id, const char* _name);
		static void set_uniform_int(const int& _loc, int _value);
		static void set_uniform_uint(const int& _loc, uint32_t _value);
		static void set_uniform_float(const int& _loc, float _value);
		static void set_uniform_vec2(const int& _loc, float _x, float _y);
		static void set_uniform_vec3(const int& _loc, float _x, float _y, float _z);
		static void set_uniform_vec4(const int& _loc, float _x, float _y, float _z, float _w);
		static void set_uniform_mat2(const int& _loc, const float* _ptr);
		static void set_uniform_mat3(const int& _loc, const float* _ptr);
		static void set_uniform_mat4(const int& _loc, const float* _ptr);
		static void use_program(const uint32_t& _id);
		static void delete_program(const uint32_t& _id);
	public:
		static void draw_arrays(
			const PrimitiveType& _mode,
			const int& _offset,
			const uint32_t& _index_count
		);
		static void draw_elements(
			const PrimitiveType& _mode,
			const uint32_t& _index_count,
			const NumericType& _type,
			const intptr_t& _offset
		);
	};

	class ErrorQueue final {
	private:
		std::queue<ErrorType> m_queue;
	private:
		ErrorQueue() = default;
		~ErrorQueue() = default;
	public:
		static ErrorQueue& get_instance() {
			static ErrorQueue _instance;
			return _instance;
		}
	public:
		void check();
		void pop();
		ErrorType& front();
		bool empty();
		void flush();
	};

	enum class EventType : uint32_t {
		Unknown = 0,
		KeyJustPressed,
		KeyJustReleased,
		MousePositionChanged,
		MouseButtonUp,
		MouseButtonDown,
		MouseScrollDetected,
		FramebufferResized,
		WindowMoved,
		WindowResized,
		CharInputted,
		TerminationRequested,
		WindowMinimized,
		WindowMaximized,
	};

	struct Event final {
		union {
			struct {
				Key key;
			} just_pressed_key;

			struct {
				Key key;
			} just_released_key;

			struct {
				double x;
				double y;
				double relx;
				double rely;
			} changed_mouse_position;

			struct {
				Button button_index;
			} upped_mouse_button;

			struct {
				Button button_index;
			} downed_mouse_button;

			struct {
				double amount_x;
				double amount_y;
			} scrolled_mouse;

			struct {
				int xpos;
				int ypos;
			} moved_window;

			struct {
				uint32_t width;
				uint32_t height;
			} resized_window;

			struct {
				uint32_t width;
				uint32_t height;
			} resized_framebuffer;

			struct {
				uint32_t codepoint;
			} inputted_char;

			struct {
				bool toggle;
			} minimized_window;

			struct {
				bool toggle;
			} maximized_window;
		} data { };
		EventType type = EventType::Unknown;
	};

	struct WindowConfig {
	public:
		int GLMajorVersion = 3;
		int GLMinorVersion = 3;
		bool GLCoreProfileEnabled = false;
	public:
		std::string Title = "";
		int  Width = 1280;
		int  Height = 720;
		int  PosX = static_cast<int>(GLFW_ANY_POSITION);
		int  PosY = static_cast<int>(GLFW_ANY_POSITION);
		int  MSAAFactor = 4;
		bool Visible = true;
	public:
		bool Resizable = true;
		bool FocusOnShow = true;
	public:
		bool Minimized = false;
		bool Maximized = false;
		bool Centered = true;
		bool FullscreenEnabled = false;
		bool CursorDisabled = false;
		bool RelativeCursorRequested = false;
	};

	class Window final {
	private:
		static void callback_window_pos_changed(GLFWwindow* window, int xpos, int ypos);
		static void callback_window_size_changed(GLFWwindow* window, int width, int height);
		static void callback_buffer_size_changed(GLFWwindow* window, int width, int height);
		static void callback_key_detected(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void callback_mouse_scrolled(GLFWwindow* window, double xoffset, double yoffset);
		static void callback_mouse_moved(GLFWwindow* window, double xposIn, double yposIn);
		static void callback_mouse_button_detected(GLFWwindow* window, int button, int action, int mods);
		static void callback_char_inputted(GLFWwindow* window, unsigned int codepoint);
		static void callback_termination_requested(GLFWwindow* window);
		static void callback_window_minimized(GLFWwindow* window, int value);
		static void callback_window_maximized(GLFWwindow* window, int value);
	private:
		WindowConfig m_config { };
		int          m_origin_width = 0;
		int          m_origin_height = 0;
	private:
		GLFWwindow* m_window = nullptr;
	private:
		GLFWcursor* m_cursor_arrow = nullptr;
		GLFWcursor* m_cursor_ibeam = nullptr;
		GLFWcursor* m_cursor_crosshair = nullptr;
		GLFWcursor* m_cursor_pointing_hand = nullptr;
		GLFWcursor* m_cursor_resize_ew = nullptr;
		GLFWcursor* m_cursor_resize_ns = nullptr;
		GLFWcursor* m_cursor_resize_nwse = nullptr;
		GLFWcursor* m_cursor_resize_nesw = nullptr;
		GLFWcursor* m_cursor_resize_all = nullptr;
		GLFWcursor* m_cursor_not_allowed = nullptr;
	private:
		std::queue<Event> m_event_queue { };
	public:
		Window() = delete;
		Window(const WindowConfig& _cfg);
		~Window();
	public:
		bool pop_event(Event& _out_event);
	public:
		void set_title(const char* _value);
		void set_resizable(bool _value);
		void set_visible(bool _value);
		void set_size(int _width, int _height);
		void set_width(int _value);
		void set_height(int _value);
		void set_position(int _xpos, int _ypos);
		void set_mouse_position(double _xpos, double _ypos);
		void set_fullscreen_enabled(bool _value);
		void set_cursor_disabled(bool _value);
	public:
		void set_icon(unsigned char* _data);
		void set_cursor(const CursorType& _type);
	public:
		GLFWwindow* get();
		bool get_should_be_closed();
	public:
		void get_position(int& _out_xpos, int& _out_ypos) const;
		void get_mouse_position(double& _out_xpos, double& _out_ypos) const;
		bool get_key_is_held(const Key& _key) const;
		bool get_button_is_held(const Button& _btn) const;
		void get_monitor_resolution(int& _out_width, int& _out_height) const;
	public:
		const bool& get_visible() const;
		const int& get_width() const;
		const int& get_height() const;
		const int& get_origin_width() const;
		const int& get_origin_height() const;
		const bool& get_fullscreen_enabled() const;
		const bool& get_cursor_disabled() const;
	};

	class BufferData final {
	private:
		std::vector<unsigned char> m_data{};
	public:
		BufferData() = default;
		~BufferData() = default;
	public:
		void add_int32(const int& _data);
		void add_uint32(const uint32_t& _data);
		void add_float(const float& _data);
	private:
		void _add(const unsigned char* _data, size_t _count);
	public:
		const void* get_pointer() const;
		size_t get_length() const;
	};
	
	class Handle abstract {
	protected:
		uint32_t m_ID = 0U;
	public:
		Handle() = delete;
		Handle(const uint32_t& _id);
		virtual ~Handle() = 0;
	public:
		virtual void bind() const = 0;
	public:
		const uint32_t& id() const;
	};

	class VertexArrayObject final : public Handle {
	public:
		VertexArrayObject();
		~VertexArrayObject() override;
	public:
		void bind() const override;
	public:
		void attrib_pointer(const uint32_t& _offset_index, const uint32_t& _count, const NumericType& _type, const size_t& _stride, intptr_t _offset) const;
	public:
		static void bind_default();
	};

	class VertexBufferObject final : public Handle {
	public:
		VertexBufferObject();
		~VertexBufferObject() override;
	public:
		void bind() const override;
	public:
		static void bind_default();
	public:
		void bind_data(const BufferData& _data, BufferUsage _usage) const;
		void bind_empty_data(const size_t& _length, BufferUsage _usage) const;
		void bind_sub_data(const BufferData& _data, size_t _offset) const;
	};

	class ElementBufferObject final : public Handle {
	public:
		ElementBufferObject();
		~ElementBufferObject() override;
	public:
		void bind() const override;
	public:
		static void bind_default();
	public:
		void bind_data(const BufferData& _data, BufferUsage _usage) const;
		void bind_empty_data(const size_t& _length, BufferUsage _usage) const;
		void bind_sub_data(const BufferData& _data, size_t _offset) const;
	};
	
	class FrameBufferObject : public Handle {
	private:
		std::map<AttachmentType, const Texture*> m_texture_attachments { };
	public:
		FrameBufferObject();
		~FrameBufferObject() override;
	public:
		void bind() const override;
	public:
		static void bind_default();
	public:
		void attach_texture(AttachmentType _type, const Texture* _tex);
		void attach_render_buffer(AttachmentType _type, const RenderBufferObject* _rbo);
	public:
		const Texture* get_attached_texture(AttachmentType _type) const;
	public:
		static const char* link_status_type_to_str(const FramebufferLinkStatusType& _type);
	};

	class RenderBufferObject : public Handle {
	public:
		RenderBufferObject();
		~RenderBufferObject() override;
	public:
		void bind() const override;
	public:
		static void bind_default();
	public:
		void set_storage(InternalFormat _format, uint32_t _width, uint32_t _height);
	};

	struct TextureConfig {
		FilterType MagFilter = FilterType::Linear;
		FilterType MinFilter = FilterType::Linear;
		WrappingType WrappingS = WrappingType::Repeat;
		WrappingType WrappingT = WrappingType::Repeat;
		InternalFormat FormatInternal = InternalFormat::RGBA;
		TextureFormat FormatTexture = TextureFormat::RGBA;
		DataType FormatData = DataType::UnsignedByte;
		bool MipmapGeneration = false;
		bool FilpY = true;
	};

	class Texture : public Handle {
	private:
		TextureConfig m_config { };
	private:
		uint32_t m_width = 0U;
		uint32_t m_height = 0U;
		uint32_t m_channels = 0U;
	public:
		Texture(const TextureConfig& _cfg, const uint32_t& _width, const uint32_t& _height, const uint32_t& _channels);
		Texture(const TextureConfig& _cfg, const unsigned char* _data, const uint32_t& _width, const uint32_t& _height, const uint32_t& _channels);
		~Texture() override;
	public:
		void bind() const override;
	public:
		static void bind_default();
	public:
		const uint32_t& width() const;
		const uint32_t& height() const;
		const uint32_t& channels() const;
	};

	class Shader final {
	private:
		ShaderType m_type = ShaderType::Vertex;
		uint32_t m_ID = 0U;
	public:
		Shader() = delete;
		Shader(const ShaderType& _type, const char* _src);
		~Shader();
	public:
		const uint32_t& id() const;
		const ShaderType& type() const;
		const char* type_str() const;
	public:
		static const char* type_to_str(const ShaderType& _type);
	};

	class Program final {
	private:
		uint32_t m_ID = 0U;
	private:
		std::map<std::string_view, int> m_uniform_locations { };
	public:
		Program();
		~Program();
	public:
		void use() const;
		static void use_default();
	public:
		void attach(const Shader* _shader) const;
		void link() const;
	public:
		void register_uniform(const std::string_view& _name);
	public:
		void set_sampler2D(const std::string_view& _name, int _slot_id) const;
		void set_int(const std::string_view& _name, int _value) const;
		void set_uint(const std::string_view& _name, uint32_t _value) const;
		void set_float(const std::string_view& _name, float _value) const;
		void set_vec2(const std::string_view& _name, float _x, float _y) const;
		void set_vec3(const std::string_view& _name, float _x, float _y, float _z) const;
		void set_vec4(const std::string_view& _name, float _x, float _y, float _z, float _w) const;
		void set_mat2(const std::string_view& _name, const float* _ptr) const;
		void set_mat3(const std::string_view& _name, const float* _ptr) const;
		void set_mat4(const std::string_view& _name, const float* _ptr) const;
	public:
		const uint32_t& id() const;
	};
}

#endif // !__GLW_HPP__