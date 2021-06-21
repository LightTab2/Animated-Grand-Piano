#include "render.h"

void RenderingSystem::drawScene(float angle_x, float angle_y) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V = glm::lookAt(pos, pos + calcDir(angle_x, angle_y), glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku

	glm::mat4 P = glm::perspective(50.0f * static_cast<float>(PI) / 180.0f, aspectRatio, 0.01f, 50.0f); //Wylicz macierz rzutowania

	glm::mat4 M = glm::mat4(1.0f);

	sp->use();//Aktywacja programu cieniującego
	//Przeslij parametry programu cieniującego do karty graficznej
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	glm::mat4 Mfloor = glm::scale(M, glm::vec3(3.f));
	Mfloor = glm::translate(Mfloor, glm::vec3(0.f,0.f,-0.2f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Mfloor));

	glUniform4f(sp->u("ls1"), light1.x, light1.y, light1.z, 1.f); //light source
	glUniform4f(sp->u("ls2"), light2.x, light2.y, light2.z, 1.f); //light source

	glUniform1i(sp->u("pink"), pink);
	glUniform1i(sp->u("l1on"), l1);
	glUniform1i(sp->u("l2on"), l2);
	glUniform1i(sp->u("seed"), rand());

	glUniform1f(sp->u("textureless"), 0);		//uzyj tekstur 0/1
	drawModel(Floor);
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	drawModel(ToyPianoBase);

	//PRZYCISKI oraz MLOTKI
	for (int i = 0; i < 8; i++)
	{
		glm::mat4 Mbutton = glm::translate(M, glm::vec3(0.0f, -ToyPianoButtonsDepth[i], 0.0f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Mbutton));
		drawModel(ToyPianoButtons[i]);

		glm::mat4 Mhammer = glm::translate(M, HammerPositions[i]);
		Mhammer = glm::rotate(Mhammer, HammerAngles[i], glm::vec3(1.0f, 0.0f, 0.0f)); //Wylicz macierz modelu
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Mhammer));
		drawModel(ToyPianoHammer);
	}

	//KLAPA
	glm::mat4 Mlid = glm::translate(M, glm::vec3(-0.644877f, 0.740097f, 0.0f)); //Wylicz macierz modelu
	Mlid = glm::rotate(Mlid, static_cast<float>(lidAngle), glm::vec3(0.0f, 0.0f, 1.0f)); //Wylicz macierz modelu
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Mlid));
	drawModel(ToyPianoLid);

	//PODSTAWA GRAND
	M = glm::scale(M, glm::vec3(0.5f));
	M = glm::translate(M, glm::vec3(0.f, 0.f, -10.0f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	drawModel(GrandPianoBase);

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	drawModel(GrandPianoStrings);

	//KLAPA GRAND

	Mlid = glm::translate(M, glm::vec3(-2.25911f - lidAngle*2.9f, -0.91436f, 0.065f)); //Wylicz macierz modelu
	Mlid = glm::rotate(Mlid, - 35.0f * static_cast<float>(TO_RAD), glm::vec3(0.0f, 0.0f, 1.0f)); //Wylicz macierz modelu
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Mlid));
	drawModel(GrandPianoLid);

	//Klawisze grand

	glm::mat4 keyM = glm::translate(M, glm::vec3(0.f, GrandButtonOffsets[0], 0.f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(keyM));
	drawModel(leftKey);
	keyM = glm::translate(M, glm::vec3(0.f, GrandButtonOffsets[1], 0.f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(keyM));
	drawModel(blackKey);
	keyM = glm::translate(M, glm::vec3(0.f, GrandButtonOffsets[2], 0.f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(keyM));
	drawModel(rightKey);
	float unit = 0.094753329f;
	float unit12 = 7.f*unit;
	for (int i = 0; i != 7; ++i)
	{
		keyM = glm::translate(M, glm::vec3(unit12 * i + 2 * unit, GrandButtonOffsets[3 + i*12], 0.f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(keyM));
		drawModel(leftKey);

		keyM = glm::translate(M, glm::vec3(unit12 * i + 2 * unit, GrandButtonOffsets[4 + i * 12], 0.f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(keyM));
		drawModel(blackKey);

		keyM = glm::translate(M, glm::vec3(unit12 * i, GrandButtonOffsets[5 + i * 12], 0.f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(keyM));
		drawModel(middleKey);

		keyM = glm::translate(M, glm::vec3(unit12 * i + 3 * unit, GrandButtonOffsets[6 + i * 12], 0.f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(keyM));
		drawModel(blackKey);

		keyM = glm::translate(M, glm::vec3(unit12 * i + 3* unit, GrandButtonOffsets[7 + i * 12], 0.f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(keyM));
		drawModel(rightKey);

		keyM = glm::translate(M, glm::vec3(unit12 * i + 5* unit, GrandButtonOffsets[8 + i * 12], 0.f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(keyM));
		drawModel(leftKey);

		keyM = glm::translate(M, glm::vec3(unit12 * i + 5 * unit, GrandButtonOffsets[9 + i * 12], 0.f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(keyM));
		drawModel(blackKey);

		keyM = glm::translate(M, glm::vec3(unit12 * i + 3 * unit, GrandButtonOffsets[10 + i * 12], 0.f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(keyM));
		drawModel(middleKey);

		keyM = glm::translate(M, glm::vec3(unit12 * i + 6 * unit, GrandButtonOffsets[11 + i * 12], 0.f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(keyM));
		drawModel(blackKey);

		keyM = glm::translate(M, glm::vec3(unit12 * i + 4 * unit, GrandButtonOffsets[12 + i * 12], 0.f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(keyM));
		drawModel(middleKey);

		keyM = glm::translate(M, glm::vec3(unit12 * i + 7 * unit, GrandButtonOffsets[13 + i * 12], 0.f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(keyM));
		drawModel(blackKey);

		keyM = glm::translate(M, glm::vec3(unit12 * i + 7 * unit, GrandButtonOffsets[14 + i * 12], 0.f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(keyM));
		drawModel(rightKey);
	}

	/*for (int i = 0; i != 87; ++i)
	{
		glm::mat4 hammerM = glm::scale(M, glm::vec3(0.054768f, 0.024811f, 0.024811f));
		hammerM = glm::translate(hammerM, glm::vec3(unit * i * 4, 0.0f, 0.0f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(hammerM));
		if (GrandPianoButtonsPress[i])
		{
			unsigned index = static_cast<unsigned>(floor(11 * std::min(GrandButtonAnimationTimeEnd,GrandButtonAnimationTime[i]) / GrandButtonAnimationTimeEnd));
			drawModel(GrandPianoHammer[index]);
		}
		else if (GrandButtonAnimationTime[i] != 0.0)
			drawModel(GrandPianoHammer[11 + static_cast<unsigned>(floor(9 * (1.0 - std::min(GrandButtonAnimationTimeEnd, GrandButtonAnimationTime[i]) / GrandButtonAnimationTimeEnd)))]);
		else drawModel(GrandPianoHammer[0]);
	}*/

	glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}