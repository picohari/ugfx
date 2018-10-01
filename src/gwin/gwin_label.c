/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
 * @file	src/gwin/gwin_label.c
 * @brief	GWIN label widget header file
 */

#include "../../gfx.h"

#if GFX_USE_GWIN && GWIN_NEED_LABEL

#include "gwin_class.h"

// macros to assist in data type conversions
#define gh2obj					((GLabelObject *)gh)
#define gw2obj					((GLabelObject *)gw)

static const gwidgetVMT labelVMT = {
	{
		"Label",					// The class name
		sizeof(GLabelObject),		// The object size
		_gwidgetDestroy,			// The destroy routine
		_gwidgetRedraw, 			// The redraw routine
		0,							// The after-clear routine
	},
	gwinLabelDrawJustifiedLeft,		// default drawing routine
	#if GINPUT_NEED_MOUSE
		{
			0,						// Process mose down events (NOT USED)
			0,						// Process mouse up events (NOT USED)
			0,						// Process mouse move events (NOT USED)
		},
	#endif
	#if GINPUT_NEED_KEYBOARD || GWIN_NEED_KEYBOARD
		{
			0						// Process keyboard key down events
		},
	#endif
	#if GINPUT_NEED_TOGGLE
		{
			0,						// No toggle role
			0,						// Assign Toggles (NOT USED)
			0,						// Get Toggles (NOT USED)
			0,						// Process toggle off event (NOT USED)
			0,						// Process toggle on event (NOT USED)
		},
	#endif
	#if GINPUT_NEED_DIAL
		{
			0,						// No dial roles
			0,						// Assign Dials (NOT USED)
			0, 						// Get Dials (NOT USED)
			0,						// Procees dial move events (NOT USED)
		},
	#endif
};

GHandle gwinGLabelCreate(GDisplay *g, GLabelObject *widget, GWidgetInit *pInit) {

	if (!(widget = (GLabelObject *)_gwidgetCreate(g, &widget->w, pInit, &labelVMT)))
		return 0;

	#if GWIN_LABEL_ATTRIBUTE
		widget->tab = 0;
		widget->attr = 0;
	#endif

	gwinSetVisible(&widget->w.g, pInit->g.show);

	return (GHandle)widget;
}

void gwinLabelSetBorder(GHandle gh, gBool border) {
	// is it a valid handle?
	if (gh->vmt != (gwinVMT *)&labelVMT)
		return;
	
	if (border)
		gh2obj->w.g.flags |= GLABEL_FLG_BORDER;
	else
		gh2obj->w.g.flags &=~ GLABEL_FLG_BORDER;
}

#if GWIN_LABEL_ATTRIBUTE
	void gwinLabelSetAttribute(GHandle gh, gCoord tab, const char* attr) {
		// is it a valid handle?
		if (gh->vmt != (gwinVMT *)&labelVMT)
			return;
	
		gh2obj->tab = tab;
		gh2obj->attr = attr;

		gwinRedraw(gh); 
	}
#endif // GWIN_LABEL_ATTRIBUTE

void gwinLabelDrawJustified(GWidgetObject *gw, void *param) {
	gCoord				w, h;
	gColor				c;

	gJustify justify = (gJustify)param;

	// is it a valid handle?
	if (gw->g.vmt != (gwinVMT *)&labelVMT)
		return;

	w = gw->g.width;
	h = gw->g.height;
	c = (gw->g.flags & GWIN_FLG_SYSENABLED) ? gw->pstyle->enabled.text : gw->pstyle->disabled.text;

	#if GWIN_LABEL_ATTRIBUTE
		if (gw2obj->attr) {
			gdispGFillStringBox(gw->g.display, gw->g.x, gw->g.y, gw2obj->tab, h, gw2obj->attr, gw->g.font, c, gw->pstyle->background, justify);
			gdispGFillStringBox(gw->g.display, gw->g.x + gw2obj->tab, gw->g.y, w-gw2obj->tab, h, gw->text, gw->g.font, c, gw->pstyle->background, justify);
		} else
			gdispGFillStringBox(gw->g.display, gw->g.x, gw->g.y, w, h, gw->text, gw->g.font, c, gw->pstyle->background, justify);
	#else
		gdispGFillStringBox(gw->g.display, gw->g.x, gw->g.y, w, h, gw->text, gw->g.font, c, gw->pstyle->background, justify);
	#endif

	// render the border (if any)
	if (gw->g.flags & GLABEL_FLG_BORDER)
		gdispGDrawBox(gw->g.display, gw->g.x, gw->g.y, w, h, (gw->g.flags & GWIN_FLG_SYSENABLED) ? gw->pstyle->enabled.edge : gw->pstyle->disabled.edge);
}

void gwinLabelDrawJustifiedLeft(GWidgetObject *gw, void *param) {
	(void)param;
	
	gwinLabelDrawJustified(gw, (void *)gJustifyLeft);
}

void gwinLabelDrawJustifiedRight(GWidgetObject *gw, void *param) {
	(void)param;
	
	gwinLabelDrawJustified(gw, (void *)gJustifyRight);
}

void gwinLabelDrawJustifiedCenter(GWidgetObject *gw, void *param) {
	(void)param;
	
	gwinLabelDrawJustified(gw, (void *)gJustifyCenter);
}

#undef gh2obj
#undef gw2obj
#endif // GFX_USE_GWIN && GFX_NEED_LABEL
