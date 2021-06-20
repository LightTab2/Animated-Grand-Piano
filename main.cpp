#include "global.h"
#include "render.h"
#include "lilPiano.h"

int main(void)
{
	srand(time(NULL));
	RenderingSystem render;
	SoundSystem sound;
	soundPtr = &sound;				//inputManagement musi mieć dostęp do sound, żeby można było grać na pianinie
	double	angle_x = -90,
			angle_y = 0;

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
		render.updatePos(speed * speed_y * static_cast<float>(glfwGetTime())* calcDir(angle_x, angle_y));
		render.updatePos(speed * speed_x * static_cast<float>(glfwGetTime())* calcDir(angle_x, angle_y, glm::vec4(1, 0, 0, 0)));
		
		lilPianoAnim();

		glfwSetTime(0);				//Zeruj timer
		render.drawScene(angle_x, angle_y); //Wykonaj procedurę rysującą
		glfwPollEvents();			//Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły
	}
	exit(EXIT_SUCCESS);
}
