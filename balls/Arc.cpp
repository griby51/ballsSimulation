#include "Arc.hpp"
#include <cmath>
#include "RandomUtils.hpp"

Arc::Arc(int cx, int cy, int r, int w, double angle_start, double angle_size, float speed)
	: centerX(cx), centerY(cy), radius(r), width(w), angleStart(angle_start), angleSize(angle_size), rotationSpeed(speed) {
}

void Arc::update() {
	angleStart -= rotationSpeed;
	if (angleStart >= 360) angleStart -= 360;
}

void Arc::draw(SDL_Renderer* renderer, SDL_Color color) {

	double start = fmod(360.0 - angleStart, 360.0);
	double end = fmod(start - angleSize, 360.0);

	thickArcRGBA(renderer,
		centerX, centerY,
		radius,
		start, end,
		color.r, color.g, color.b, color.a,
		width);
}

bool Arc::contains(int px, int py) {
	double dx = px - centerX;
	double dy = py - centerY;

	double dist = std::sqrt(dx * dx + dy * dy);
	/*if (dist < radius - width / 2.0 || dist > radius + width / 2) {
		return false;
	}*/

	double angle = std::atan2(dy, dx) * 180 / M_PI;
	if (angle < 0) angle += 360;

	double start = fmod(angleStart, 360);
	if (start < 0) start += 360;
	double end = fmod(angleStart + angleSize, 360);

	if (angleSize >= 360) {
		return true;
	}

	if (start < end) {
		return (angle >= start && angle <= end);
	}
	else {
		return (angle >= start || angle <= end);
	}

}

bool Arc::sameSide(double x1, double y1, double x2, double y2, double px, double py, double qx, double qy) const {
	double cross1 = (x2 - x1) * (py - y1) - (y2 - y1) * (px - x1);
	double cross2 = (x2 - x1) * (qy - y1) - (y2 - y1) * (qx - x1);
	return (cross1 * cross2 >= 0);
}

void Arc::setSpeed(float speed) {
	rotationSpeed = speed;
}

void Arc::setAngles(double angle_start, double angle_size) {
	angleStart = angle_start;
	angleSize = angle_size;
}

