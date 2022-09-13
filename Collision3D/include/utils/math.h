#pragma once
#include <cmath>
#include <cassert>
#include <cstdint>
#include <immintrin.h>
#include <string>
#include <iostream>

#pragma warning (push)
#pragma warning (disable : 4201) //to avoid nameless struct / union warning.
namespace fm //Fast Math
{
	inline float pi()
	{
		return 3.14159265358979323846f;
	}

	inline float to_radians(float degrees)
	{
		return degrees * 0.01745329251994329576923690768489f;
	}

	template<typename T>
	T min(T a, T b)
	{
		return ((a) < (b) ? (a) : (b));
	}

	template<typename T>
	T max(T a, T b)
	{
		return ((a) > (b) ? (a) : (b));
	}

	template<typename T>
	T clamp(T x, T a, T b)
	{
		return max(min(x, b), a);
	}
	struct vec2
	{
		float x, y;

		vec2(float x, float y)
			: x(x), y(y) {}
		vec2(const float i)
			: vec2(i, i) {}
		vec2()
			: vec2(0) {}
		vec2(const vec2& i)
			: vec2(i.x, i.y) {}

		float& operator[](size_t i)
		{
			assert(i < 2);
			return *(&x + i);
		}

		vec2 operator - () const { return { -x, -y }; }
		vec2 operator + (const vec2& i) const { return { x + i.x, y + i.y }; }
		vec2 operator - (const vec2& i) const { return { x - i.x, y - i.y }; }
		vec2 operator * (const vec2& i) const { return { x * i.x, y * i.y }; }
		vec2 operator * (const float i) const { return { x * i, y * i }; }
		vec2 operator / (const vec2& i) const { return { x / i.x, y / i.y }; }
		vec2 operator / (const float i) const { return { x / i, y / i }; }
		const vec2& operator += (const vec2& i) { return *this = *this + i; }
		const vec2& operator -= (const vec2& i) { return *this = *this - i; }
		const vec2& operator *= (const vec2& i) { return *this = *this * i; }
		const vec2& operator *= (const float i) { return *this = *this * i; }
		const vec2& operator /= (const vec2& i) { return *this = *this / i; }
		const vec2& operator /= (const float i) { return *this = *this / i; }
	};
	
	struct vec3
	{
		float x, y, z;

		vec3(float x, float y, float z)
			: x(x), y(y), z(z) {}
		vec3(const float i)
			: vec3(i, i, i) {}
		vec3()
			: vec3(0) {}
		vec3(const vec3& i)
			: vec3(i.x, i.y, i.z) {}

		float& operator[](size_t i)
		{
			assert(i < 3);
			return *(&x + i);
		}

		vec3 operator - () const { return { -x, -y, -z }; }
		vec3 operator + (const vec3& i) const { return { x + i.x, y + i.y, z + i.z }; }
		vec3 operator - (const vec3& i) const { return { x - i.x, y - i.y, z - i.z }; }
		vec3 operator * (const vec3& i) const { return { x * i.x, y * i.y, z * i.z }; }
		vec3 operator * (const float i) const { return { x * i, y * i, z * i }; }
		vec3 operator / (const vec3& i) const { return { x / i.x, y / i.y, z / i.z }; }
		vec3 operator / (const float i) const { return { x / i, y / i, z / i }; }
		const vec3& operator += (const vec3& i) { return *this = *this + i; }
		const vec3& operator -= (const vec3& i) { return *this = *this - i; }
		const vec3& operator *= (const vec3& i) { return *this = *this * i; }
		const vec3& operator *= (const float i) { return *this = *this * i; }
		const vec3& operator /= (const vec3& i) { return *this = *this / i; }
		const vec3& operator /= (const float i) { return *this = *this / i; }

		bool operator==(const vec3& i) { return x == i.x && y == i.y && z == i.z;}

		[[nodiscard]] uint32_t get_argb() const
		{
			const float t_r = clamp<float>(x, 0, 1);
			const float t_g = clamp<float>(y, 0, 1);
			const float t_b = clamp<float>(z, 0, 1);
			return 255 | (static_cast<uint8_t>(t_r * 255) << 16) | (static_cast<uint8_t>(t_g * 255) << 8) | (static_cast<uint8_t>(t_b * 255));
		}

