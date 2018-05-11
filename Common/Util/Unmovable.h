#ifndef _UNMOVABLE_H
#define _UNMOVABLE_H

///
//
//  Class:      Unmovable
//  
//  Purpose:    Using inheritance, this will make a derived class unable to be moved
//              (unallowed move ctor/assignment)
//
///
class Unmovable
{
private:
    Unmovable(Unmovable&&)              = delete;
    Unmovable& operator=(Unmovable&&)   = delete;

public:
    Unmovable() = default;
    Unmovable(const Unmovable&) = default;
	~Unmovable() = default;

    Unmovable& operator=(const Unmovable&) = default;
};

#endif // _UNMOVABLE_H