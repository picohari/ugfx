/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
 * @file	src/gwin/gwin_spinbox.c
 * @brief	GWIN sub-system spinbox code
 */

// This widget uses partial redraw by using the widget flags to indicate which
// portions of the widget needs to be redrawn.

#include "../../gfx.h"
#if GFX_USE_GWIN && GWIN_NEED_SPINBOX

#include "gwin_class.h"
#include "stdlib.h"
#include "string.h"

// Configuration options
#define TEXTPADDING	2
#define BOXPADDING	8
#define MAXCHARS	20

// Macros to assist in data type conversions
#define gw2obj ((GSpinboxObject *)gw)
#define gw2ButtonSize (gw->g.height)

// Macros to assist in calculations
#define FIELD_WIDTH(gw) (gw->g.width - 2*gw->g.height)

// Function to convert number to string, add decimal mark and sign
// Parameter s initially points to end of buffer
// ToDo: Improve this function. Need some serious clean-up & proper error detection/prevention/handling.
char* fmtNum(int value, char* s, short placesOrTxtlen, const char* mark)
{
	uint8_t places = 0;
	bool_t sign;
	bool_t markAdded = FALSE;

	sign = (value < 0) ? 1 : 0;					// Sign flag

	*--s = 0;									// Add null-terminator to end of string

	for (; value; value /= 10) {
		*--s = '0' + abs(value) % 10;
		if (++places == placesOrTxtlen) {
			*--s = *mark;						// Add decimal mark
			markAdded = TRUE;
		}
	}

	if (placesOrTxtlen && !markAdded) {			// If a decimalplaces and no mark added then add leading zeroes
		do {
			*--s = '0';
		} while (++places != placesOrTxtlen);
		*--s = *mark;							// Add decimal mark
	}

	if (*s == *mark)
		*--s = '0';								// If first char is a decimalmark then add a leading zero
	if (sign)
		*--s = '-';

	return s;
}

static void sendSpinboxEvent(GSpinboxObject* gsw)
{
	#define ple ((GEventGWinSpinbox*)pe)
	
	GSourceListener* psl;
	GEvent* pe;

	// Trigger a GWIN spinbox event
	psl = 0;

	while ((psl = geventGetSourceListener(GWIDGET_SOURCE, psl))) {
		if (!(pe = geventGetEventBuffer(psl)))
			continue;

		// Fill in the event
		ple->type = GEVENT_GWIN_SPINBOX;
		ple->gwin = (GHandle) gsw;
		ple->value = gsw->initial;
#if GWIN_WIDGET_TAGS
		ple->tag = gw->tag;
#endif

		geventSendEvent(psl);
	}
	
	#undef ple
}

static void mouseUp(GWidgetObject* gw, coord_t x, coord_t y)
{
	#define gsw	((GSpinboxObject*)gw)
	
	// Store the current value so we can figure out whether the value
	// changed as we don't want to send events if the value remains
	// the same.
	int oldValue = gsw->initial;

	// Only handle releases on the up/down buttons
	if (y <= gw2ButtonSize) {
		// The 'down' area was released
		if (x >= gw->g.width - gw2ButtonSize) {
			gsw->initial -= gsw->increment;
			if (gsw->initial < gsw->minval)
				gsw->initial = gsw->minval;
		}

		else if (x <= gw->g.x + gw2ButtonSize) {
			// The 'up' area was released
			gsw->initial += gsw->increment;
			if (gsw->initial > gsw->maxval)
				gsw->initial = gsw->maxval;
		}
		
		// Otherwise, don't do anything
		else {
			return;
		}

		// Issue a redraw. Use partial redrawing.
		gw->g.flags |= GSPINBOX_NUM_REDRAW;
		_gwinUpdate((GHandle) gw);

		// Spinbox events are sent upon mouse release
		if (gsw->initial != oldValue)
			sendSpinboxEvent(gsw);	
	}

	#undef gsw
}

static const gwidgetVMT SpinboxVMT = {
	{
		"Spinbox",					// The class name
		sizeof(GSpinboxObject),		// The object size
		_gwidgetDestroy,			// The destroy routine
		_gwidgetRedraw, 			// The redraw routine
		0,							// The after-clear routine
	},
	gwinSpinboxDefaultDraw,			// Default drawing routine
#if GINPUT_NEED_MOUSE
	{ 	0,							// Process mouse down events
		mouseUp,					// Process mouse up events
		0,							// Process mouse move events
	},
#endif
#if GINPUT_NEED_KEYBOARD || GWIN_NEED_KEYBOARD
	{
		0							// Process keyboard key down events
	},
#endif
#if GINPUT_NEED_TOGGLE
	{
		0,							// No toggle role
		0,							// Assign Toggles
		0,							// Get Toggles
		0,							// Process toggle off event
		0,							// Process toggle on event
	},
#endif
#if GINPUT_NEED_DIAL
	{
		0,							// Dial roles
		0,							// Assign Dials
		0,							// Get Dials
		0,							// Procees dial move events
	},
#endif
};

