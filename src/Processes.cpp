#include <iostream>
#include <ostream>
#include <string>

#include <events.hpp>

using namespace FearEngine::Events;

struct A
{
	static void test(const char* message)
	{
		std::cout << "test" << message << '\n';
	}

	void another(const char* message)
	{
		std::cout << "Another" << message << '\n';
	}
};

void test(const char* message)
{
	std::cout << "func test" << message << '\n';
}

int main()
{
	auto first = [](const char* message) { std::cout << "first" << message << '\n'; };
	auto second = [](const char* message) { std::cout << "second" << message << '\n'; };
	Observer<decltype(test)> obs = Observer<decltype(test)>();

	obs.attach(first);
	obs.attach(second);
	obs.attach([](const char* message) { std::cout << "third" << message << '\n'; });
	obs.attach(detail::delegate<decltype(test)>::create<&A::test>());
	obs.attach(detail::delegate<decltype(test)>::create<&test>());

	A a;
	obs.attach(detail::delegate<decltype(test)>::create<A, &A::another>(&a));

	obs.notify(" message received.");

	NamedEvent<decltype(test)> evnt = NamedEvent<decltype(test)>();

	evnt.receive(std::string("test"), first);

	evnt.send("test", " message received.");

	return 0;
}
