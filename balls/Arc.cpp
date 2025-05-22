#include "Arc.hpp"
#include <cmath>
#include "RandomUtils.hpp"

Arc::Arc(int cx, int cy, int r, int w, double angle_start, double angle_size, float speed)
	: centerX(cx), centerY(cy), radius(r), width(w), angleStart(angle_start), angleEnd(angle_start + angle_size), angleSize(angle_size), rotationSpeed(speed) {
}

void Arc::update() {
	angleStart += rotationSpeed;
	angleEnd += rotationSpeed;

	if (angleStart < 0) angleStart += 360;
	if (angleStart >= 360) angleStart -= 360;
}

void Arc::draw(SDL_Renderer* renderer, SDL_Color color) {
	double start_deg = fmod(angleStart + angleSize, 360.0);
	double end_deg = fmod(angleStart, 360.0);

	Sint16 middle_radius = static_cast<Sint16>(radius + width / 2);

	thickArcRGBA(renderer, centerX, centerY, middle_radius, static_cast<Sint16>(start_deg), static_cast<Sint16>(end_deg),
		color.r, color.g, color.b, color.a, width);
}

bool Arc::contains(int px, int py) {
	double theta0 = angleStart * M_PI / 180.0;
	double theta1 = theta0 + (angleSize * M_PI / 180.0);
	double Ax = centerX, Ay = centerY;
	double Bx = Ax + (radius + 1000) * cos(theta0), By = Ay + (radius + 1000) * sin(theta0);
	double Cx = Ax + (radius + 1000) * cos(theta1), Cy = Ay + (radius + 1000) * sin(theta1);
	return sameSide(Ax, Ay, Bx, By, px, py, Cx, Cy) &&
		sameSide(Bx, By, Cx, Cy, px, py, Ax, Ay) &&
		sameSide(Cx, Cy, Ax, Ay, px, py, Bx, By);
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

