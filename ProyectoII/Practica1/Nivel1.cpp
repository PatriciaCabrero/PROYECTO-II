#include "Nivel1.h"
#include "PremioPG.h"
#include "SDL_mixer.h"
#include "GameOver.h"
#include "Arbol.h"
#include <algorithm>
#include "Pausa.h"
#include "Cebo.h"
#include "Cuerda.h"
#include "Enredadera.h"
#include "Hueso.h"
#include "Madera.h"
#include "Piedra.h"
#include "TextCb.h"
#include "Trigger.h"
#include "TrampaCerrada.h"
#include "Yesca.h"
#include "MCrafteo.h"
#include "Lobo.h"
#include "AntorchaC.h"
#include "follow.h""
#include "TrampaAbierta.h"
#include "Equipo.h"
#include "Escondite.h"
#include "Obstaculo.h"
#include "Carro�a.h"
#include "Valla.h"
#include "Arbol2.h"
#include "Arbol3.h"
#include "BbYgBroom.h"
#include "BbYgBucket.h"
#include "ObstaculoNieve.h"
#include "ObstaculoPiedra.h"
#include "Caseta.h"
#include "Pozo.h"
#include "RocaGrande.h"
#include "Tronco.h"


Nivel1::Nivel1(juegoPG*jug, std::string map, std::string objetos, Punto posRec, Punto posCaz, std:: string act) : EstadoPG(jug, 0){
	mapa = new GrafoMapa();
	mode = Play;
	visible = false; //Muestra o no las colisiones
	std::vector<char> mapAux;
	cargaMapa(map, mapAux);
	mapa->creaMapa(mapAux);
	camara.x = camara.y = 0;
	camara.h = pJuego->getScreenHeight(); camara.w = pJuego->getScreenWidth();
	animNieve1.h = animNieve2.h = camara.h+1000; animNieve1.w = animNieve2.w = camara.w+1000;
	animNieve1.x = animNieve2.x = camara.w;
	animNieve1.y = animNieve2.y = camara.h;

	archivoObj = objetos;

	if (act == "C"){
		pCazador = new Cazador(pJuego, camara.x + (camara.w / 2), camara.y + (camara.h / 2));
		pCazador->newComponente(new AntorchaC(pCazador, this), "AntorchaC");
		vecObj.push_back(pCazador);
		pCazador->setAbsRect(abs(pCazador->getAbsRect().x - posCaz.x), abs(pCazador->getAbsRect().y - posCaz.y));

		pRecolector = new Recolector(pJuego, posRec.x, posRec.y);
		pRecolector->newComponente(new AntorchaC(pRecolector, this), "AntorchaC");
		vecObj.push_back(pRecolector);
	}
	else {
		pCazador = new Cazador(pJuego, posCaz.x,posCaz.y);
		pCazador->newComponente(new AntorchaC(pCazador, this), "AntorchaC");
		vecObj.push_back(pCazador);

		pRecolector = new Recolector(pJuego, camara.x + (camara.w / 2), camara.y + (camara.h / 2));
		pRecolector->newComponente(new AntorchaC(pRecolector, this), "AntorchaC");
		vecObj.push_back(pRecolector);
		pRecolector->setAbsRect(abs(pRecolector->getAbsRect().x - posCaz.x), abs(pRecolector->getAbsRect().y - posCaz.y));
	}

	centroRel.x = camara.x + (camara.w / 2);
	centroRel.y = camara.y + (camara.h / 2);
	if (act == "C"){
		camara.x = (posCaz.x - (camara.w / 2));
		camara.y = (posCaz.y - (camara.h / 2));
	}
	else {
		camara.x = (posRec.x - (camara.w / 2));
		camara.y = (posRec.y - (camara.h / 2));
	}

	Trigger *auxTr;
	//dialogos de tutorial
	auxTr = new Trigger(pJuego, 6850, 9150, pCazador, pRecolector); //tabulador
	auxTr->setCallback(new TextCb(auxTr, "../docs/textos/tutorial1Juntos.txt"));
	vecTriggers.push_back(auxTr);

	auxTr = new Trigger(pJuego, 5850, 9800, pCazador, pRecolector); //escondites
	auxTr->setCallback(new TextCb(auxTr, "../docs/textos/tutorial2Juntos.txt"));
	auxTr->setTriggerDim(500, 500);
	vecTriggers.push_back(auxTr);

	//Random comments
	auxTr = new Trigger (pJuego, 1662, 1284, pCazador, pRecolector);
	auxTr->setCallback(new TextCb(auxTr, "../docs/textos/dialogo1.txt"));
	vecTriggers.push_back(auxTr);

	auxTr = new Trigger(pJuego, 1662, 1084, pCazador, pRecolector);
	auxTr->setCallback(new TextCb(auxTr, "../docs/textos/dialogo2.txt"));
	vecTriggers.push_back(auxTr);

	auxTr = new Trigger(pJuego, 865, 1070, pCazador, pRecolector);
	auxTr->setCallback(new TextCb(auxTr, "../docs/textos/dialogo3.txt"));
	auxTr->setTriggerDim(80, 80);
	vecTriggers.push_back(auxTr);

	cargaObj(objetos);

	cargarAudio("../sounds/reverb/ReverbBosque.wav");
	cargarAssetsAudio("../docs/fxNivel1.txt", 'f');
	cargarAssetsAudio("../docs/mNivel1.txt", 'm');
	reproduceMusica("Bosque", false);
	reproduceAmb("Viento",false);


	activePlayer = act;
	if (activePlayer == "R"){
		pCazador->swAble();
		pRecolector->swAble();
	}

	
	vecObj.push_back(new Lobo(pJuego, pCazador ,pRecolector,mapa, 1050, 1200));
	pCazador->newComponente(new Equipo(pCazador, static_cast<Mochila*>(pRecolector->dameComponente("Mochila"))), "Equipo");
	pRecolector->newComponente(new Equipo(pRecolector, static_cast<Mochila*>(pRecolector->dameComponente("Mochila"))), "Equipo");
	pRecolector->newComponente(new follow(pRecolector, pCazador, mapa, true), "follow");

	rectEquipo.x = 50; rectEquipo.y = 50;
	rectEquipo.h = rectEquipo.w = 50;  animEquipo.h = animEquipo.w = 100;
	animEquipo.y = animEquipo.x = 0;

	rectZonaOscura.h = 1400; rectZonaOscura.w = 1200;
	rectZonaOscura.x = -100; rectZonaOscura.y = 500;
	hasTorch = false;
	alpha = 255;
	firsTime = true;
}

