#pragma once

#include <cmath>
#include <ostream>

class Vector2D {
private:
	float x_;
	float y_;

public:
	Vector2D() noexcept : x_(), y_() {}
	Vector2D(const Vector2D& v) : x_(v.getX()), y_(v.getY()) { }
	Vector2D(Vector2D&& v) noexcept : x_(v.getX()), y_(v.getY()) { }
	Vector2D(const Vector2D* v) : x_(v->getX()), y_(v->getY()) { }
	Vector2D(float x, float y) : x_(x), y_(y) { }
	~Vector2D() { }

	// Getters
	inline float getX() const { return x_; }
	inline float getY() const { return y_; }

	// Setters
	inline void setX(float x) { x_ = x; }
	inline void setY(float y) { y_ = y; }
	inline void set(float x, float y) {
		x_ = x;
		y_ = y;
	}
	inline void set(const Vector2D& v) {
		x_ = v.x_;
		y_ = v.y_;
	}
	inline void set(const Vector2D&& v) {
		x_ = v.x_;
		y_ = v.y_;
	}
	inline void set(const Vector2D* v) {
		x_ = v->x_;
		y_ = v->y_;
	}

	// Copy
	inline Vector2D& operator=(const Vector2D& v) {
		x_ = v.x_;
		y_ = v.y_;
		return *this;
	}

	// Length
	inline float magnitude() const { return sqrtf(powf(x_, 2) + powf(y_, 2)); }
	// Normalize
	inline Vector2D normalize() const { return *this / magnitude(); }
	Vector2D rotate(float degrees) const;
	float angle(const Vector2D& v) const;

	inline Vector2D operator-(const Vector2D& v) const { return Vector2D(x_ - v.x_, y_ - v.y_); }
	inline Vector2D operator+(const Vector2D& v) const { return Vector2D(x_ + v.x_, y_ + v.y_); }
	inline Vector2D operator*(float k) const { return Vector2D(x_ * k, y_ * k); }
	inline Vector2D operator/(float k) const { return Vector2D(x_ / k, y_ / k); }
	inline float operator*(const Vector2D& v) const { return v.x_ * x_ + v.y_ * y_; }
};


// To print the vector values
std::ostream& operator<<(std::ostream& os, const Vector2D& v);
