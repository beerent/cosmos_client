//
//  Singleton.h
//  Projectios2
//
//  Created by Kamil Konecko on 6/25/15.
//
//

#ifndef Projectios2_Singleton_h
#define Projectios2_Singleton_h
template <class T> class Singleton
{
public:
    static T& instance()
    {
        static T instance;
        return instance;
    }
};
#endif
