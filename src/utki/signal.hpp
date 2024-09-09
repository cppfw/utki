/*
The MIT License (MIT)

utki - Utility Kit for C++.

Copyright (c) 2015-2024 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* ================ LICENSE END ================ */

#pragma once

#include <functional>
#include <list>

namespace utki {

/**
 * @brief Signal-slot like callback calling utility.
 * Allows connecting many callbacks and call all the connected callbacks with given arguments.
 * @tparam args_type - callback argument types pack.
 */
template <typename... args_type>
class signal
{
public:
	/**
	 * @brief The callback type name.
	 */
	using callback_type = std::function<void(args_type...)>;

private:
	std::list<callback_type> callbacks;

public:
	/**
	 * @brief Connection ID.
	 * The type name of the ID identifying the connection of a callback to the signal.
	 * The ID can be used to later disconnect the callback from the signal.
	 */
	using connection = typename decltype(callbacks)::iterator;

	/**
	 * @brief Connect a callback to the signal.
	 * @param callback - callback to connect.
	 * @return Connection ID.
	 */
	connection connect(callback_type callback)
	{
		this->callbacks.push_back(std::move(callback));
		return std::prev(this->callbacks.end());
	}

	/**
	 * @brief Disconnect a callback from the signal.
	 * @param conn - connection id of the callback to disconnect.
	 */
	void disconnect(connection conn) noexcept
	{
		this->callbacks.erase(conn);
	}

	/**
	 * @brief Emit the signal.
	 * Calls all the connected callbacks with given arguments.
	 * @param a - arguments pack to pass to each of the connected callbacks.
	 */
	void emit(args_type... a)
	{
		for (const auto& c : this->callbacks) {
			c(std::forward<args_type>(a)...);
		}
	}

	/**
	 * @brief Get number of callbacks connected.
	 * @return Number of callbacks connected.
	 */
	size_t size() const noexcept
	{
		return this->callbacks.size();
	}

	/**
	 * @brief Check if no callbacks are connected to the signal.
	 * @return true if no callbacks are connected to the signal.
	 * @return false otherwise.
	 */
	bool empty() const noexcept
	{
		return this->size() == 0;
	}
};

} // namespace utki
