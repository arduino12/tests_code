#ifndef VIEW_H
#define VIEW_H

/* define for single color display */
#define MONOCHROM_DISPLAY


class View {
	public:
		View();
		
		virtual void draw();
		
		int16_t x, y;
#ifdef MONOCHROM_DISPLAY
		bool invert;
#endif
		
	// private:
};

#endif /* VIEW_H */
