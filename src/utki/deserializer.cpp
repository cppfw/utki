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

#include "deserializer.hpp"

#include "string.hpp"
#include "util.hpp"

using namespace utki;

std::string_view deserializer::read_string(size_t length)
{
	if (this->size() < length) {
		throw std::invalid_argument(
			utki::cat("deserializer::read_string(", length, "): buffer size is only ", this->size())
		);
	}

	auto ret = utki::make_string_view(this->data.subspan(0, length));
	this->data = this->data.subspan(length);
	return ret;
}

utki::span<const uint8_t> deserializer::read_span(size_t length)
{
	if (this->size() < length) {
		throw std::invalid_argument(
			utki::cat("deserializer::read_span(", length, "): buffer size is only ", this->size())
		);
	}

	auto ret = this->data.subspan(0, length);
	this->data = this->data.subspan(length);
	return ret;
}

uint8_t deserializer::read_uint8()
{
	if (this->size() < sizeof(uint8_t)) {
		throw std::invalid_argument("deserializer::read_uint8(): buffer has less bytes then needed");
	}

	auto ret = this->data.front();
	this->data = this->data.subspan(sizeof(uint8_t));
	return ret;
}

uint16_t deserializer::read_uint16_le()
{
	if (this->size() < sizeof(uint16_t)) {
		throw std::invalid_argument("deserializer::read_uint16_le(): buffer has less bytes then needed");
	}

	auto ret = utki::deserialize16le(this->data.data());
	this->data = this->data.subspan(sizeof(uint16_t));
	return ret;
}

uint32_t deserializer::read_uint32_le()
{
	if (this->size() < sizeof(uint32_t)) {
		throw std::invalid_argument("deserializer::read_uint32_le(): buffer has less bytes then needed");
	}

	auto ret = utki::deserialize32le(this->data.data());
	this->data = this->data.subspan(sizeof(uint32_t));
	return ret;
}

uint64_t deserializer::read_uint64_le()
{
	if (this->size() < sizeof(uint64_t)) {
		throw std::invalid_argument("deserializer::read_uint64_le(): buffer has less bytes then needed");
	}

	auto ret = utki::deserialize64le(this->data.data());
	this->data = this->data.subspan(sizeof(uint64_t));
	return ret;
}

float deserializer::read_float_le()
{
	if (this->size() < sizeof(float)) {
		throw std::invalid_argument("deserializer::read_float_le(): buffer has less bytes then needed");
	}

	auto ret = utki::deserialize_float_le(this->data.data());
	this->data = this->data.subspan(sizeof(float));
	return ret;
}

uint16_t deserializer::read_uint16_be()
{
	if (this->size() < sizeof(uint16_t)) {
		throw std::invalid_argument("deserializer::read_uint16_be(): buffer has less bytes then needed");
	}

	auto ret = utki::deserialize16be(this->data.data());
	this->data = this->data.subspan(sizeof(uint16_t));
	return ret;
}

uint32_t deserializer::read_uint32_be()
{
	if (this->size() < sizeof(uint32_t)) {
		throw std::invalid_argument("deserializer::read_uint32_be(): buffer has less bytes then needed");
	}

	auto ret = utki::deserialize32be(this->data.data());
	this->data = this->data.subspan(sizeof(uint32_t));
	return ret;
}

uint64_t deserializer::read_uint64_be()
{
	if (this->size() < sizeof(uint64_t)) {
		throw std::invalid_argument("deserializer::read_uint64_be(): buffer has less bytes then needed");
	}

	auto ret = utki::deserialize64be(this->data.data());
	this->data = this->data.subspan(sizeof(uint64_t));
	return ret;
}

float deserializer::read_float_be()
{
	if (this->size() < sizeof(float)) {
		throw std::invalid_argument("deserializer::read_float_be(): buffer has less bytes then needed");
	}

	auto ret = utki::deserialize_float_be(this->data.data());
	this->data = this->data.subspan(sizeof(float));
	return ret;
}

void deserializer::skip(size_t length)
{
	if (this->size() < length) {
		throw std::invalid_argument(utki::cat("deserializer::skip(", length, "): buffer size is only ", this->size()));
	}

	this->data = this->data.subspan(length);
}
