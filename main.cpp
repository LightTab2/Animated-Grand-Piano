#include "global.h"
#include "render.h"
#include "lilPiano.h"
#include "grandPiano.h"

int main(void)
{
	srand(static_cast<unsigned>(time(NULL)));
	RenderingSystem render;
	SoundSystem sound;
	soundPtr = &sound;				//inputManagement musi mieć dostęp do sound, żeby można było grać na pianinie

	grandPianoInit();
	double	angle_x = 500,
			angle_y = 3160;

	render.setCursorPos(angle_y, angle_x);
	glfwSetTime(0);
	while (!render.shouldClose())	//Tak długo jak okno nie powinno zostać zamknięte
	{
		render.getCursorPos(angle_y, angle_x);
		if (angle_x > 1570)
		{
			angle_x = 1570;
			render.setCursorPos(angle_y, angle_x);
		}
		else if (angle_x < -1570)
		{
			angle_x = -1570;
			render.setCursorPos(angle_y, angle_x);
		}
		angle_x /= 1000.0;
		angle_y /= -1000.0;
		render.updatePos(speed * speed_y * static_cast<float>(glfwGetTime()) * calcDir(static_cast<float>(angle_x), static_cast<float>(angle_y)));
		render.updatePos(speed * speed_x * static_cast<float>(glfwGetTime()) * calcDir(static_cast<float>(angle_x), static_cast<float>(angle_y), glm::vec4(1, 0, 0, 0)));
		
		lilPianoAnim();
		grandAnimation();
		glfwSetTime(0);				//Zeruj timer
		render.drawScene(static_cast<float>(angle_x), static_cast<float>(angle_y)); //Wykonaj procedurę rysującą
		glfwPollEvents();			//Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły
	}
	exit(EXIT_SUCCESS);
}
