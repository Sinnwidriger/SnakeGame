#ifndef SNAKE_GAME_SHARED_SYSTEM_INFORMATION_H_
#define SNAKE_GAME_SHARED_SYSTEM_INFORMATION_H_

namespace shared
{

class SystemInformation
{
public:
	SystemInformation() = default;
	SystemInformation(int index, std::wstring name, std::wstring description);

	inline int index() const { return index_; }
	inline std::wstring name() const { return name_; }
	inline std::wstring description() const { return description_; }
	inline std::wstring numeric_value() const { return numeric_value_; }

protected:
	int index_;
	std::wstring name_;
	std::wstring description_;
	std::wstring numeric_value_;
};

}

#endif