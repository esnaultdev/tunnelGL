#ifndef _COLLIDE
#define _COLLIDE

class Collide {

public:
	Collide();
	virtual ~Collide();
	virtual void init();
	virtual void update(double dt);
	virtual void draw();

private:

};

#endif