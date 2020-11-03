#include "SDL.h"
#include "app.h"

int main(int argc, char* argv[]) {
	App* app = new App();

	while (app -> Running()) {
		app -> Event();
		app -> Update();
		app -> Render();
	}

	app -> ~App();

	return 1;
}