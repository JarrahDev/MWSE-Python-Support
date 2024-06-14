#include "/usr/include/python3.11/Python.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Initialize the Python interpreter
void initializePython() {
    Py_Initialize();
    if (!Py_IsInitialized()) {
        std::cerr << "Python initialization failed!" << std::endl;
        exit(1);
    }
}

// Finalize the Python interpreter
void finalizePython() {
    Py_Finalize();
}

// Run Python script from a file
void runPythonScriptFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open Python script file: " << filename << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string script = buffer.str();
    
    // Run the script with error handling
    int result = PyRun_SimpleString(script.c_str());
    if (result != 0) {
        PyErr_Print();
        std::cerr << "Error executing Python script file: " << filename << std::endl;
    }
}

// Convert C++ string to Python string object
PyObject* toPythonObject(const std::string& arg) {
    return PyUnicode_FromString(arg.c_str());
}

// Convert C++ int to Python int object
PyObject* toPythonObject(int arg) {
    return PyLong_FromLong(arg);
}

// Convert C++ double to Python float object
PyObject* toPythonObject(double arg) {
    return PyFloat_FromDouble(arg);
}

// General function to convert any type to Python object
template<typename T>
PyObject* toPythonObject(T arg) {
    // Add additional specializations for different types as needed
    return Py_None; // This should be specialized or extended for different types
}

// Call a Python function from a module with a vector of arguments and return a string
std::string callPythonFunctionWithArgs(const std::string& moduleName, const std::string& functionName, const std::vector<PyObject*>& args) {
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;

    // Add the current directory to the Python path
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");

    pName = PyUnicode_DecodeFSDefault(moduleName.c_str());
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    std::string resultStr;

    if (pModule != nullptr) {
        pFunc = PyObject_GetAttrString(pModule, functionName.c_str());
        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(args.size());
            for (size_t i = 0; i < args.size(); ++i) {
                PyTuple_SetItem(pArgs, i, args[i]); // PyTuple_SetItem steals a reference to args[i]
            }

            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);

            if (pValue != nullptr) {
                if (PyUnicode_Check(pValue)) {
                    resultStr = PyUnicode_AsUTF8(pValue);
                } else if (PyLong_Check(pValue)) {
                    resultStr = std::to_string(PyLong_AsLong(pValue));
                } else if (PyFloat_Check(pValue)) {
                    resultStr = std::to_string(PyFloat_AsDouble(pValue));
                } else {
                    resultStr = "Unsupported return type";
                }
                Py_DECREF(pValue);
            } else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                std::cerr << "Call failed" << std::endl;
                return "";
            }
        } else {
            if (PyErr_Occurred()) {
                PyErr_Print();
            }
            std::cerr << "Cannot find function " << functionName << std::endl;
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        std::cerr << "Failed to load " << moduleName << std::endl;
    }

    return resultStr;
}

int main() {
    // Initialize the Python interpreter
    initializePython();

    // Prompt user for Python script file
    std::cout << "Enter the path to the Python script file: ";
    std::string scriptFilename;
    std::getline(std::cin, scriptFilename);

    // Run the Python script file
    runPythonScriptFile(scriptFilename);

    // Finalize the Python interpreter
    finalizePython();

    std::cout << "Python script executed successfully." << std::endl;
    return 0;
}
