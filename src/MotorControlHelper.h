#ifndef MOTORCONTROLHELPER_H
#define MOTORCONTROLHELPER_H

#include <map>

class SettingsFile;
class SpeedController;

class MotorControlHelper
{
public:
	static SpeedController *ReadSpeedController(SettingsFile &settings, const char *motor_name);
	static SpeedController *ReadSpeedController(SettingsFile &settings, const char *section, const char *motor_key_name);

	static float ScaleSpeed(float value, std::map<float, float> &curve_points);
	static float Limit(float value);
	static float Limit(float value, float min, float max);
	static float LimitAcceleration(float cur_value, float requested_value, float max_accel);
	static float LimitDeceleration(float cur_value, float requested_value, float max_decel);
	static float LimitForwardAcceleration(float cur_value, float requested_value, float max_accel);
	static float LimitReverseAcceleration(float cur_value, float requested_value, float max_accel);
	static float LimitAccelerationDeceleration(float cur_value, float requested_value, float max_accel, float max_decel);
};

#endif
