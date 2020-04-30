#Compile: g++ `Magick++-config --cxxflags --cppflags` -o newmain pixel.h pixel.cpp imageutil.h imageutil.cpp reflect.h reflect.cpp blur.h blur.cpp contrast.h contrast.cpp brightness.h brightness.cpp edge.h edge.cpp art.h art.cpp main.cpp `Magick++-config --ldflags --libs`

CXX=g++
RM=rm
CXXFLAGS1 += `Magick++-config --cxxflags --cppflags`
CXXFLAGS2 += `Magick++-config --ldflags --libs`
OBJS=pixel.h pixel.cpp imageutil.h imageutil.cpp reflect.h reflect.cpp blur.h blur.cpp contrast.h contrast.cpp brightness.h brightness.cpp edge.h edge.cpp art.h art.cpp subtraction.h subtraction.cpp sharpen.h sharpen.cpp oil.h oil.cpp main.cpp

all: newmain

makemain: $(OBJS)
	$(CXX) $(CXXFLAGS1) -o newmain $(OBJS) $(CXXFLAGS2)