		[[nodiscard]] uint32_t get_rgba() const
		{
			const float t_r = clamp<float>(x, 0, 1);
			const float t_g = clamp<float>(y, 0, 1);
			const float t_b = clamp<float>(z, 0, 1);
			return static_cast<uint32_t>((static_cast<uint8_t>(t_r * 255) << 24) | (static_cast<uint8_t>(t_g * 255) << 16) | (static_cast<uint8_t>(t_b * 255) << 8) | 255);
		}

		[[nodiscard]] float dot(const vec3& i) const
		{
			return (x * i.x + y * i.y + z * i.z);
		}

		[[nodiscard]] vec3 cross(const vec3& i) const
		{
			return vec3(
				y * i.z - z * i.y, 
				z * i.x - x * i.z, 
				x * i.y - y * i.x);
		}
	};

	struct alignas(4 * sizeof(float)) vec4
	{
		float x, y, z, w;

		vec4(float x, float y, float z, float w)
			: x(x), y(y), z(z), w(w) {}
		vec4(const float i)
			: vec4(i, i, i, i) {}
		vec4()
			: vec4(0) {}
		vec4(const vec4& i)
			: vec4(i.x, i.y, i.z, i.w) {}
		vec4(const vec3& i)
			: vec4(i.x, i.y, i.z, 0.f) {}

		float& operator[](size_t i)
		{
			assert(i < 4);
			return *(&x + i);
		}

		vec4 operator - () const { return { -x, -y, -z, -w }; }
		vec4 operator + (const vec4& i) const { return { x + i.x, y + i.y, z + i.z, w + i.w }; }
		vec4 operator - (const vec4& i) const { return { x - i.x, y - i.y, z - i.z, w - i.w }; }
		vec4 operator * (const vec4& i) const { return { x * i.x, y * i.y, z * i.z, w * i.w }; }
		vec4 operator * (const float i) const { return { x * i, y * i, z * i, w * i }; }
		vec4 operator / (const vec4& i) const { return { x / i.x, y / i.y, z / i.z, w / i.w }; }
		vec4 operator / (const float i) const { return { x / i, y / i, z / i, w / i }; }
		const vec4& operator += (const vec4& i) { return *this = *this + i; }
		const vec4& operator -= (const vec4& i) { return *this = *this - i; }
		const vec4& operator *= (const vec4& i) { return *this = *this * i; }
		const vec4& operator *= (const float i) { return *this = *this * i; }
		const vec4& operator /= (const vec4& i) { return *this = *this / i; }
		const vec4& operator /= (const float i) { return *this = *this / i; }

		[[nodiscard]] uint32_t get_argb() const
		{
			const float t_a = clamp<float>(w, 0, 1);
			const float t_r = clamp<float>(x, 0, 1);
			const float t_g = clamp<float>(y, 0, 1);
			const float t_b = clamp<float>(z, 0, 1);
			return static_cast<uint32_t>((static_cast<uint8_t>(t_a * 255) << 24) | (static_cast<uint8_t>(t_r * 255) << 16) | (static_cast<uint8_t>(t_g * 255) << 8) | (static_cast<uint8_t>(t_b * 255)));
		}

		[[nodiscard]] uint32_t get_rgba() const
		{
			const float t_a = clamp<float>(w, 0, 1);
			const float t_r = clamp<float>(x, 0, 1);
			const float t_g = clamp<float>(y, 0, 1);
			const float t_b = clamp<float>(z, 0, 1);
			return static_cast<uint32_t>((static_cast<uint8_t>(t_r * 255) << 24) | (static_cast<uint8_t>(t_g * 255) << 16) | (static_cast<uint8_t>(t_b * 255) << 8) | (static_cast<uint8_t>(t_a * 255)));
		}
	};

	inline float get_m128_together(__m128 i)
	{
		float result[4];
		_mm_store_ps(result, i);

		return result[0] + result[1] + result[2] + result[3];
	}

	struct mat3
	{
		vec3 x, y, z;

		mat3() {}

		mat3(float i) :
			x(i, 0, 0),
			y(0, i, 0),
			z(0, 0, i)
		{}

		mat3(vec3 x, vec3 y, vec3 z)
			: x(x), y(y), z(z) {}
		mat3(const mat3& i)
			: x(i.x), y(i.y), z(i.z) {}

		vec3& operator[](size_t i)
		{
			assert(i < 3);
			return *(&x + i);
		}

		mat3 operator*(mat3& i)
		{
			return
			{
				{x[0] * i.x[0] + x[1] * i.y[0] + x[2] * i.z[0]},
				{y[0] * i.x[1] + y[1] * i.y[1] + y[2] * i.z[1]},
				{z[0] * i.x[2] + z[1] * i.y[2] + z[2] * i.z[2]}
			};
		}
	};

