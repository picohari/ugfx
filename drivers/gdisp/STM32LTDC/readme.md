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
	
# Configuration
Configuration options available in `gfxconf.h`:

| Option | Default | Description |
| --- | --- | --- |
| `STM32LTDC_DMA_CACHE_FLUSH` | `GFOFF` | Whether to flush the DMA cache on DMA2D operations. This will be turned on automatically on certian platforms/systems. |
| `STM32LTDC_USE_DMA2D` | `GFXON` | Whether to use the DMA2D peripheral for hardware acceleration. |
| `STM32LTDC_USE_LAYER2` | `GFXOFF` | Whether to use the 2nd LTDC layer. |
| `STM32LTDC_USE_RGB565` | `GFXOFF` | Whether to use RGB565 instead of RGB888. |

# 2nd layer
To use the 2nd LTDC layer, set `STM32LTDC_USE_LAYER2` to `GFXON` in `gfxconf.h`.

The 2nd layer is exposed as a separate display. Use `gdispGetDisplay()` to retrieve the individual layers.

For more information, see:
  - https://wiki.ugfx.io/index.php/Multiple_displays#Example_-_Same_controller
  - https://wiki.ugfx.io/index.php/Multiple_displays#Access_the_displays
