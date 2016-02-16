#include "MotorControlHelper.h"
#include "SettingsFile.h"

#include <CANTalon.h>
#include <Talon.h>
#include <CANJaguar.h>
#include <Jaguar.h>
#include <Victor.h>
#include <VictorSP.h>

SpeedController *MotorControlHelper::ReadSpeedController(SettingsFile &settings, const char *section, const char *motor_key_name)
{
	if (!section || !motor_key_name || !settings.HasSection(section))
		return 0;

	std::string motor_name;
	if (!settings.GetSetting(section, motor_key_name, motor_name))
		return 0;

	return ReadSpeedController(settings, motor_name.c_str());
}

SpeedController *MotorControlHelper::ReadSpeedController(SettingsFile &settings, const char *motor_name)
{
	if (!motor_name || !motor_name[0] || !settings.HasSection(motor_name))
		return 0;

	int port;
	bool invert;
	std::string type;
	settings.GetSetSetting(motor_name, "Port", port, 0);
	settings.GetSetSetting(motor_name, "Inverted", invert, false);
	settings.GetSetSetting(motor_name, "Type", type, "CANTalon");

	std::transform(type.begin(), type.end(), type.begin(), tolower);

	SpeedController *result = 0;
	if (type == "cantalon")
		result = new CANTalon(port);
	if (type == "talon")
		result = new Talon(port);
	if (type == "canjaguar")
		result = new CANJaguar(port);
	if (type == "jaguar")
		result = new Jaguar(port);
	if (type == "victor")
		result = new Victor(port);
	if (type == "victorsp")
		result = new VictorSP(port);

	if (result)
		result->SetInverted(invert);

	std::string motor = "Motor";
	std::string name_string = motor + "/" + motor_name;
	SmartDashboard::PutNumber((name_string + "/Port").c_str(), port);
	SmartDashboard::PutBoolean((name_string + "/Inverted"), invert);
	SmartDashboard::PutString((name_string + "/Type"), type.c_str());
	return result;
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


