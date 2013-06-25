#include <Python.h>
#include <err.h>

#include "aps.h"

int g_dev;

static PyObject *aps_get_accel(PyObject *, PyObject *);

static PyMethodDef ApsMethods[] = {
	{"get_accel", aps_get_accel, METH_VARARGS,
	 "Get accelerometer data."},
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initaps(void)
{
	PyObject *m;

	g_dev = get_aps_sensor_index();
	if (g_dev < 0)
		err(1, "get_sensor_index");

	m = Py_InitModule("aps", ApsMethods);
	if (m == NULL)
		return;
}

static PyObject *
aps_get_accel(PyObject *self, PyObject *args)
{
	int x, y, r;
	r = get_accel(g_dev, &x, &y);
	if (r == -1) {
		err(1, "get_accel");
	}
	return Py_BuildValue("ii", x, y);
}