bool ordena(ObjetoJuego*p1, ObjetoJuego*p2){
	return(dynamic_cast<ObjetoPG*>(p1)->getColisionBox().y < dynamic_cast<ObjetoPG*>(p2)->getColisionBox().y);
}
void Nivel1::draw(){

	if (firsTime){
		firsTime = false;
		fadeIn(20);
	}
	SDL_Rect aux;
	Tile tile;
	
	if (pJuego->input.sw) swPlayer();
	else{
		for (unsigned int i = 0; i < vecTile.size(); i++){
			vecTile[i].x -= camara.x; vecTile[i].y -= camara.y;
			tile = vecTile[i];
			aux.x = tile.x; aux.y = tile.y; aux.w = 122; aux.h = 83;
			if (aux.x > -200 && aux.x < pJuego->getScreenWidth() && aux.y > -200 && aux.y < pJuego->getScreenHeight())
				pJuego->getTextura(TTileSet)->draw(pJuego->getRender(), tile.rectTileset, aux);
		}
		for (unsigned int i = 0; i < vectBordes.size(); i++){
			vectBordes[i].A.x -= camara.x;
			vectBordes[i].A.y -= camara.y;
			vectBordes[i].B.x -= camara.x;
			vectBordes[i].B.y -= camara.y;
			vectBordes[i].C.x -= camara.x;
			vectBordes[i].C.y -= camara.y;
		}
		std::sort(vecObj.begin(), vecObj.end(), ordena);

		for (HuellasCamino* ob : huellasCamino) ob->draw((activePlayer == "R" || mode == Edition));
		for (ObjetoJuego* ob : vecObj) ob->draw();
		for (ObjetoJuego* trg : vecTriggers) trg->draw();

		centroRel.x += camara.x; centroRel.y += camara.y;
		rectZonaOscura.x -= camara.x;
		rectZonaOscura.y -= camara.y;

		setCamara(0, 0); //Se reinicia el offset a 0
		int x = rand() % 100;
		if (x >= 60){
			animNieve1.x--;
			animNieve1.y--;
		}
		if (animNieve1.x <= 0)
			animNieve1.x = camara.w;
		if (animNieve1.y <= 0)
			animNieve1.y = camara.h;

		if (x >= 70){
			animNieve2.x--;
			animNieve2.y--;
		}
		if (animNieve2.x <= 0)
			animNieve2.x = camara.w;
		if (animNieve2.y <= 0)
			animNieve2.y = camara.h;

		pJuego->getTextura(TNieve1)->draw(pJuego->getRender(), animNieve1, camara);
		pJuego->getTextura(TNieve2)->draw(pJuego->getRender(), animNieve2, camara);

		if (hasTorch){
			int aux, aux2; aux2 = rand() % 51; aux = 0;
			if (aux2 >= 45) aux = rand() % 20;

			//Poner aqu� todas las zonas oscuras del mapa
			pJuego->getTextura(TZonaOscura)->setBlendMode(pJuego->getRender(), SDL_BLENDMODE_BLEND);
			pJuego->getTextura(TZonaOscura)->draw(pJuego->getRender(), rectZonaOscura, 240 + (aux / 2));
			pCazador->lateDraw();
			pRecolector->lateDraw();

		}
		else pJuego->getTextura(TZonaOscura)->draw(pJuego->getRender(), rectZonaOscura);

		pJuego->getTextura(TLuz)->draw(pJuego->getRender(), pJuego->getNieblaRect(), camara);

		for (ObjetoJuego* trg : vecTriggers) trg->lateDraw();//TIENE QUE SER LO ULTIMO EN DIBUJARSE
	}
	drawEquipo();
	
}
void Nivel1::drawEquipo(){

	int auxiliar;
	if (activePlayer == "C") auxiliar = static_cast<Equipo*>(pCazador->dameComponente("Equipo"))->getEquipo();
	else  auxiliar = static_cast<Equipo*>(pRecolector->dameComponente("Equipo"))->getEquipo();

	//Nada, Trampa, Antorcha, Hacha
	bool dibuja = true;
	switch (auxiliar)
	{
	case 0: dibuja = false; 
		break;
	case 1: animEquipo.x = 400; //Trampa
		break;
	case 2: animEquipo.x = 0; //Antorcha
		break;
	case 3: animEquipo.x = 100; //Hacha
		break;
	case 4: animEquipo.x = 200; //Pala
		break;
	case 5: animEquipo.x = 300; //Pico
		break;
	default:
		break;
	}

	if (dibuja) pJuego->getTextura(TObjetoEquipo)->draw(pJuego->getRender(), animEquipo, rectEquipo);
}
void Nivel1::swPlayer(){
	SDL_Rect aux;
	Tile tile;
	struct feedback{
		float x; float y;
	};
	feedback fd;
	reproduceFx("CambioPersonaje", 0, 0, 0);
	if (activePlayer == "C"){
		pCazador->swAble();
		fd.x = -1 * (pCazador->getRect().x - pRecolector->getRect().x);
		fd.y = -1 * (pCazador->getRect().y - pRecolector->getRect().y);
		activePlayer = "R";
	}
	else if (activePlayer == "R") {
		pRecolector->swAble();
		fd.x = -1 * (pRecolector->getRect().x - pCazador->getRect().x);
		fd.y = -1 * (pRecolector->getRect().y - pCazador->getRect().y);
		activePlayer = "C";
	}
	
	for (int i = 0; i < 8; i++){
		camara.x = fd.x / 8.0f;
		camara.y = fd.y / 8.0f;

		for (unsigned int i = 0; i < vecTile.size(); i++){
			vecTile[i].x -= camara.x; vecTile[i].y -= camara.y;
			tile = vecTile[i];
			aux.x = tile.x; aux.y = tile.y; aux.w = 122; aux.h = 83;
			pJuego->getTextura(TTileSet)->draw(pJuego->getRender(), tile.rectTileset, aux);
		}

		for (unsigned int i = 0; i < vectBordes.size(); i++){
			vectBordes[i].A.x -= camara.x;
			vectBordes[i].A.y -= camara.y;
			vectBordes[i].B.x -= camara.x;
			vectBordes[i].B.y -= camara.y;
			vectBordes[i].C.x -= camara.x;
			vectBordes[i].C.y -= camara.y;
		}
		for (HuellasCamino* ob : huellasCamino) ob->draw((activePlayer == "R" || mode == Edition));
		std::sort(vecObj.begin(), vecObj.end(), ordena);
		for (ObjetoJuego* ob : vecObj){
			ob->draw();
		}
		for (ObjetoJuego* ob : vecTriggers){ //TIENE QUE SER LO ULTIMO EN DIBUJARSE
			ob->draw();
		}
		centroRel.x += camara.x; centroRel.y += camara.y;
		rectZonaOscura.x -= camara.x;
		rectZonaOscura.y -= camara.y;

		setCamara(0, 0); //Se reinicia el offset a 0
		int x = rand() % 100;
		if (x >= 60){
			animNieve1.x--;
			animNieve1.y--;
		}
		if (animNieve1.x <= 0)
			animNieve1.x = camara.w;
		if (animNieve1.y <= 0)
			animNieve1.y = camara.h;

		if (x >= 70){
			animNieve2.x--;
			animNieve2.y--;
		}
		if (animNieve2.x <= 0)
			animNieve2.x = camara.w;
		if (animNieve2.y <= 0)
			animNieve2.y = camara.h;

		pJuego->getTextura(TNieve1)->draw(pJuego->getRender(), animNieve1, camara);
		pJuego->getTextura(TNieve2)->draw(pJuego->getRender(), animNieve2, camara);

		if (hasTorch){
			int aux, aux2; aux2 = rand() % 51; aux = 0;
			if (aux2 >= 45) aux = rand() % 20;

			//Poner aqu� todas las zonas oscuras del mapa
			pJuego->getTextura(TZonaOscura)->setBlendMode(pJuego->getRender(), SDL_BLENDMODE_BLEND);
			pJuego->getTextura(TZonaOscura)->draw(pJuego->getRender(), rectZonaOscura, 240 + (aux / 2));
			pCazador->lateDraw();
			pRecolector->lateDraw();

		}
		else pJuego->getTextura(TZonaOscura)->draw(pJuego->getRender(), rectZonaOscura);

		pJuego->getTextura(TLuz)->draw(pJuego->getRender(), pJuego->getNieblaRect(), camara);

		for (ObjetoJuego* trg : vecTriggers) trg->lateDraw();//TIENE QUE SER LO ULTIMO EN DIBUJARSE
		SDL_RenderPresent(pJuego->getRender());
		SDL_Delay(20);
	}
	if (activePlayer == "C") pCazador->swAble();
	else pRecolector->swAble();
	pJuego->input.sw = false;
}
void escribe(std::string s, int x, int y, std:: string name){
	std::ofstream f;
	f.open("../docs/"+name,  ios::app);
	f << s << " , " << std::to_string(x) << " , " << std::to_string(y) << "\n";
	f.close();
}
void Nivel1::onKeyUp(char k) {
	if (mode == Play){
		switch (k)
		{
		case 'q':
			reproduceFx("AbreMenu", 0, 0, 0);
			pJuego->estados.push(new MCrafteo(pJuego, contPuntos, static_cast<Mochila*>(pRecolector->dameComponente("Mochila")),
			static_cast<Equipo*>(pCazador->dameComponente("Equipo")), static_cast<Equipo*>(pRecolector->dameComponente("Equipo"))));
			break;
		case 's':
			reproduceFx("AbreMenu", 0, 0, 0);
			pJuego->estados.push(new Pausa(pJuego, this, contPuntos));

			break;
		case 't': pJuego->input.sw = true;
			break;
		default:
			break;
		}
	}
	else {
		std::cout << "levantada tecla: " << k << ". Genera: \n";
		switch (k) { //EDITOR DE NIVEL
		case '1': //Arbol Movible
			std::cout << "agregado Arbol Movible\n";
			vecObj.push_back(new Arbol(pJuego, pCazador->getRect().x + 20, pCazador->getRect().y - 120));
			escribe("ArbolF", centroRel.x + 20, centroRel.y - 120, archivoObj);
			break;
		case 'q': //Arbol Normal
			std::cout << "agregado Arbol Normal\n";
			vecObj.push_back(new Arbol(pJuego, pCazador->getRect().x+20  ,pCazador->getRect().y-120, false));
			escribe("Arbol", centroRel.x + 20, centroRel.y - 120,archivoObj);
			break;
		case 'w': //Obstaculo piedra
			vecObj.push_back(new ObstaculoPiedra(pJuego, pCazador->getRect().x + 35, pCazador->getRect().y - 20,TObs2,"Pico"));
			escribe("ObstaculoPiedra", centroRel.x + 35, centroRel.y - 20, archivoObj);
			break;
		case 'r': //Obstaculo Nieve
			vecObj.push_back(new ObstaculoNieve(pJuego, pCazador->getRect().x + 35, pCazador->getRect().y - 20, TObs1, "Pico"));
			escribe("ObstaculoNieve", centroRel.x + 35, centroRel.y - 20, archivoObj);
			break;
		case '<': //Roca Grande
			std::cout << "agregado RocaGrande\m";
			vecObj.push_back(new RocaGrande(pJuego, pCazador->getRect().x + 35, pCazador->getRect().y - 20));
			escribe("RocaGrande", centroRel.x + 35, centroRel.y - 20, archivoObj);
			break;
		case '.': //Tronco
			std::cout << "agregado Tronco\n";
			vecObj.push_back(new Tronco(pJuego, pCazador->getRect().x + 35, pCazador->getRect().y - 20));
			escribe("Tronco", centroRel.x + 35, centroRel.y - 20, archivoObj);
			break;
		case '4': //Arbol2
			vecObj.push_back(new Arbol2(pJuego, pCazador->getRect().x + 35, pCazador->getRect().y - 20));
			escribe("Arbol2", centroRel.x + 35, centroRel.y - 20, archivoObj);
			break;
		case '5': //Arbol3A con enredaderas1
			std::cout << "agregado Arbol3A\n";
			vecObj.push_back(new Arbol3(pJuego, pCazador->getRect().x + 35, pCazador->getRect().y - 20, 0));
			escribe("Arbol3A", centroRel.x + 35, centroRel.y - 20, archivoObj);
			break;
		case '6': //Arbol3B sin enredaderas
			std::cout << "agregado Arbol3B\n";
			vecObj.push_back(new Arbol3(pJuego, pCazador->getRect().x + 35, pCazador->getRect().y - 20, 1));
			escribe("Arbol3B", centroRel.x + 35, centroRel.y - 20, archivoObj);
			break;
		case '7': //Baba Yaga Broom
			std::cout << "agregado BabaYagaBroom\n";
			vecObj.push_back(new BbYgBroom(pJuego, pCazador->getRect().x + 35, pCazador->getRect().y - 20));
			escribe("BabaYagaBroom", centroRel.x + 35, centroRel.y - 20, archivoObj);
			break;
		case '8': //Baba Yaga Bucket
			std::cout << "agregado BabaYagaBucket\n";
			vecObj.push_back(new BbYgBucket(pJuego, pCazador->getRect().x + 35, pCazador->getRect().y - 20));
			escribe("BabaYagaBucket", centroRel.x + 35, centroRel.y - 20, archivoObj);
			break;
		case '9': //Caseta Bota
			std::cout << "agregado CasetaBota\n";
			vecObj.push_back(new Caseta(pJuego, pCazador->getRect().x + 35, pCazador->getRect().y - 20, 0));
			escribe("CasetaBota", centroRel.x + 35, centroRel.y - 20, archivoObj);
			break;
		case '0': //Caseta Bota y Domovoi
			std::cout << "agregado CasetaBotaDomo\n";
			vecObj.push_back(new Caseta(pJuego, pCazador->getRect().x + 35, pCazador->getRect().y - 20, 1));
			escribe("CasetaBotaDomo", centroRel.x + 35, centroRel.y - 20, archivoObj);
			break;
		case ',': //Caseta Domovoi
			std::cout << "agregado CasetaDomovoi\n";
			vecObj.push_back(new Caseta(pJuego, pCazador->getRect().x + 35, pCazador->getRect().y - 20, 2));
			escribe("CasetaDomovoi", centroRel.x + 35, centroRel.y - 20, archivoObj);
			break;
		case 'y': //Escondite
			vecObj.push_back(new Escondite(pJuego, pCazador->getRect().x + 20, pCazador->getRect().y +45));
			escribe("Escondite", centroRel.x + 20, centroRel.y + 45, archivoObj);
			break;
		case 'u': //Piedra
			vecObj.push_back(new Piedra(pJuego, pCazador->getRect().x + 20, pCazador->getRect().y + 50));
			escribe("Piedra", centroRel.x + 20, centroRel.y + 50, archivoObj);
			break;
		case 'z': //Madera
			vecObj.push_back(new Madera(pJuego, pCazador->getRect().x + 20, pCazador->getRect().y + 50));
			escribe("Madera", centroRel.x + 20, centroRel.y + 50, archivoObj);
			break;
		case 'o': //Hueso
			vecObj.push_back(new Hueso(pJuego, pCazador->getRect().x + 20, pCazador->getRect().y + 50));
			escribe("Hueso", centroRel.x + 20, centroRel.y + 50, archivoObj);
			break;
		case 'p': //Enredadera
			vecObj.push_back(new Enredadera(pJuego, pCazador->getRect().x + 20, pCazador->getRect().y + 50));
			escribe("Enredadera", centroRel.x + 20, centroRel.y + 50, archivoObj);
			break;
		case 'c': //Cebo
			vecObj.push_back(new Cebo(pJuego, pCazador->getRect().x + 20, pCazador->getRect().y + 50));
			escribe("Cebo", centroRel.x + 20, centroRel.y + 50, archivoObj);
			break;
		case 'x': //Yesca
			vecObj.push_back(new Yesca(pJuego, pCazador->getRect().x + 20, pCazador->getRect().y + 50));
			escribe("Yesca", centroRel.x + 20, centroRel.y + 50, archivoObj);
			break;
		case 'n': //Carro�a
			vecObj.push_back(new Carro�a(pJuego, pCazador->getRect().x + 40, pCazador->getRect().y +20));
			escribe("Carro�a", centroRel.x + 40, centroRel.y + 20, archivoObj);
			break;
		case 'g': //Trampa Cerrada
			vecObj.push_back(new TrampaCerrada(pJuego, pCazador->getRect().x + 20, pCazador->getRect().y + 50));
			escribe("TrampaCerrada", centroRel.x + 20, centroRel.y + 50, archivoObj);
			break;
		case 'h': //Huella Arriba Hombre
			huellasCamino.push_back(new HuellasCamino(pJuego, pCazador->getRect().x + 20, pCazador->getRect().y + 50, "HS"));
			escribe("HS", centroRel.x + 20, centroRel.y + 50, archivoObj);
			break;
		case 'j': //Huella Abajo Hombre
			huellasCamino.push_back(new HuellasCamino(pJuego, pCazador->getRect().x + 20, pCazador->getRect().y + 50, "HI"));
			escribe("HI", centroRel.x + 20, centroRel.y + 50, archivoObj);
			break;
		case 'k': //Huella Arriba Lobo
			huellasCamino.push_back(new HuellasCamino(pJuego, pCazador->getRect().x + 20, pCazador->getRect().y + 50, "LS"));
			escribe("LS", centroRel.x + 20, centroRel.y + 50, archivoObj);
			break;
		case 'l': //Huella Abajo Lobo
			huellasCamino.push_back(new HuellasCamino(pJuego, pCazador->getRect().x + 20, pCazador->getRect().y + 50, "LI"));
			escribe("LI", centroRel.x + 20, centroRel.y + 50, archivoObj);
			break;
		case 'v': //Lobo
			vecObj.push_back(new Lobo(pJuego, pCazador, pRecolector, mapa, pCazador->getRect().x + 30, pCazador->getRect().y + 30));
			escribe("Lobo", centroRel.x + 30, centroRel.y + 30, archivoObj);
			break;
		case '2': //VALLA1
			vecObj.push_back(new Valla(pJuego, pCazador->getRect().x +45, pCazador->getRect().y - 20, "A"));
			escribe("Valla", centroRel.x + 45, centroRel.y - 20, archivoObj);
			break;
		case '3': //VALLA2
			vecObj.push_back(new Valla(pJuego, pCazador->getRect().x + 45, pCazador->getRect().y - 20, "D"));
			escribe("Valla2", centroRel.x + 45, centroRel.y - 20, archivoObj);
			break;
		case '+': //Pozo sin cuerda
			std::cout << "agregado PozoA\n";
			vecObj.push_back(new Pozo(pJuego, pCazador->getRect().x + 35, pCazador->getRect().y - 20, 0));
			escribe("PozoA", centroRel.x + 35, centroRel.y - 20, archivoObj);
			break;
		case ' ': //Pozo con cuerda
			std::cout << "agregado PozoB\n";
			vecObj.push_back(new Pozo(pJuego, pCazador->getRect().x + 35, pCazador->getRect().y - 20, 1));
			escribe("PozoB", centroRel.x + 35, centroRel.y - 20, archivoObj);
			break;
		case 's': //Sale del modo Edicion
			mode = Play;
			reproduceFx("AbreMenu", 0, 0, 0);
			pJuego->estados.push(new Pausa(pJuego, this, contPuntos));
			break;
		case 't': 
			pJuego->input.sw = true;
			break;
		default:
			std::cout << "Nada (case default). \n";
			break;
		}
	}
}

