#pragma once
#include "Componente.h"
#include "ObjetoPG.h"
#include "GrafoMapa.h"
#include "ColisionBox.h"
class follow :
	public Componente
{
public:
	follow(ObjetoJuego* ent, ObjetoPG* target, GrafoMapa* m, bool aliado);
	virtual ~follow();
	virtual void lateUpdate();
	virtual void update();
	void clearFollow();
	void doFollow();
	void setTarget(ObjetoPG* targe);
	int getDirection(){ return dir; }
protected:
	int dir;
	GrafoMapa* map;
	bool al;
	//std::vector<std::pair<int, int>> path;
	std::vector<void*> path;
	ObjetoPG* target;
	float coste, paso;
	bool following;
	int signoY, signoX, cont;
	Punto vecDir;
	Punto nextPos;
	ObjetoPG * hitInfo;
	ObjetoPG *pObj;
	std::vector<int> direccion;
};