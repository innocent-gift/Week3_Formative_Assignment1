#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <math.h>

static PyObject* analyze_dataset(PyObject *self, PyObject *args) {
    PyObject *seq;
    double threshold;

    if (!PyArg_ParseTuple(args, "Od", &seq, &threshold)) {
        return NULL;
    }

    PyObject *fast_seq = PySequence_Fast(seq, "Argument must be a sequence of numbers");
    if (!fast_seq) {
        return NULL;
    }

    Py_ssize_t n = PySequence_Fast_GET_SIZE(fast_seq);
    if (n == 0) {
        Py_DECREF(fast_seq);
        PyErr_SetString(PyExc_ValueError, "Dataset cannot be empty");
        return NULL;
    }

    PyObject **items = PySequence_Fast_ITEMS(fast_seq);
    double sum = 0.0;
    double min_val = INFINITY;
    double max_val = -INFINITY;

    double *values = malloc(n * sizeof(double));
    if (!values) {
        Py_DECREF(fast_seq);
        return PyErr_NoMemory();
    }

    for (Py_ssize_t i = 0; i < n; i++) {
        double val = PyFloat_AsDouble(items[i]);
        if (PyErr_Occurred()) {
            free(values);
            Py_DECREF(fast_seq);
            return NULL;
        }
        values[i] = val;
        sum += val;
        if (val < min_val) min_val = val;
        if (val > max_val) max_val = val;
    }

    double mean = sum / (double)n;
    double var_sum = 0.0;
    long out_of_bounds = 0;

    for (Py_ssize_t i = 0; i < n; i++) {
        double diff = values[i] - mean;
        var_sum += diff * diff;
        if (fabs(diff) > threshold) {
            out_of_bounds++;
        }
    }
    double variance = var_sum / (double)n;
    double span = max_val - min_val;

    PyObject *norm_list = PyList_New(n);
    for (Py_ssize_t i = 0; i < n; i++) {
        double norm = (span == 0.0) ? 0.0 : (values[i] - min_val) / span;
        PyList_SetItem(norm_list, i, PyFloat_FromDouble(norm));
    }

    free(values);
    Py_DECREF(fast_seq);

    return Py_BuildValue("{s:d, s:d, s:d, s:d, s:l, s:O}",
                         "mean", mean,
                         "variance", variance,
                         "min", min_val,
                         "max", max_val,
                         "out_of_bounds", out_of_bounds,
                         "normalized", norm_list);
}

static PyMethodDef AnalysisMethods[] = {
    {"analyze_dataset", analyze_dataset, METH_VARARGS, "Perform high-performance data analysis."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef analysismodule = {
    PyModuleDef_HEAD_INIT,
    "analysis_module",
    "C extension for data analysis",
    -1,
    AnalysisMethods
};

PyMODINIT_FUNC PyInit_analysis_module(void) {
    return PyModule_Create(&analysismodule);
}
