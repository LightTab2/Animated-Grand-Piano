#include "render.h"

void RenderingSystem::drawScene(float angle_x, float angle_y) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V = glm::lookAt(pos, pos + calcDir(angle_x, angle_y), glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku


	glm::mat4 P = glm::perspective(50.0f * static_cast<float>(PI) / 180.0f, aspectRatio, 0.01f, 50.0f); //Wylicz macierz rzutowania

	glm::mat4 M = glm::mat4(1.0f);

	sp->use();//Aktywacja programu cieniującego
	//Przeslij parametry programu cieniującego do karty graficznej
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	//PODSTAWA PIANINA
	glUniform4fv(sp->u("ls"), 4, glm::value_ptr(light1)); //light source

	glUniform1f(sp->u("textureless"), 0);	   //uzyj tekstur 0/1
	drawModel(ToyPianoBase);

	//PRZYCISKI oraz MLOTKI
	for (int i = 0; i < 8; i++)
	{
		glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
		glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
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
	Mlid = glm::rotate(Mlid, lidAngle, glm::vec3(0.0f, 0.0f, 1.0f)); //Wylicz macierz modelu
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Mlid));
	
(ToyPianoLid);


	glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}

/*void RenderingSystem::drawScene(float angle_x, float angle_y)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V = glm::lookAt(pos, pos + calcDir(angle_x, angle_y), glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku


	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f, 50.0f); //Wylicz macierz rzutowania

	glm::mat4 M = glm::mat4(1.0f);

	sp->use();//Aktywacja programu cieniującego
	//Przeslij parametry programu cieniującego do karty graficznej
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glUniform1i(sp->u("seed"), rand());
	glUniform4f(sp->u("swiatlo"), swiatlo.x, swiatlo.y, swiatlo.z, 1.f);

	glEnableVertexAttribArray(sp->a("vertex"));		//Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices); //Wskaż tablicę z danymi dla atrybutu vertex
	glEnableVertexAttribArray(sp->a("normal"));		//Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals); //Wskaż tablicę z danymi dla atrybutu vertex
	glEnableVertexAttribArray(sp->a("texCoord0"));  //Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords); //Wskaż tablicę z danymi dla atrybutu vertex
	glUniform1i(sp->u("textureMap0"), 0);
	glUniform1i(sp->u("textureMap1"), 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);			//Narysuj obiekt

	glDisableVertexAttribArray(sp->a("vertex"));		//Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("normal"));		//Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("texCoord0"));		//Wyłącz przesyłanie danych do atrybutu vertex

	glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}*/