void Nivel1::cargaObj(std:: string name){
	std::ifstream f; 
	
	f.open(name, std::ios::in);
	ObjetoJuego *aux;
	std::string type;
	Punto pos;
	char stash;
	while (!f.eof() && !f.fail()){
		f >> type;
		f.get(stash); f.get(stash); f.get(stash);
		f >> pos.x;
		f.get(stash); f.get(stash); f.get(stash);
		f >> pos.y;
		if (!f.fail()){
			if (type == "Arbol")vecObj.push_back(new Arbol(pJuego, pos.x, pos.y, false)); //OK
			else if (type == "ArbolF") vecObj.push_back(new Arbol(pJuego, pos.x, pos.y)); //OK
			else if (type == "RocaGrande") vecObj.push_back(new RocaGrande(pJuego, pos.x, pos.y)); //MAL
			else if (type == "Escondite") vecObj.push_back(new Escondite(pJuego, pos.x, pos.y));
			else if (type == "Piedra") vecObj.push_back(new Piedra(pJuego, pos.x, pos.y));
			else if (type == "Madera") vecObj.push_back(new Madera(pJuego, pos.x, pos.y));
			else if (type == "Hueso") vecObj.push_back(new Hueso(pJuego, pos.x, pos.y));
			else if (type == "Enredadera") vecObj.push_back(new Enredadera(pJuego, pos.x, pos.y));
			else if (type == "Cebo") vecObj.push_back(new Cebo(pJuego, pos.x, pos.y));
			else if (type == "Yesca") vecObj.push_back(new Yesca(pJuego, pos.x, pos.y));
			else if (type == "TrampaCerrada") vecObj.push_back(new TrampaCerrada(pJuego, pos.x, pos.y));
			else if (type == "Lobo") vecObj.push_back(new Lobo(pJuego, pCazador, pRecolector,mapa, pos.x, pos.y));
			else if (type == "Carro�a") vecObj.push_back(new Carro�a(pJuego, pos.x, pos.y));
			else if (type == "HS") huellasCamino.push_back(new HuellasCamino(pJuego, pos.x, pos.y, "HS"));
			else if (type == "HI")huellasCamino.push_back(new HuellasCamino(pJuego, pos.x, pos.y, "HI"));
			else if (type == "LS") huellasCamino.push_back(new HuellasCamino(pJuego, pos.x, pos.y, "LS"));
			else if (type == "LI") huellasCamino.push_back(new HuellasCamino(pJuego, pos.x, pos.y, "LI"));
			else if (type == "Valla") vecObj.push_back(new Valla(pJuego, pos.x, pos.y, "A"));
			else if (type == "Valla2")vecObj.push_back(new Valla(pJuego, pos.x, pos.y, "D"));
			//SET TONI
			else if (type == "ObstaculoNieve") vecObj.push_back(new Obstaculo(pJuego, pos.x, pos.y, TObs1, "Pala"));
			else if (type == "ObstaculoPiedra") vecObj.push_back(new Obstaculo(pJuego, pos.x, pos.y, TObs2, "Pico"));
			else if (type == "Arbol2") vecObj.push_back(new Arbol2(pJuego, pos.x, pos.y));		//MAL
			else if (type == "Arbol3A")vecObj.push_back(new Arbol3(pJuego, pos.x, pos.y, 0));	//MAL
			else if (type == "Arbol3B")vecObj.push_back(new Arbol3(pJuego, pos.x, pos.y, 1));	//MAL
			else if (type == "CasetaBota")vecObj.push_back(new Caseta(pJuego, pos.x, pos.y, 0)); //MAL
			else if (type == "CasetaBotaDomo")vecObj.push_back(new Caseta(pJuego, pos.x, pos.y, 1)); //MAL
			else if (type == "CasetaDomovoi")vecObj.push_back(new Caseta(pJuego, pos.x, pos.y, 2)); //MAL
			else if (type == "PozoA")vecObj.push_back(new Pozo(pJuego, pos.x, pos.y, 0)); //MAL
			else if (type == "PozoB")vecObj.push_back(new Pozo(pJuego, pos.x, pos.y, 1)); //MAL
			else if (type == "Tronco") vecObj.push_back(new Tronco(pJuego, pos.x, pos.y)); //MAL
			else if (type == "BabaYagaBroom") vecObj.push_back(new BbYgBroom(pJuego, pos.x, pos.y)); //MAL
			else if (type == "BabaYagaBucket") vecObj.push_back(new BbYgBucket(pJuego, pos.x, pos.y)); //MAL
		}
	}
	f.close();
}
void Nivel1::fadeOut(int time){
	for (int i = 0; i < 200; i+=2){
		pJuego->getTextura(TTapa)->draw(pJuego->getRender(), i);
		SDL_RenderPresent(pJuego->getRender());
		SDL_Delay(time);
	}
}
void Nivel1::fadeIn(int time){
	for (int i = 255; i > 6; i-= 4){
		draw();
		pJuego->getTextura(TTapa)->draw(pJuego->getRender(), i);
		SDL_RenderPresent(pJuego->getRender());
		SDL_Delay(time);
	}
}
Nivel1::~Nivel1()
{
	for (HuellasCamino* it : huellasCamino){
		delete it;
	}
}

