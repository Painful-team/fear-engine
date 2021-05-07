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
	std::cout << "function test" << message << '\n';
}

int main()
{
	auto first = [](const char* message) { std::cout << "first" << message << '\n'; };
	auto second = [](const char* message) { std::cout << "second" << message << '\n'; };
	Observer<decltype(test)> obs = Observer<decltype(test)>();

	auto res = obs.attach(second);
	obs.attach([](const char* message) { std::cout << "third" << message << '\n'; });

	obs.attach<&A::test>();
	obs.attach<&test>();

	A a;
	obs.attach<A, &A::another>(&a);
	obs.notify(" message received.");

	NamedEvent<decltype(test)> evnt = NamedEvent<decltype(test)>();

	evnt.receive("test", first);

	evnt.send("test", " message received.");

	return 0;
}
