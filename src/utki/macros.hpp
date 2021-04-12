#pragma once

/**
 * @brief Utility macro for defining macro overloads.
 * Allows defining macro overloads for different number of macro arguments.
 */
#define UTKI_GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME
