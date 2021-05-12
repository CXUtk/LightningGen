#include "Lightning.h"
#include "Canvas.h"

using namespace std;

int main(int argc, char ** argv) {
	Canvas& canvas = Canvas::GetInstance();
	canvas.Run();
	return 0;
}