	struct mat4
	{
		vec4 x, y, z, w;

		mat4() {}

		mat4(float i) :
		x(i, 0, 0, 0),
		y(0, i, 0, 0),
		z(0,0,i,0),
		w(0,0,0,i){}

		mat4(vec4 x, vec4 y, vec4 z, vec4 w)
			: x(x), y(y), z(z), w(w) {}
		mat4(const mat4& i)
			: x(i.x), y(i.y), z(i.z), w(i.w) {}

		mat4(const mat3& i)
			: x(i.x), y(i.y), z(i.z), w(0.f, 0.f, 0.f, 1.f) {}

		vec4& operator[](size_t i)
		{
			assert(i < 4);
			return *(&x + i);
		}


		mat4 operator*(const mat4& i) const
		{
			__m256 row01 = _mm256_set_ps(y.w, y.z, y.y, y.x, x.w, x.z, x.y, x.x);
			__m256 row23 = _mm256_set_ps(w.w, w.z, w.y, w.x, z.w, z.z, z.y, z.x);

			//store matrix i in columns 0123
			__m256 col01 = _mm256_set_ps(i.w.y, i.z.y, i.y.y, i.x.y, i.w.x, i.z.w, i.y.x, i.x.x);
			__m256 col23 = _mm256_set_ps(i.w.w, i.z.w, i.y.w, i.x.w, i.w.z, i.z.z, i.y.z, i.x.z);

			//also store matrix i in columns 1032
			__m256 col10 = _mm256_permute2f128_ps(col01, col01, 0x01);
			__m256 col32 = _mm256_permute2f128_ps(col23, col23, 0x01);

			__m256 index_0a5 = _mm256_mul_ps(row01, col01);
			__m256 index_3a6 = _mm256_mul_ps(row01, col32);

			__m256 index_9a12 = _mm256_mul_ps(row23, col10);
			__m256 index_8a13 = _mm256_mul_ps(row23, col01);

			__m256 index_2a7 = _mm256_mul_ps(row01, col23);
			__m256 index_1a4 = _mm256_mul_ps(row01, col10);

			__m256 index_11a14 = _mm256_mul_ps(row23, col32);
			__m256 index_10a15 = _mm256_mul_ps(row23, col23);

			mat4 returnMatrix(
				{
					get_m128_together(_mm256_extractf128_ps(index_0a5, 0)),
					get_m128_together(_mm256_extractf128_ps(index_1a4, 0)),
					get_m128_together(_mm256_extractf128_ps(index_2a7, 0)),
					get_m128_together(_mm256_extractf128_ps(index_3a6, 0)),
				},
				{
					get_m128_together(_mm256_extractf128_ps(index_1a4, 1)),
					get_m128_together(_mm256_extractf128_ps(index_0a5, 1)),
					get_m128_together(_mm256_extractf128_ps(index_3a6, 1)),
					get_m128_together(_mm256_extractf128_ps(index_2a7, 1)),
				},
				{
					get_m128_together(_mm256_extractf128_ps(index_8a13, 0)),
					get_m128_together(_mm256_extractf128_ps(index_9a12, 0)),
					get_m128_together(_mm256_extractf128_ps(index_10a15, 0)),
					get_m128_together(_mm256_extractf128_ps(index_11a14, 0)),
				},
				{
					get_m128_together(_mm256_extractf128_ps(index_9a12, 1)),
					get_m128_together(_mm256_extractf128_ps(index_8a13, 1)),
					get_m128_together(_mm256_extractf128_ps(index_11a14, 1)),
					get_m128_together(_mm256_extractf128_ps(index_10a15, 1)),
				});

			return returnMatrix;
		}

		const mat4& operator *= (const mat4& i) { return *this = *this * i; }

		std::string to_string()
		{
			return std::to_string(x.x) + " : " + std::to_string(x.y) + " : " + std::to_string(x.z) + " : " + std::to_string(x.w) + "\n" +
				std::to_string(y.x) + " : " + std::to_string(y.y) + " : " + std::to_string(y.z) + " : " + std::to_string(y.w) + "\n" +
				std::to_string(z.x) + " : " + std::to_string(z.y) + " : " + std::to_string(z.z) + " : " + std::to_string(z.w) + "\n" +
				std::to_string(w.x) + " : " + std::to_string(w.y) + " : " + std::to_string(w.z) + " : " + std::to_string(w.w) + "\n";
		}
	};

