#include "Lightning.h"
#include "Canvas.h"

using namespace std;

int main() {
	Canvas& canvas = Canvas::GetInstance();
	canvas.Run();
	return 0;
}
