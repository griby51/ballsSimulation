#include "Arc.hpp"
#include <cmath>

Arc::Arc(int cx, int cy, int r, int w, double angle_start, double angle_size, float speed)
	: centerX(cx), centerY(cy), radius(r), width(w), angleStart(angle_start), angleSize(angle_size), rotationSpeed(speed) {
}

void Arc::update() {
	angleStart += rotationSpeed;
	if (angleStart >= 360) angleStart -= 360;
}

void Arc::draw(SDL_Renderer* renderer, SDL_Color color) {
	const int teethLength = 1000;

	double theta0 = angleStart * M_PI / 180.0;
	double theta1 = theta0 + (angleSize * M_PI / 180.0);

	Sint16 vx[3] = {
		(Sint16)centerX,
		(Sint16)(centerX + (radius + teethLength) * cos(theta0)),
		(Sint16)(centerX + (radius + teethLength) * cos(theta1))
	};

	Sint16 vy[3] = {
		(Sint16)centerY,
		(Sint16)(centerY + (radius + teethLength) * sin(theta0)),
		(Sint16)(centerY + (radius + teethLength) * sin(theta1))
	};

	filledCircleRGBA(renderer, centerX, centerY, radius + width, color.r, color.g, color.b, color.a);
	filledCircleRGBA(renderer, centerX, centerY, radius, 0, 0, 0, 255);

	filledPolygonRGBA(renderer, vx, vy, 3, 0, 0, 0, 255);
}

bool Arc::contains(int px, int py) const {
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