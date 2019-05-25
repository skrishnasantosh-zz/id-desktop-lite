#pragma once


#define BEGIN_NAMESPACE1(X) namespace X{
#define END_NAMESPACE1() }
#define BEGIN_NAMESPACE2(X,Y) namespace X { namespace Y {
#define END_NAMESPACE2() } }
#define BEGIN_NAMESPACE3(X,Y,Z) namespace X { namespace Y { namespace Z {
#define END_NAMESPACE3() } } }
#define BEGIN_NAMESPACE4(A,B,C,D) namespace A { namespace B { namespace C { namespace D {
#define END_NAMESPACE4() } } } }

BEGIN_NAMESPACE3(Autodesk, Identity, Client)
END_NAMESPACE3()

#ifdef _WIN32
#ifdef _WINDLL
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#endif