	struct quat
	{
		float x, y, z, w;

		quat() = default;
		quat(float x, float y, float z, float w)
			: x(x), y(y), z(z), w(w){}
		quat(float a_x, float a_y, float a_z)
		{
			float cy = std::cosf(a_z * 0.5f);
			float sy = std::sinf(a_z * 0.5f);
			float cp = std::cosf(a_y * 0.5f);
			float sp = std::sinf(a_y * 0.5f);
			float cr = std::cosf(a_x * 0.5f);
			float sr = std::sinf(a_x * 0.5f);

			w = cr * cp * cy + sr * sp * sy;
			x = sr * cp * cy - cr * sp * sy;
			y = cr * sp * cy + sr * cp * sy;
			z = cr * cp * sy - sr * sp * cy;
		}

		quat(const vec3& i)
			: quat(i.x, i.y, i.z) {}

		quat operator - () const { return { -x, -y, -z, -w }; }
		quat operator + (const quat& i) const { return { x + i.x, y + i.y, z + i.z, w + i.w }; }
		quat operator - (const quat& i) const { return { x - i.x, y - i.y, z - i.z, w - i.w }; }
		quat operator * (const float i) const { return { x * i, y * i, z * i, w * i }; }
		quat operator * (const quat& i) const { return
			{
				i.w * i.x + i.x * i.w + i.y * i.z - i.z * i.y,
				i.w * i.y + i.y * i.w + i.z * i.x - i.x * i.z,
				i.w * i.z + i.z * i.w + i.x * i.y - i.y * i.x,
				i.w * i.w - i.x * i.x - i.y * i.y - i.z * i.z
			};
		}
		const quat& operator += (const quat& i) { return *this = *this + i; }
		const quat& operator -= (const quat& i) { return *this = *this - i; }
		const quat& operator *= (const quat& i) { return *this = *this * i; }
		const quat& operator *= (const float i) { return *this = *this * i; }

		vec3 get_euler() const
		{
			vec3 angles;

			// yaw (z-axis rotation)
			{
				float siny_cosp = 2.f * (y * z + w * x);
				float cosy_cosp = w * w - x * x - y * y + z * z;

				angles.x = std::atan2(siny_cosp, cosy_cosp);
			}

			// pitch (y-axis rotation)
			{
				float sinp = 2.f * (w * y - z * x);
				if (std::abs(sinp) >= 1.f)
					angles.y = std::copysign(pi() / 2.f, sinp); // use 90 degrees if out of range
				else
					angles.y = std::asin(sinp);
			}

			// roll (x-axis rotation)
			{
				float t_y = 2.f * (x * y + w * z);
				float t_x = w * w + x * x - y * y - z * z;

				angles.z = std::atan2(t_y, t_x);
			}

			return angles;
		}
	};

	inline vec2 normalize(const vec2& i)
	{
		float mag = std::sqrtf(i.x * i.x + i.y * i.y);
		assert(mag != 0.f && "Try using normalize_safe instead");
		float inv = 1.f / mag;
		return { i.x * inv, i.y * inv };
	}


	inline vec2 normalize_safe(const vec2& i)
	{
		float mag = std::sqrtf(i.x * i.x + i.y * i.y);
		if (mag == 0.f)
			mag = 1.f;
		float inv = 1.f / mag;
		return { i.x * inv, i.y * inv };
	}

	inline vec3 normalize(const vec3& i)
	{
		float mag = std::sqrtf(i.x * i.x + i.y * i.y + i.z * i.z);
		assert(mag != 0.f && "Try using normalize_safe instead");
		float inv = 1.f / mag;
		return { i.x * inv, i.y * inv, i.z * inv };
	}

	inline vec3 normalize_safe(const vec3& i)
	{
		float mag = std::sqrtf(i.x * i.x + i.y * i.y + i.z * i.z);
		if (mag == 0.f)
			mag = 1.f;
		float inv = 1.f / mag;
		return { i.x * inv, i.y * inv, i.z * inv };
	}

	inline vec4 normalize(const vec4& i)
	{
		float mag = std::sqrtf(i.x * i.x + i.y * i.y + i.z * i.z);
		assert(mag != 0.f && "Try using normalize_safe instead");
		float inv = 1.f / mag;
		return { i.x * inv, i.y * inv, i.z * inv, i.w * inv };
	}

