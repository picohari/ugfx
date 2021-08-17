# Usage
To use this driver:

1. Add in your `gfxconf.h`:
	a) `#define GFX_USE_GDISP		GFXON`

2. To your makefile add the following lines:
```
	include $(GFXLIB)/gfx.mk
	include $(GFXLIB)/drivers/gdisp/STM32LTDC/driver.mk
```

3. Add a `board_STM32LTDC.h` to you project directory (or board directory)
	based on one of the templates.

# 2nd layer
Two things need to happen in order to use the 2nd LTDC layer:
  - Set `LTDC_USE_2ND_LAYER` to `GFXON` in the board file.
  - Set `GDISP_TOTAL_DISPLAYS` to `2` in `gfxconf.h`.

The 2nd layer is exposed as a separate display. Use `gdispGetDisplay()` to retrieve the individual layers.

For more information, see:
  - https://wiki.ugfx.io/index.php/Multiple_displays#Example_-_Same_controller
  - https://wiki.ugfx.io/index.php/Multiple_displays#Access_the_displays
