/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
 * @file	src/gwin/gwin_spinbox.h
 * @brief	GWIN Spinbox widget header file
 *
 * @defgroup Spinbox Spinbox
 * @ingroup Widgets
 *
 * @brief		Widget that spins through a text array or numeric values and returns an integer for the selection.
 *
 * @note		The numeric spinbox is created using the following syntax:
 * 				ghSpinBox1 = gwinSpinboxNumCreate(0, &wi, 10, 0, 90, 3, ",", 1, "px");
 * 				where parms = initial, min, max, step, mark, places, units, (see typedef struct GSpinboxObject)
 *
 *				The text spinbox is created using the following syntax:
 *				ghSpinBox2 = gwinSpinboxTxtCreate(0, &wi, DaysOfWeek);
 *				where DaysOfWeek is defined thus:
 *				static const char *DaysOfWeek[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday", 0};
 *
 * 			 	GDISP_NEED_CONVEX_POLYGON will display up/down arrows if set, '+' & '-' symbols if not
 *
 * @pre			GFX_USE_GDISP must be set to TRUE in your gfxconf.h
 * @pre			GFX_USE_GWIN must be set to TRUE in your gfxconf.h
 * @pre			GDISP_NEED_TEXT must be set to TRUE in your gfxconf.h
 * @pre			GWIN_NEED_SPINBOX must be set to TRUE in your gfxconf.h
 * @pre			The fonts you want to use must be enabled in your gfxconf.h
 *
 * @{
 */

#ifndef _GWIN_SPINBOX_H
#define _GWIN_SPINBOX_H

// This file is included within "src/gwin/gwin_widget.h"

/**
 * @brief	The event type for a spinbox event
 */
#define GEVENT_GWIN_SPINBOX		(GEVENT_GWIN_CTRL_FIRST+10)

/**
 * @brief	The internal spinbox flags
 * @note	Used only for writing a custom draw routine.
 * @{
 */
#define GSPINBOX_NUM_REDRAW		0x01	// Set flag for minimal redraw of numeric value or text, (no widget or units redrawn)
#define GSPINBOX_TXT			0x02	// Flag for text spinbox code flow
#define GSPINBOX_NUM			0x04	// Flag for numeric spinbox code flow
#define GSPINBOX_UPDOWN			0x08	// Increment/decrement flag

/**
 * @brief	A spinbox event
 */
typedef struct GEventGWinSpinbox {
	GEventType		type;				// The type of this event (GEVENT_GWIN_SPINBOX)
	GHandle			gwin;				// The spinbox
	#if GWIN_WIDGET_TAGS
		WidgetTag	tag;				// Tag is an short - allows user to assign a numeric ID to identify widget without knowing its type.
	#endif
	int				value;				// The spinbox value 
} GEventGWinSpinbox;


// A Spinbox widget
typedef struct GSpinboxObject {
	GWidgetObject	w;					// Base Class

	int				initial;			// Initial value and current value
	int				minval;				// Minimum numeric value
	int				maxval;				// Maximum numeric value
	int				increment;			// Numeric step value
	const char*		decimalmark;		// Decimal mark character (eg. ".", "," etc.)
	short			placesOrTxtlen;		// Number of digits after decimal point or the length of the text string
	const char*		strData;			// Inits for numeric spinbox
	const char**	txtArray;			// Pointer to array of pointers for spinbox text data
	short 			fieldwidth;			// Text field width in Text Spinbox,Value field width in Numeric Spinbox
} GSpinboxObject;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief					Create a text Spinbox widget
 * @details					A Spinbox widget is a rectangular box which can contain text or numeric data and allows the user to select
 * 							different numeric/text values with the use of mouse/touchpanel, (up/down arrows, keyboard & dial controls not supported).
 *
 * @param[in] g				The GDisplay on which the Spinbox should be displayed
 * @param[in] wt			The Spinbox structure to initialise. If this is NULL, the structure is dynamically allocated.
 * @param[in] pInit			The initialisation parameters to use.
 * @param[in] textArray		Array of strings through which the spinbox will spin through.
 * @param[in] numEntries	The number of strings in the @p textArray.
 * @param[in] numEntries	Text field pixel width in Text Spinbox,Value field width in Numeric Spinbox
 *
 * @return					NULL if there is no resultant drawing area, otherwise the widget handle.
 *
 * @note					If the initial text set is larger than maxSize then the text is truncated at maxSize characters.
 * @api
 */
GHandle gwinGSpinboxTxtCreate(GDisplay* g, GSpinboxObject* wt, GWidgetInit* pInit,  const char** textArray, uint8_t numEntries,short fieldWidth);
#define gwinSpinboxTxtCreate(wt, pInit, textArray, numEntries, fieldWidth)			gwinGSpinboxTxtCreate(GDISP, wt, pInit, textArray, numEntries, fieldWidth)


/**
 * @brief				Create a numeric Spinbox widget
 * @details				A Spinbox widget is a rectangular box which can contain text or numeric data and allows the user to select
 * 						different numeric/text values with the use of mouse/touchpanel, (up/down arrows, keyboard & dial controls not supported).
 *
 * @param[in] g			The GDisplay on which the Spinbox should be displayed
 * @param[in] wt		The Spinbox structure to initialise. If this is NULL, the structure is dynamically allocated.
 * @param[in] pInit		The initialisation parameters to use.
 * @param[in] init		The initial spinbox value.
 * @param[in] min		The minimum spinbox value.
 * @param[in] max		The maximum spinbox value.
 * @param[in] step		The increment value.
 * @param[in] mark		The mark character separator, (decimal point, comma, colon etc).
 * @param[in] places	The number of places after the mark, (0 will prevent any mark displayed).
 * @param[in] units		The units string, (blank for no units).
 *
 * @return				NULL if there is no resultant drawing area, otherwise the widget handle.
 *
 * @note				If the initial text set is larger than maxSize then the text is truncated at maxSize characters.
 * @api
 */
GHandle gwinGSpinboxNumCreate(GDisplay* g, GSpinboxObject* wt, GWidgetInit* pInit, int init, int min, int max, int step,  const char* mark,  short places, const char* units, short fieldWidth);
#define gwinSpinboxNumCreate(wt, pInit, init, min, max, step, mark, places, units, fieldWidth)			gwinGSpinboxNumCreate(GDISP, wt, pInit, init, min, max, step, mark, places, units, fieldWidth)

/**
 * @defgroup Renderings_Spinbox Renderings
 *
 * @brief				Built-in rendering functions for the Spinbox widget.
 *
 * @details				These function may be passed to @p gwinSetCustomDraw() to get different Spinbox drawing styles.
 *
 * @note				In your custom Spinbox drawing function you may optionally call these
 * 						standard functions and then draw your extra details on top.
 * @note				These custom drawing routines don't have to worry about setting clipping as the framework
 * 						sets clipping to the object window prior to calling these routines.
 *
 * @{
 */

/**
 * @brief				The default rendering function for the Spinbox widget.
 *
 * @param[in] gw		The widget object (must be a Spinbox).
 * @param[in] param		A parameter passed in from the user. Ignored by this function.
 *
 * @api
 */

void gwinSpinboxDefaultDraw(GWidgetObject* gw, void* param);
/**
 * @brief   Set the spinbox minimum and maximum values.
 *
 * @param[in] gh		The window handle (must be a spinbox window)
 * @param[in] min		The minimum value
 * @param[in] max		The maximum value
 * @param[in] step		The value to increment
 * @note				Sets the minimum, maximum and step values, returns if min > max.
 *
 * @api
 */
void gwinSpinboxSetParams (GHandle gh, int initial, int min, int max, int step, const char* decimalmark, short placesOrTxtlen, const char* units);


/** @} */

#ifdef __cplusplus
}
#endif

#endif // _GWIN_Spinbox_H
/** @} */
