#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/sensors.h>
#include <errno.h>

#include "aps.h"


static const char *aps_devname = "aps0";


/*
 * raw0: X_ACCEL
 * raw1: Y_ACCEL
 */
int
get_accel(int dev, int *x_return, int *y_return)
{
	int mib[5];
	struct sensor sens;
	size_t senslen = sizeof(sens);
	int x, y;

	mib[0] = CTL_HW;
	mib[1] = HW_SENSORS;
	mib[2] = dev;
	mib[3] = SENSOR_INTEGER;
	mib[4] = 0;
	if (sysctl(mib, 5, &sens, &senslen, NULL, 0) == -1) {
		return -1;
	}
	x = sens.value;

	mib[4] = 1;
	if (sysctl(mib, 5, &sens, &senslen, NULL, 0) == -1) {
		return -1;
	}
	y = sens.value;

	if (x_return)
		*x_return = x;
	if (y_return)
		*y_return = y;
	return 1;
}

int
get_aps_sensor_index(void)
{
	int dev;
	int mib[3];
	struct sensordev sd;
	size_t sdlen = sizeof(sd);

	mib[0] = CTL_HW;
	mib[1] = HW_SENSORS;
	for (dev = 0; ; dev++) {
		mib[2] = dev;
		if (sysctl(mib, 3, &sd, &sdlen, NULL, 0) == -1) {
			if (errno == ENXIO)
				continue;
			if (errno == ENOENT)
				break;
		}
		if (strcmp(aps_devname, sd.xname) == 0)
			break;
	}
	if (strcmp(aps_devname, sd.xname) != 0) {
		return -1;
	}
	return dev;
}
