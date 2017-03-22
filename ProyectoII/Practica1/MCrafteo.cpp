#include "MCrafteo.h"
#include "TexturasSDL.h"



MCrafteo::MCrafteo(juegoPG*jug, int puntos) : EstadoPG(jug, puntos)
{
	pag1.h = pag2.h = 500; //poner mas peque�o o en funcion de la pantalla
	pag1.w = pag2.w = 375;
	pag1.x = pag2.x = 100; //la posicion del libro
	pag1.y = pag2.y = 30;

	sombra.h = pag1.h;
	sombra.w = 30;
	sombra.x = pag1.x + pag1.w - 20;
	sombra.y = pag1.y;

	numPag = 0;
	derecha = izquierda = flag = false;
	fondo = new TexturasSDL;
	fondo->load(pJuego->getRender(), "..//bmps//temporal//screenshot.bmp");

	rFondo.x = rFondo.y = 0; rFondo.w = pJuego->getScreenWidth(); rFondo.h = pJuego->getScreenHeight(); //rect del fondo (ocupa toda la pantalla)
	ni�os.x = 150; ni�os.y = 560; ni�os.w = ni�os.h = 120; //rect de los personajes
	recuadros.x = 500; recuadros.y = 30; recuadros.w = 500; recuadros.h = 333; //recuadro
}


MCrafteo::~MCrafteo()
{
	delete fondo;
}

void MCrafteo::draw() {
	//Fondo
	fondo->draw(pJuego->getRender(), rFondo);
	
	//Libro
	pJuego->getTextura(TGris)->draw(pJuego->getRender(), rFondo);
	if (numPag < 5)	pJuego->getPag(numPag + 1)->draw(pJuego->getRender(), pag2);
	pJuego->getPag(numPag)->draw(pJuego->getRender(), pag1);

	if (derecha && numPag < 5) animacionS(); //A. siguiente pagina
	else if (izquierda) animacionA(); //A. pagina anterior
	else { //default
		derecha = false;
		izquierda = false;
	}

	//Personajes
	pJuego->getTextura(TLyov)->draw(pJuego->getRender(), ni�os);
	ni�os.x += pag2.w - ni�os.w - 100;
	pJuego->getTextura(TZhenia)->draw(pJuego->getRender(), ni�os);
	ni�os.x = 150;

	//Equipables
	recuadros.y = 30;
	recuadros.h = 333;
	pJuego->getTextura(TEquipables)->draw(pJuego->getRender(), recuadros);

	//Materiales
	recuadros.y += recuadros.h + 50;
	recuadros.h = 250;
	pJuego->getTextura(TMateriales)->draw(pJuego->getRender(), recuadros);
}

void MCrafteo::animacionS() {

	--pag1.w; //la pag se hace mas peque�a
	sombra.x = pag1.x + pag1.w; //le sigue la sombra

	if (pag1.w < 350 && pag1.w >= 0) {
		++aux;
		if (aux == 10) { ++sombra.w; aux = 0; } //la sombra se hace mas grande
		pJuego->getTextura(TSombra1)->draw(pJuego->getRender(), sombra);
	}

	else if (pag1.w <= 0) {
		derecha = false;
		++numPag;

		pag1.w = 375;
		sombra.w = 30;
	}

}

void MCrafteo::animacionA() {

	if (!flag && numPag != 0) { //solo una vez
		pag1.w = 0;
		flag = true;
		aux = 0;
		sombra.w = 65;

		--numPag;
		if (numPag == -1) numPag = 0;
	}

	else if (flag) {

		++pag1.w; //la pag se hace mas grande
		sombra.x = pag1.x + pag1.w; //le sigue la sombra

		if (pag1.w < 350) {
			++aux;
			if (aux == 10) { --sombra.w; aux = 0; } //la sombra se hace mas peque�a
			pJuego->getTextura(TSombra1)->draw(pJuego->getRender(), sombra);
		}

		else if (pag1.w >= 375) {
			izquierda = false;
			flag = false;

			pag1.w = 375;
			sombra.w = 30;
		}
	}
	else izquierda = false;
}

void MCrafteo::update() {}

void MCrafteo::onKeyUp(char k) {

	switch (k) {
	case 'q':
		remove("..//bmps//temp//screenshot.bmp");
		pJuego->estados.pop();
		break;

	case 'd':
		derecha = true;
		break;

	case 'i':
		izquierda = true;
		break;

	default:
		break;
	}
}