	inline vec4 normalize_safe(const vec4& i)
	{
		float mag = std::sqrtf(i.x * i.x + i.y * i.y + i.z * i.z);
		if (mag == 0.f)
			mag = 1.f;
		float inv = 1.f / mag;
		return { i.x * inv, i.y * inv, i.z * inv, i.w * inv };
	}

	inline mat4 translate(mat4 matrix, vec3 vector)
	{
		matrix.w = matrix.x * vector.x + matrix.y * vector.y + matrix.z * vector.z + matrix.w;
		return matrix;
	}

	inline mat4 scale(mat4 matrix, vec3 vector)
	{
		matrix[0] = matrix[0] * vector[0];
		matrix[1] = matrix[1] * vector[1];
		matrix[2] = matrix[2] * vector[2];
		return matrix;
	}

	inline mat4 rotate(mat4 m, float angle, vec3 v)
	{
		float const a = angle;
		float const c = cos(a);
		float const s = sin(a);

		vec3 axis(normalize(v));
		vec3 temp(axis * (1.f - c));

		mat4 temp_matrix;
		temp_matrix[0][0] = c + temp[0] * axis[0];
		temp_matrix[0][1] = temp[0] * axis[1] + s * axis[2];
		temp_matrix[0][2] = temp[0] * axis[2] - s * axis[1];

		temp_matrix[1][0] = temp[1] * axis[0] - s * axis[2];
		temp_matrix[1][1] = c + temp[1] * axis[1];
		temp_matrix[1][2] = temp[1] * axis[2] + s * axis[0];

		temp_matrix[2][0] = temp[2] * axis[0] + s * axis[1];
		temp_matrix[2][1] = temp[2] * axis[1] - s * axis[0];
		temp_matrix[2][2] = c + temp[2] * axis[2];

		mat4 result;
		result[0] = m[0] * temp_matrix[0][0] + m[1] * temp_matrix[0][1] + m[2] * temp_matrix[0][2];
		result[1] = m[0] * temp_matrix[1][0] + m[1] * temp_matrix[1][1] + m[2] * temp_matrix[1][2];
		result[2] = m[0] * temp_matrix[2][0] + m[1] * temp_matrix[2][1] + m[2] * temp_matrix[2][2];
		result[3] = m[3];
		return result;
	}

	inline mat4 get_perspective(float fovy, float aspect, float znear, float zfar)
	{
		assert(abs(aspect - std::numeric_limits<float>::epsilon()) > 0.f);

		float const tanHalfFovy = tan(fovy / 2.f);

		mat4 result(1.f);
		result[0][0] = 1.f / (aspect * tanHalfFovy);
		result[1][1] = 1.f / (tanHalfFovy);
		result[2][2] = -(zfar + znear) / (zfar - znear);
		result[2][3] = -1.f;
		result[3][2] = -2.f * zfar * znear / (zfar - znear);
		return result;
	}

	inline mat3 to_mat3(const quat& q)
	{
		mat3 result(1.f);
		float qxx(q.x * q.x);
		float qyy(q.y * q.y);
		float qzz(q.z * q.z);
		float qxz(q.x * q.z);
		float qxy(q.x * q.y);
		float qyz(q.y * q.z);
		float qwx(q.w * q.x);
		float qwy(q.w * q.y);
		float qwz(q.w * q.z);

		result[0][0] = 1.f - 2.f * (qyy + qzz);
		result[0][1] = 2.f * (qxy + qwz);
		result[0][2] = 2.f * (qxz - qwy);

		result[1][0] = 2.f * (qxy - qwz);
		result[1][1] = 1.f - 2.f * (qxx + qzz);
		result[1][2] = 2.f * (qyz + qwx);

		result[2][0] = 2.f * (qxz + qwy);
		result[2][1] = 2.f * (qyz - qwx);
		result[2][2] = 1.f - 2.f * (qxx + qyy);
		return result;
	}

	inline mat4 to_mat4(const quat& q)
	{
		return mat4(to_mat3(q));
	}

	inline quat normalize(const quat& q)
	{
		float n = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
		if (n <= 0.f)
			return { 0,0,0,0 };
		n = 1.f / n;
		return
		{
			q.x * n,
			q.y * n,
			q.z * n,
			q.w * n
		};
	}

	inline quat angle_axis(float angle, const vec3& i)
	{
		float a(angle);
		float s = std::sin(a * 0.5f);

		vec3 xyz = i * s;
		return { xyz.x, xyz.y, xyz.z,std::cos(a * 0.5f) };
	}
}

#pragma warning (pop)
