#pragma once

namespace utki{

/**
 * @brief Destructable class.
 * This class is to be used as a bse class for the cases when void* is needed, but the ownership of the passed in
 * pointer is transferred, so that the object pointed to by the pointer could be destroyed correctly.
 */
class destructable{
public:
    virtual ~destructable()noexcept{}
};

}