GHandle gwinGSpinboxTxtCreate(GDisplay* g, GSpinboxObject* wt, GWidgetInit* pInit,  const char** textArray, uint8_t numEntries)
{
	// Create the widget
	if (!(wt = (GSpinboxObject*)_gwidgetCreate(g, &wt->w, pInit, &SpinboxVMT))) {
		return 0;
	}

	// Initialize
	wt->initial = 0;				// Set to first item in string array
	wt->minval = 0;					// Set minval to first text item index
	wt->maxval = numEntries-1;		// Set maxval to last text item index
	wt->increment = 1;
	wt->txtArray = textArray;
	wt->w.g.flags |= GSPINBOX_TXT;	// Set flag for text spinbox
	gwinSetVisible(&wt->w.g, pInit->g.show);

	return (GHandle)wt;
}

GHandle gwinGSpinboxNumCreate(GDisplay* g, GSpinboxObject* wt, GWidgetInit* pInit,
		int init, int min, int max, int step, const char* mark,
		short places, const char* units) {

	// Create the widget
	if (!(wt = (GSpinboxObject*) _gwidgetCreate(g, &wt->w, pInit, &SpinboxVMT))) {
		return 0;
	}

	// Initialize
	wt->initial = init;
	wt->minval = min;
	wt->maxval = max;
	wt->increment = step;
	wt->decimalmark = mark;
	wt->placesOrTxtlen = places;
	wt->strData = units;
	wt->w.g.flags |= GSPINBOX_NUM;		// Set flag for numeric spinbox
	gwinSetVisible(&wt->w.g, pInit->g.show);

	return (GHandle)wt;
}

