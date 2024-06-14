#include "/usr/include/python3.11/Python.h"
#include <iostream>

void initializePython() {
    Py_Initialize();
}

void finalizePython() {
    Py_Finalize();
}

void runPythonScript(const std::string& script) {
    PyRun_SimpleString(script.c_str());
}

int main() {
    // Initialize the Python interpreter
    initializePython();

    // Run a simple Python script
    runPythonScript("print('Hello from Python!')");

    // Finalize the Python interpreter
    finalizePython();

    std::cout << "Python script executed successfully." << std::endl;
    return 0;
}
