#include "log.hpp"
#include "mainApplication.hpp"

int main()
{
	logger::init();

	MainApplication app;
	app.init();
	app.run();

	return 0;
}