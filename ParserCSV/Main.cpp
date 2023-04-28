#include <fstream>
#include <iostream>
#include <tuple>

template <class Ch, class Tr, int id, class... Args>
class Printer
{
public:
	static void tuple_print(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t) { 
		//Рекурсивная статичная (для вызова функции без объекта) шаблонная функция внутри класса (для частичной специализации)
		
		Printer<Ch, Tr, id - 1, Args...>::tuple_print(os, t);

		os << std::get<id>(t);
		if (id > 1)
			os << ", ";
	}
};

template <class Ch, class Tr, class... Args>
class Printer<Ch, Tr, -1, Args...> //Частичная специализация по значению id для 0
{
public:
	static void tuple_print(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t) { return; }
};

template <class Ch, class Tr, class... Args>
auto& operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t) {

	os << "(";
	Printer<Ch, Tr, sizeof...(Args) - 1, Args...>::tuple_print(os, t);
	os << ")";

	return os;
}

int main() {

	std::tuple a{ 0, "abcde" };
	std::cout << a << std::endl;

	std::tuple b{ 1, "fffff", 2.f };
	std::cout << b << std::endl;

	std::tuple c{ 2 };
	std::cout << c << std::endl;

	std::tuple d{ std::tuple{ 1, 2 }, std::tuple{ "3", 4.5 }};
	std::cout << d << std::endl;

	std::tuple e{};
	std::cout << e << std::endl;

	return 0;
}