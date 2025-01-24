//
// Created by ameer on 1/4/2023.
//

#ifndef UNTITLED2_ARRAYHASH_H
#define UNTITLED2_ARRAYHASH_H
#include <iostream>
#define INITIAL_SIZE 5
#define FACTOR 2

#include <memory>

using std::shared_ptr;

template<class T>
class ArrayHash{
public:
    T* data;
    int size;

    ArrayHash();
    explicit ArrayHash(int size);
    ArrayHash(const ArrayHash& array);
    ~ArrayHash();
    ArrayHash& operator=(const ArrayHash& array);
    T& operator[](int index);
    const T& operator[](int index) const;
    bool Extend();
};

template <class T>
ArrayHash<T>::ArrayHash(): data(new T[INITIAL_SIZE]) , size(INITIAL_SIZE)
{
}

template<class T>
ArrayHash<T>::ArrayHash(int size): data(new T[size]), size(size)
{
}

template <class T>
ArrayHash<T>::ArrayHash(const ArrayHash<T>& array): data(new T[array.size]), size(array.size)
{
    try
    {
        int i = 0 ;
        while(i < size)
        {
            data[i]=array[i];
            i++;
        }
    }
    catch(...)
    {
        delete[] data;
        throw;
    }
}

template <class T>
ArrayHash<T>::~ArrayHash()
{
    if (data != nullptr)
    {
        delete[] data;
    }
}

template<class T>
ArrayHash<T>& ArrayHash<T>::operator=(const ArrayHash<T>& array)
{
    if(&array == this){
        return *this;
    }

    T* temp_data=new T[array.size];
    try{
        int i = 0 ;
        while (i < array.size)
        {
            temp_data[i]=array[i];
            i++;
        }
    }
    catch(...){
        delete[] temp_data;
        throw;
    }

    delete[] data;
    data=temp_data;
    size= array.size;
    return *this;
}

template <class T>
T& ArrayHash<T>::operator[](int index)
{
    return data[index];
}

template <class T>
const T& ArrayHash<T>::operator[](int index) const
{
    return data[index];
}


template <class T>
bool ArrayHash<T>::Extend()
{
    T* tmp;
    try
    {
        tmp = new T[size*FACTOR];
    }
    catch(const std::exception& e)
    {
        return false;
    }
    int i = 0;
    while( i < size)
    {
        tmp[i]=data[i];
        i++;
    }
    delete[] data;
    data=tmp;
    size*=FACTOR;
    return true;
}



#endif //UNTITLED2_ARRAYHASH_H
