#pragma once


template <typename T>
class Singleton
{
public:
    static T& GetInstance()
    {
        static T instance{};
        return instance;
    }

    Singleton(const Singleton& other) = delete;
    Singleton(Singleton&& other) = delete;
    Singleton& operator=(const Singleton& other) = delete;
    Singleton& operator=(Singleton&& other) = delete;

    virtual ~Singleton() = default;

private:


protected:
    Singleton() = default;


};

