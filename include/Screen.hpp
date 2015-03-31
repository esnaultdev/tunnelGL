#ifndef _SCREEN
#define _SCREEN

class Screen {

public:
	Screen();
	virtual ~Screen();
	virtual void init();
	virtual void update(double dt);
	virtual void draw();

private:

};

#endif