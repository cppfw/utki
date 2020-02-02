# Introduction

It is often required to create singleton objects in the program, i.e. use the singleton pattern.

**utki** provides a template for quick creation of singleton classes.



## Header file
In order to use `utki::singleton` one needs to include the following header file:
```cpp
#include <utki/singleton.hpp>
```

## Usage
The usage is simple:
```cpp
class MySingletonClass : public utki::singleton<MySingletonClass>{
public:
    int a;
    int b;
};

int main(int argc, char** argv){
    // create singleton object
    MySingletonClass mySingletonObject;

    // NOTE: since this is a singleton class, creating other
    //       instances of this class will cause an exception to be thrown.

    // access the members of singleton object
    MySingletonClass::inst().a = 355;
    MySingletonClass::inst().b = 13;
    
    int k = MySingletonClass::inst().b;
}
```

Note, that we create a singleton object inside of the main() function. Though it is not forbidden to create singleton objects in a global scope, it is not recommended and considered a bad practice. This is because when defining objects in global scope and in different files their order of construction is undetermined. Creating objects inside of the main() function allows you to strictly define the order of objects construction.
