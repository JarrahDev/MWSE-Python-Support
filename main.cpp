#include "/usr/include/python3.11/Python.h"
#include <iostream>
#include <fstream>
#include <sstream>

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

// Run Python code from a string
void runPythonScript(const std::string& script) {
    int result = PyRun_SimpleString(script.c_str());
    if (result != 0) {
        std::cerr << "Error executing Python script: " << script << std::endl;
    }
}

// Run Python code from a file
void runPythonScriptFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open Python script file: " << filename << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string script = buffer.str();
    
    int result = PyRun_SimpleString(script.c_str());
    if (result != 0) {
        std::cerr << "Error executing Python script file: " << filename << std::endl;
    }
}

// Call a Python function from a module
void callPythonFunction(const std::string& moduleName, const std::string& functionName) {
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;

    // Add the current directory to the Python path
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");

    pName = PyUnicode_DecodeFSDefault(moduleName.c_str());
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != nullptr) {
        pFunc = PyObject_GetAttrString(pModule, functionName.c_str());
        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(0); // Adjust for function arguments if needed
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);

            if (pValue != nullptr) {
                std::cout << "Result of call: " << PyLong_AsLong(pValue) << std::endl;
                Py_DECREF(pValue);
            } else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                std::cerr << "Call failed" << std::endl;
                return;
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
}

int main() {
    // Initialize the Python interpreter
    initializePython();

    // Run a simple Python script
    runPythonScript("print('Hello from Python!')");

    // Run a more complex Python script from a string
    runPythonScript("def greet(name):\n"
                    "    return f'Hello, {name}!'\n"
                    "print(greet('Morrowind'))");

    // Run a Python script from a file
    runPythonScriptFile("script.py");

    // Call a function from a Python module
    callPythonFunction("mymodule", "myfunction");

    // Finalize the Python interpreter
    finalizePython();

    std::cout << "Python script executed successfully." << std::endl;
    return 0;
}
