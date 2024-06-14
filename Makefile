CXX = g++
PYTHON_VERSION = 3.11
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
PYTHON_LIB = /usr/lib/x86_64-linux-gnu/libpython$(PYTHON_VERSION).so
CXXFLAGS = -I$(PYTHON_INCLUDE)
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lpython$(PYTHON_VERSION)
TARGET = mypythonapp
SRCS = main.cpp

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
