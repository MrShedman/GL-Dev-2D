#pragma once

#include <vector>
#include <string>
#include <istream>
#include <ostream>
#include <fstream>

class BinaryStreamRead
{
public:

	BinaryStreamRead(const std::string& filename)
		:
		in(filename, std::ifstream::in | std::ifstream::binary)
	{
	}

	~BinaryStreamRead()
	{
		in.close();
	}

	template<typename T>
	T read()
	{
		T value;
		in.read(reinterpret_cast<char*>(&value), sizeof(T));
		return value;
	}

	void advance(size_t length)
	{
		for (size_t i = 0; i < length; ++i)
		{
			char temp;
			in.read(&temp, length);
		}
	}

	template<typename T>
	std::vector<T> readVector(size_t length)
	{
		std::vector<T> temp;
		temp.resize(length);

		for (size_t i = 0; i < length; ++i)
		{
			in.read(&temp[i], sizeof(T));
		}

		return temp;
	}

	std::string readString(size_t length)
	{
		std::string temp;
		temp.resize(length);

		for (size_t i = 0; i < length; ++i)
		{
			in.read(&temp[i], sizeof(char));
		}

		return temp;
	}


private:

	std::ifstream in;
};

class BinaryStreamWrite
{
public:
	BinaryStreamWrite(const std::string& filename)
		:
		out(filename, std::ifstream::out | std::ifstream::binary)
	{
	}

	~BinaryStreamWrite()
	{
		out.close();
	}

	template<typename T>
	void write(const T& value)
	{
		out.write(reinterpret_cast<const char*>(&value), sizeof(T));
	}

	template<typename T>
	void writeVector(const std::vector<T>& value)
	{
		out.write(reinterpret_cast<const char*>(&value[0]), value.size() * sizeof(T));
	}

	void writeString(const std::string& value)
	{
		out.write(value.c_str(), value.length());
	}

	void newLine()
	{
		out << std::endl;
	}

private:

	std::ofstream out;
};