#ifndef TEMPLATE_H_INCLUDED
#define TEMPLATE_H_INCLUDED

#include <iostream>

template <typename T>
class myTemplate
{
  private:
  T data;

  public:
  myTemplate();
  myTemplate(T t);
  T getData() const;
  void displayData() const;

  static int someValue;
};

template<typename T>
myTemplate<T>::myTemplate()
:data()
{
}

template<typename T>
myTemplate<T>::myTemplate(T t)
:data(t)
{
}

template <typename T>
T myTemplate<T>::getData() const
{
    return data;
}

template <typename T>
void myTemplate<T>::displayData() const
{
    std::cout << data <<std::endl;
}


template<typename T>
int myTemplate<T>::someValue = 100;


#endif // TEMPLATE_H_INCLUDED
