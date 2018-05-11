#ifndef _UNCOPYABLE_H
#define _UNCOPYABLE_H

///
//
//  Class:      Uncopyable
//  
//  Purpose:    Using inheritance, this will make a derived class unable to be copied 
//              (unallowed copy ctor/assignment).
//
///
class Uncopyable
{
private:
    Uncopyable(const Uncopyable&) = delete;
    Uncopyable& operator=(const Uncopyable&) = delete;

public:
    Uncopyable() = default;
    Uncopyable(Uncopyable&&) = default;
    ~Uncopyable() = default;

    Uncopyable& operator=(Uncopyable&&) = default;
};

#endif // _UNCOPYABLE_H