#include "MotorControlHelper.h"
#include "SettingsFile.h"

#include <CANTalon.h>
#include <Talon.h>

SpeedController *MotorControlHelper::ReadSpeedController(SettingsFile &settings, const char *motor_name)
{
	return 0;
}

float MotorControlHelper::ScaleSpeed(float value, std::map<float, float> &curve_points)
{
	return value;
}

float MotorControlHelper::Limit(float value)
{
	return Limit(value, -1.0f, 1.0f);
}

float MotorControlHelper::Limit(float value, float min, float max)
{
	if (value > max)
		return max;
	if (value < min)
		return min;
	return value;
}

float MotorControlHelper::LimitAcceleration(float cur_value, float requested_value, float max_accel)
{
	return LimitAccelerationDeceleration(cur_value, requested_value, max_accel, 1.0f);
}

float MotorControlHelper::LimitDeceleration(float cur_value, float requested_value, float max_decel)
{
	return LimitAccelerationDeceleration(cur_value, requested_value, 1.0f, max_decel);
}

float MotorControlHelper::LimitForwardAcceleration(float cur_value, float requested_value, float max_accel)
{
	float delta = requested_value - cur_value;
	if (delta > 0 && delta > max_accel)
		return cur_value + max_accel;
	return requested_value;
}

float MotorControlHelper::LimitReverseAcceleration(float cur_value, float requested_value, float max_accel)
{
	float delta = requested_value - cur_value;
	if (delta < 0 && delta < -max_accel)
		return cur_value - max_accel;
	return requested_value;
}

float MotorControlHelper::LimitAccelerationDeceleration(float cur_value, float requested_value, float max_accel, float max_decel)
{
	float delta = requested_value - cur_value;

	// Always have to decelerate to 0 (or really close to it), and then accelerate
	if (max_decel > std::abs(cur_value))
		max_decel = std::max(std::abs(cur_value), max_accel);

	if (cur_value < 0)
	{
		if (delta < 0)
		{
			// Accelerating (going faster in reverse)
			if (delta < -max_accel)
				return cur_value - max_accel;
		}
		else
		{
			// Decelerating (going slower in reverse)
			if (delta > max_decel)
				return cur_value + max_decel;
		}
	}
	else
	{
		if (delta > 0)
		{
			// Accelerating, (going faster forward)
			if (delta > max_accel)
				return cur_value + max_accel;
		}
		else
		{
			if (delta < -max_decel)
				return cur_value - max_decel;
		}
	}

	// Within the limits, so just return requested value
	return requested_value;
}