void gwinSpinboxDefaultDraw(GWidgetObject* gw, void* param)
{
	#define gsw	((GSpinboxObject*)gw)

	(void)param;

	char p[MAXCHARS];					// spinbox text max char
	char* ptr;
	const GColorSet* ps;
	coord_t num=0;
	coord_t arrowSize, border;

	// is it a valid handle?
	if (gw->g.vmt != (gwinVMT*)&SpinboxVMT) {
		return;
	}

	// Retrieve colours
	if ((gw->g.flags & GWIN_FLG_SYSENABLED)) {
		ps = &gw->pstyle->enabled;
	} else {
		ps = &gw->pstyle->disabled;
	}

	if (gw->g.flags & GSPINBOX_NUM) {
		// Add padding zeroes for required decimalplaces
		if (gsw->placesOrTxtlen >= num) {
			while (num <= gsw->placesOrTxtlen)
				num++;		// If num <= places, (not including sign), add 1 to num for each additional place inc leading 0
			num++;			// Add 1 for digit before decimal mark
		}

		// format text string for display including sign and decimal mark if required
		ptr = &p[MAXCHARS - 1];		// End of buffer
		ptr = fmtNum(gsw->initial, ptr, gsw->placesOrTxtlen, gsw->decimalmark);
	}
	else if (gw->g.flags & GSPINBOX_TXT) {
		ptr = (char*)gsw->txtArray[gsw->initial];
	}

	// Only numeric/text field requires updating if GSPINBOX_NUM_REDRAW flag set
	if ((gw->g.flags & GSPINBOX_NUM_REDRAW)) {

		// Fill the stringbox with value - justifyRight
		gdispGFillStringBox(gw->g.display, gw->g.x + gw2ButtonSize + 1 + TEXTPADDING,
				gw->g.y + 1, gw->g.width - 2 * gw2ButtonSize - gdispGetStringWidth(gsw->strData, gw->g.font) - 6, gw->g.height - 2, ptr,
				gw->g.font, ps->text, gw->pstyle->background, justifyRight);
				gw->g.flags &= ~GSPINBOX_NUM_REDRAW;		// Reset flag

	} else {

		border = gw2ButtonSize / 8;				// border is one eighth of gw2ButtonWidth
		arrowSize = border*6;					// arrowsize is 0.75 * gw2ButtonWidth

		#if GDISP_NEED_CONVEX_POLYGON
			point upArrow[3] = {
				{ 0, arrowSize },
				{ arrowSize + 1, arrowSize },
				{ arrowSize / 2 + 1, 0 }
			};

			point downArrow[3] = {
				{ 0, -arrowSize },
				{ arrowSize, -arrowSize },
				{ arrowSize / 2, 0 }
			};
		#endif

		// Draw the border
		gdispGDrawBox(gw->g.display, gw->g.x, gw->g.y, gw->g.width,
				gw->g.height, ps->edge);

		// Fill left button box background area
		gdispGFillArea(gw->g.display, gw->g.x + 1,
				gw->g.y + 1, gw2ButtonSize-1, gw->g.height - 2,
				gdispBlendColor(ps->fill, gw->pstyle->background, 128));

		// Fill right button box background area
		gdispGFillArea(gw->g.display, gw->g.x + gw->g.width - gw2ButtonSize,
				gw->g.y + 1, gw2ButtonSize-1, gw->g.height - 2,
				gdispBlendColor(ps->fill, gw->pstyle->background, 128));

		// Draw left vertical line
		gdispGDrawLine(gw->g.display, gw->g.x + gw2ButtonSize,
				gw->g.y + 1, gw->g.x + gw2ButtonSize,
				gw->g.y + gw->g.height - 2, ps->edge);

		// Draw right vertical line
		gdispGDrawLine(gw->g.display, gw->g.x + gw->g.width - gw2ButtonSize,
				gw->g.y + 1, gw->g.x + gw->g.width - gw2ButtonSize,
				gw->g.y + gw->g.height - 2, ps->edge);

		#if GDISP_NEED_CONVEX_POLYGON
			// Up Arrow
			gdispGFillConvexPoly(gw->g.display,gw->g.x + border, gw->g.y + border, upArrow, 3, ps->edge);
			// Down Arrow
			gdispGFillConvexPoly(gw->g.display,gw->g.x + gw->g.width - arrowSize - border,gw->g.y + gw->g.height - border,downArrow,3,ps->edge);
		#else
			#warning "GWIN: Spinbox will display arrow symbols when GDISP_NEED_CONVEX_POLYGON is turned on"
			// Plus symbol horizontal line
			gdispGDrawLine(gw->g.display, gw->g.x + border,
				gw->g.y + gw->g.height / 2, gw->g.x + gw2ButtonSize - border,
				gw->g.y + gw->g.height / 2, ps->edge);

			// Plus symbol vertical line
			gdispGDrawLine(gw->g.display, gw->g.x + gw2ButtonSize/2,
				gw->g.y + border, gw->g.x + gw2ButtonSize/2,
				gw->g.y + gw->g.height - border-1, ps->edge);

			// Minus symbol horizontal line
			gdispGDrawLine(gw->g.display, gw->g.x + gw->g.width - 1 - border,
				gw->g.y + gw->g.height - gw->g.height / 2 -1, gw->g.x + gw->g.width - gw2ButtonSize + border,
				gw->g.y + gw->g.height - gw->g.height / 2 -1, ps->edge);
		#endif

		if ((gw->g.flags & GSPINBOX_NUM)) {
			// Fill the stringbox with units - justifyRight
			gdispGFillStringBox(gw->g.display, gw->g.x + gw->g.width - gw2ButtonSize - gdispGetStringWidth(gsw->strData, gw->g.font) - TEXTPADDING,
				gw->g.y + 1, gdispGetStringWidth(gsw->strData, gw->g.font),
				gw->g.height - 2, gsw->strData, gw->g.font, ps->text,
				gw->pstyle->background, justifyRight);
		}

		// Fill the stringbox with value - justifyRight
		gdispGFillStringBox(gw->g.display, gw->g.x + gw2ButtonSize + 1 + TEXTPADDING,
			gw->g.y + 1, gw->g.width - 2 * gw2ButtonSize - gdispGetStringWidth(gsw->strData, gw->g.font) - 6, gw->g.height - 2, ptr, gw->g.font, ps->text,	gw->pstyle->background, justifyRight);
	}

	#undef gsw
}

// Limit the scope of the macros. Required for single-file compilation.
#undef TEXTPADDING
#undef BOXPADDING
#undef MAXCHARS
#undef gw2obj
#undef gw2ButtonSize

#endif // GFX_USE_GWIN && GWIN_NEED_SPINBOX
